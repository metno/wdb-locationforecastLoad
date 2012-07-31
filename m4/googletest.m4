AC_DEFUN([GTEST_CHECK],
[
AC_ARG_WITH([gtest],
    [AS_HELP_STRING([--with-gtest], [Specify google test directory])],
    [gtest_base=${with_gtest}],
    [gtest_base=/usr])

AC_LANG_PUSH(C++)

includes_old="${INCLUDES}"
AS_IF([test "x$gtest_base" = "x/usr"],
    [],
    [gtest_includes="-I${gtest_base}/include"])

INCLUDES="${INCLUDES} ${gtest_includes}"
AC_CHECK_HEADER([gtest/gtest.h],
    [],
    [AC_MSG_ERROR([Unable to find header gtest/gtest.h])])

INCLUDES="${includes_old}"

have_compiled_gtest=no
ldflags_old="${LDFLAGS}"
AS_IF([test "x$gtest_base" = "x/usr"],
    [],
    [gtest_ldflags="-L${gtest_base}/lib"])

LDFLAGS="${LDFLAGS} ${gtest_ldflags}"
AC_CHECK_LIB([gtest_main],
    [main],
    [have_compiled_gtest=yes],
    [AC_CHECK_FILE([${gtest_base}/src/gtest/src/gtest-all.cc],[],
        [AC_MSG_ERROR([Unable to find precompiled gtest_main or gtest-all.cc])])])

LDFLAGS="${ldflags_old}"
AS_IF([test "x${have_compiled_gtest}" = "xyes"],
    [gtest_libs="-lgtest"],
    [gtest_ldflags=""])

AC_LANG_POP(C++)

AC_SUBST([gtest_BASE],[${gtest_base}])
AC_SUBST([gtest_CFLAGS], [${gtest_includes}])
AC_SUBST([gtest_LDFLAGS], [${gtest_ldflags}])
AC_SUBST([gtest_LIBS], [${gtest_libs}])
AM_CONDITIONAL([HAVE_COMPILED_GTEST],[test "x${have_compiled_gtest}" = "xyes"])
AM_CONDITIONAL([HAVE_GTEST],[test "x${have_compiled_gtest}" = "xyes"])
])


AC_DEFUN([GMOCK_CHECK],
[
AC_ARG_WITH([gmock],
    [AS_HELP_STRING([--with-gmock], [Specify google mock directory])],
    [gtest_base=${with_gmock}],
    [gmock_base=/usr])

AC_LANG_PUSH(C++)

includes_old="${INCLUDES}"
AS_IF([test "x$gmock_base" = "x/usr"],
    [],
    [gmock_includes="-I${gmock_base}/include"])

INCLUDES="${INCLUDES} ${gmock_includes}"
AC_CHECK_HEADER([gmock/gmock.h],
    [],
    [AC_MSG_ERROR([Unable to find header gmock/gmock.h])])

INCLUDES="${includes_old}"

ldflags_old="${LDFLAGS}"
AS_IF([test "x$gmock_base" = "x/usr"],
    [],
    [gmock_ldflags="-L${gmock_base}/lib"])

LDFLAGS="${LDFLAGS} ${gmock_ldflags}"
#AC_CHECK_LIB([gmock], [], [gmock_libs=-lgmock])
gmock_libs=-lgmock

LDFLAGS="${ldflags_old}"

AC_LANG_POP(C++)

AC_SUBST([gmock_BASE],[${gmock_base}])
AC_SUBST([gmock_CXXFLAGS], [${gmock_includes}])
AC_SUBST([gmock_LDFLAGS], [${gmock_ldflags}])
AC_SUBST([gmock_LIBS], [${gmock_libs}])
AM_CONDITIONAL([HAVE_GMOCK],[true])

])
