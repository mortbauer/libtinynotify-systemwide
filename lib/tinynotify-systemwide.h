/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#ifndef _TINYNOTIFY_SYSTEMWIDE_H
#define _TINYNOTIFY_SYSTEMWIDE_H

#include <tinynotify.h>

/**
 * SECTION: systemwide
 * @short_description: API to handle system-wide notifications
 * @include: tinynotify-systemwide.h
 *
 * The libtinynotify-systemwide helper library provides the routines to send
 * notifications system-wide, i.e. on all D-Bus session buses in the system.
 *
 * Although libtinynotify-cli links with the actual libtinynotify itself,
 * and uses its header, one should refer to the libtinynotify pkg-config
 * package explicitly and include its header anyway.
 */

/**
 * NOTIFY_ERROR_SYSCALL_FAILED
 *
 notification_send_systemwide* An error raised when one of the syscalls required for the operation fails.
 */
extern const NotifyError NOTIFY_ERROR_SYSCALL_FAILED;
/**
 * NOTIFY_ERROR_UIDS_COMPROMISED
 *
 * An error raised when setresuid() fails to restore the original caller UID.
 *
 * This means that the application has lost its original UID and should
 * terminate ASAP.
 */
extern const NotifyError NOTIFY_ERROR_UIDS_COMPROMISED;
/**
 * NOTIFY_ERROR_NO_BUS_FOUND
 *
 * An error returned by notification_send_systemwide() when it was unable
 * to find a single D-Bus session bus in procfs.
 *
 * Please note that this as well covers a case when the session bus is not
 * visible to user in procfs due to limited privileges.
 */
extern const NotifyError NOTIFY_ERROR_NO_BUS_FOUND;

/**
 * notification_send_systemwide
 * @notification: the notification to send
 * @session: session to send the notification through
 *
 * Send a notification to all notification daemons found (in procfs).
 *
 * Todo: support formatstring args.
 *
 * Returns: number of notifications actually sent or 0 if no daemon reached
 * (one can use notify_session_get_error_message() to get more details on that)
 */
int notification_send_systemwide(Notification notification, NotifySession session);

#endif
