#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([locationforecastLoad], [0.2.0], [wdb@met.no])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/locationforecast/Document.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR ([m4])


AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

PKG_CHECK_MODULES(libwdbLoad, libwdbLoad)
PKG_CHECK_MODULES(libxmlpp, libxml++-2.6)
PKG_CHECK_MODULES(libcurl, libcurl)

GTEST_CHECK

AC_PATH_PROG([HELP2MAN], [help2man])
test -z $HELP2MAN && AC_MSG_ERROR([Unable ot find help2man program])

AC_HEADER_STDBOOL
AC_C_INLINE

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
