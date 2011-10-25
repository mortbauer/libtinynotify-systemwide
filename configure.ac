AC_PREREQ([2.60])
AC_INIT([libtinynotify-systemwide], [0.0.1])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.6 foreign dist-bzip2 no-dependencies -Wall])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES])

AC_PROG_CC
AM_PROG_CC_C_O
AC_USE_SYSTEM_EXTENSIONS
LT_INIT([disable-static])
GTK_DOC_CHECK([1.15])

PKG_CHECK_MODULES([LIBTINYNOTIFY], [libtinynotify])
AC_CHECK_FUNCS([setresuid],,
	[AC_MSG_ERROR([One of required system library functions was not found!])])
AC_CHECK_LIB([proc], [readproc],,
	[AC_MSG_ERROR([Error: required library proc not found! Please install procps.])])

AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--disable-debug],
		[Disable debugging asserts])])
AS_IF([test x"$enable_debug" = x"no"], [
	AC_DEFINE([NDEBUG], [1], [Set this to disable debugging asserts])
])

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile libtinynotify-systemwide.pc])
AC_OUTPUT
