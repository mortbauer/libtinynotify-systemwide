/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#ifndef _TINYNOTIFY_CLI_H
#define _TINYNOTIFY_CLI_H

#include <tinynotify.h>

/**
 * SECTION: systemwide
 * @short_description: API to handle system-wide notifications
 * @include: tinynotify-systemwide.h
 *
 * The libtinynotify-systemwide helper library provides the routines to send
 * notifications system-wide, i.e. on all D-Bus session buses in the system.
 */

/**
 * notification_send_systemwide
 * @notification: the notification to send
 * @session: session to send the notification through
 *
 * Send a notification to all notification daemons found.
 *
 * Todo: support formatstring args.
 *
 * Returns: number of notifications actually sent or 0 if no daemon reached
 */
int notification_send_systemwide(Notification notification, NotifySession session);

#endif
