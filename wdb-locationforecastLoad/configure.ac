#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([locationforecastLoad], [0.2.0], [wdb@met.no])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/locationforecast/Document.cpp])
AC_CONFIG_HEADERS([config.h])


AC_PROG_CXX
AC_PROG_CC


PKG_CHECK_MODULES(libwdbLoad, libwdbLoad)


AC_HEADER_STDBOOL
AC_C_INLINE

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
