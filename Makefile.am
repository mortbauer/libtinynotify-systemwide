ACLOCAL_AMFLAGS = -I m4
DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

SUBDIRS = doc

pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libtinynotify-systemwide.la
pkgconfig_DATA = libtinynotify-systemwide.pc
include_HEADERS = lib/tinynotify-systemwide.h

libtinynotify_systemwide_la_LDFLAGS = -version-info 0:0:0
libtinynotify_systemwide_la_CPPFLAGS = $(LIBTINYNOTIFY_CFLAGS)
libtinynotify_systemwide_la_LIBADD = $(LIBTINYNOTIFY_LIBS)
libtinynotify_systemwide_la_SOURCES = lib/tinynotify-systemwide.c lib/tinynotify-systemwide.h

bin_PROGRAMS = sw-notify-send

sw_notify_send_CPPFLAGS = -I$(top_srcdir)/lib $(LIBTINYNOTIFY_CFLAGS) $(LIBTINYNOTIFY_CLI_CFLAGS)
sw_notify_send_LDADD = $(LIBTINYNOTIFY_LIBS) $(LIBTINYNOTIFY_CLI_LIBS) libtinynotify-systemwide.la
sw_notify_send_SOURCES = sw-notify-send.c lib/tinynotify-systemwide.h
