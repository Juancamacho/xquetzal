
AC_PREREQ(2.60)
AC_INIT([xquetzal],[0.1],[spartan3004@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.9 -Wall -Werror dist-bzip2])
#AM_INIT_AUTOMAKE([subdir-object])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
# Compiling sources with per-target flags requires AM_PROG_CC_C_O
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LIBTOOL

GTK_REQUIRED_VERSION=3.0.0
PKG_CHECK_MODULES(GTK, gtk+-3.0 >= $GTK_REQUIRED_VERSION)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)
#AM_PATH_GTK_3_0([gtk+-3.0],,AC_MSG_ERROR([Gtk+ 3.0 or higher required.]))

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
        Makefile
        src/Makefile
        tests/Makefile
])
AC_OUTPUT
