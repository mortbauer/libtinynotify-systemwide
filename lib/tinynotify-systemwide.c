/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#include "config.h"
#include "tinynotify-systemwide.h"
#include <tinynotify.h>

#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <pwd.h>

#include <proc/readproc.h>

#include <assert.h>

const struct notify_error _error_syscall_failed = { "Required system call %s failed: %s" };
const NotifyError NOTIFY_ERROR_SYSCALL_FAILED = &_error_syscall_failed;
const struct notify_error _error_uids_compromised = { "Required system call setresuid() failed to restore UIDs: %s" };
const NotifyError NOTIFY_ERROR_UIDS_COMPROMISED = &_error_uids_compromised;
const struct notify_error _error_no_bus_found = { "No D-Bus session bus process found" };
const NotifyError NOTIFY_ERROR_NO_BUS_FOUND = &_error_no_bus_found;

static int _proc_matches_dbus_session(proc_t* const p) {
	const char *procname;
	char* const *ap;

	if (!p->cmdline)
		return 0;

	/* Check whether the binary name matches. */
	procname = basename(p->cmdline[0]);
	if (strcmp(procname, "dbus-daemon"))
		return 0;

	/* Lookup supplied command-line argument list for '--session'.
	 * We don't have to worry about additional '--system' arguments as
	 * dbus refuses to run with multiple configuration files supplied.
	 */
	for (ap = &(p->cmdline[1]); *ap; ap++)
		if (!strcmp(*ap, "--session"))
			return 1;

	return 0;
}

static char *_proc_getenv(const proc_t* const p, const char* const keystr) {
	char* const *ap;
	const int matchlen = strlen(keystr);

	assert(keystr[matchlen-1] == '=');

	if (!p->environ)
		return NULL;

	for (ap = p->environ; *ap; ap++) {
		if (!strncmp(*ap, keystr, matchlen))
			return *ap;
	}

	return NULL;
}

struct _sys_save_state {
	int filled_in;

	uid_t ruid;
	uid_t euid;
	uid_t suid;
};

static int _notification_send_for_bus(Notification n, NotifySession s,
		uid_t uid, struct _sys_save_state *saved_state) {
	int uids_switched = 0;
	int ret = 0;

	if (!saved_state->filled_in) {
		if (getresuid(&saved_state->ruid, &saved_state->euid,
					&saved_state->suid)) {
			notify_session_set_error(s, NOTIFY_ERROR_SYSCALL_FAILED,
					"getresuid()", strerror(errno));
			return 0;
		}

		saved_state->filled_in = 1;
	}

	/* We need to use setresuid() because D-Bus checks uid & euid,
	 * and we need to be able to return to root */
	if (!setresuid(uid, uid, saved_state->suid))
		uids_switched++;

	/* Ensure getting a new connection. */
	notify_session_disconnect(s);
	ret = !notification_send(n, s);

	if (uids_switched && setresuid(saved_state->ruid, saved_state->euid,
				saved_state->suid)) {
		notify_session_set_error(s, NOTIFY_ERROR_UIDS_COMPROMISED, strerror(errno));
		saved_state->filled_in = 0;
		/* XXX: what if notification_send() succeeded? */
		return 0;
	}

	return ret;
}

int notification_send_systemwide(Notification n, NotifySession s) {
	PROCTAB *proc;
	proc_t *p = NULL;
	int ret = 0;
	struct _sys_save_state saved_state = {0};

	proc = openproc(PROC_FILLCOM | PROC_FILLENV);
	if (!proc) {
		notify_session_set_error(s, NOTIFY_ERROR_SYSCALL_FAILED,
				"openproc()", strerror(errno) /* XXX */);
		return 0;
	}

	notify_session_set_error(s, NOTIFY_ERROR_NO_BUS_FOUND);

	while (((p = readproc(proc, p)))) {
		if (_proc_matches_dbus_session(p)) {
			char* const display = _proc_getenv(p, "DISPLAY=");
			char* const xauth = _proc_getenv(p, "XAUTHORITY=");
			char* home = _proc_getenv(p, "HOME=");
			const struct passwd* const pw = getpwuid(p->euid); /* XXX */

			/* All of the following are necessary to proceed. */
			if (!display || !pw || (!xauth && !home && !pw->pw_dir))
				continue;

			/* Set environment early;
			 * we won't have to worry about UIDs if it fails. */
			if (putenv(display)) {
				notify_session_set_error(s, NOTIFY_ERROR_SYSCALL_FAILED,
						"putenv(DISPLAY)", strerror(errno));
				break;
			}
			if (xauth) {
				if (putenv(xauth)) {
					notify_session_set_error(s, NOTIFY_ERROR_SYSCALL_FAILED,
							"putenv(XAUTHORITY)", strerror(errno));
					break;
				}
			} else {
				if (unsetenv("XAUTHORITY")) {
					notify_session_set_error(s, NOTIFY_ERROR_SYSCALL_FAILED,
							"unsetenv(XAUTHORITY)", strerror(errno));
					break;
				}
				if (home) {
					if (putenv(home)) {
						notify_session_set_error(s, NOTIFY_ERROR_SYSCALL_FAILED,
								"putenv(HOME)", strerror(errno));
						break;
					}
				} else {
					if (setenv("HOME", pw->pw_dir, 1)) {
						notify_session_set_error(s, NOTIFY_ERROR_SYSCALL_FAILED,
								"setenv(HOME)", strerror(errno));
						break;
					}
				}
			}

			if (_notification_send_for_bus(n, s, p->euid, &saved_state))
				ret += 1;
			else if (!saved_state.filled_in) /* oh no, early failure! */
				break;
		}
	}
	closeproc(proc);

	return ret;
}
