#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([locationforecastLoad], [0.7.2], [wdb@met.no])
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

#AX_CHECK_COMPILE_FLAG([-std=c++0x], [CXXFLAGS="$CXXFLAGS -std=c++0x"])
#AX_CHECK_COMPILE_FLAG([-std=c++0x], [CXXFLAGS="$CXXFLAGS -g"])
#AX_CHECK_COMPILE_FLAG([-std=c++0x], [CXXFLAGS="$CXXFLAGS -O0"])

AC_ARG_WITH([xml_parser],
    [AS_HELP_STRING([--with-xml-parser], [Specify xml parser (libxml (default) or boost) ])],
    [xmlparser=${with_xml_parser}],
    [xmlparser=libxml])
AM_CONDITIONAL(BOOST_XML_PARSE, [test x${xmlparser} = xboost])
if test x${xmlparser} != xboost -a x${xmlparser} != xlibxml; then
   AC_MSG_ERROR([Value for --with-xml-parser must be libxml or boost])
fi

forbedre sjekk av libxml, så det bare gjres når det trengs

GTEST_CHECK

AC_PATH_PROG([HELP2MAN], [help2man])
test -z $HELP2MAN && AC_MSG_ERROR([Unable ot find help2man program])

AC_HEADER_STDBOOL
AC_C_INLINE

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
