AC_INIT([died], [0.2], [hagen.moebius@starschiffchen.de])
AC_CONFIG_SRCDIR([Source])
AM_INIT_AUTOMAKE([dist-bzip2])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(GLIBMM, glibmm-2.4)
PKG_CHECK_MODULES(GTKMM, gtkmm-2.4)

AC_CONFIG_FILES([Makefile Source/Makefile Source/DiED/Makefile Source/GUI/Makefile Source/Network/Makefile])
AC_OUTPUT
