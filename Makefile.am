ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libtinynotify-systemwide.la
pkgconfig_DATA = libtinynotify-systemwide.pc
include_HEADERS = lib/tinynotify-systemwide.h

libtinynotify_systemwide_la_LDFLAGS = -version-info 0:0:0
libtinynotify_systemwide_la_CPPFLAGS = $(LIBTINYNOTIFY_CFLAGS)
libtinynotify_systemwide_la_LIBADD = $(LIBTINYNOTIFY_LIBS)
libtinynotify_systemwide_la_SOURCES = lib/tinynotify-systemwide.c lib/tinynotify-systemwide.h+

EXTRA_DIST = NEWS
NEWS: configure.ac Makefile.am
	git for-each-ref refs/tags --sort '-*committerdate' \
		--format '# %(tag) (%(*committerdate:short))%0a%(contents:body)' \
		> NEWS
