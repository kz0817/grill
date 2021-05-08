AC_INIT([grill], [0.0.1])
AM_INIT_AUTOMAKE([foreign])

AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_PROG_CXX

AC_CONFIG_MACRO_DIR([m4])
AM_SILENT_RULES([yes])

CXXFLAGS="$CXXFLASG -I \${top_srcdir}/include -Wall -O3 -g3 -std=c++2a"

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  test/Makefile
])
AC_OUTPUT
