/* System-wide tinynotify-send
 * (c) 2011 Michał Górny
 * Released under the terms of 2-clause BSD license
 */

#include "config.h"

#include <tinynotify.h>
#include <tinynotify-cli.h>
#include "tinynotify-systemwide.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
	NotifySession s;
	Notification n;

	n = notification_new_from_cmdline(argc, argv,
			"sw-notify-send " PACKAGE_VERSION);
	if (!n)
		return 1;

	s = notify_session_new("sw-notify-send", NOTIFY_SESSION_NO_APP_ICON);
	if (!notification_send_systemwide(n, s))
		fprintf(stderr, "%s\n", notify_session_get_error_message(s));

	notify_session_free(s);
	notification_free(n);

	return 0;
}
