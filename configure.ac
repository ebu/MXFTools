dnl-----------------------------------------------------------
dnl -- Extract package info
dnl-----------------------------------------------------------

m4_define([m4_MXFTOOLS_LIBRARY_NAME],
m4_esyscmd([(sed -n 's/^.*MXFTOOLS_LIBRARY_NAME[^\"]*\"\([^\"]*\)\"/\1/p' version.h | tr -d '\n')]))
m4_define([m4_MXFTOOLS_VERSION_MAJOR],
m4_esyscmd([(sed -n 's/^.*MXFTOOLS_VERSION_MAJOR\ *\([0-9][0-9]*\)/\1/p' version.h | tr -d '\n')]))
m4_define([m4_MXFTOOLS_VERSION_MINOR],
m4_esyscmd([(sed -n 's/^.*MXFTOOLS_VERSION_MINOR\ *\([0-9][0-9]*\)/\1/p' version.h | tr -d '\n')]))
m4_define([m4_MXFTOOLS_VERSION_MICRO],
m4_esyscmd([(sed -n 's/^.*MXFTOOLS_VERSION_MICRO\ *\([0-9][0-9]*\)/\1/p' version.h | tr -d '\n')]))
m4_define([m4_MXFTOOLS_BUGREPORT],
m4_esyscmd([(sed -n 's/^.*MXFTOOLS_BUGREPORT[^\"]*\"\([^\"]*\)\"/\1/p' version.h | tr -d '\n')]))
m4_define([m4_MXFTOOLS_TARNAME],
m4_esyscmd([(sed -n 's/^.*MXFTOOLS_TARNAME[^\"]*\"\([^\"]*\)\"/\1/p' version.h | tr -d '\n')]))
m4_define([m4_MXFTOOLS_URL],
m4_esyscmd([(sed -n 's/^.*MXFTOOLS_URL[^\"]*\"\([^\"]*\)\"/\1/p' version.h | tr -d '\n')]))

m4_define([m4_MXFTOOLS_VERSION],
m4_defn([m4_MXFTOOLS_VERSION_MAJOR]).m4_defn([m4_MXFTOOLS_VERSION_MINOR]).m4_defn([m4_MXFTOOLS_VERSION_MICRO]))

dnl-----------------------------------------------------------
dnl -- Init
dnl-----------------------------------------------------------

AC_PREREQ(m4_defn([AC_AUTOCONF_VERSION]))

AC_INIT(m4_defn([m4_MXFTOOLS_LIBRARY_NAME]), m4_defn([m4_MXFTOOLS_VERSION]), m4_defn([m4_MXFTOOLS_BUGREPORT]), m4_defn([m4_MXFTOOLS_TARNAME]), m4_defn([m4_MXFTOOLS_URL]))

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR(m4)

AM_INIT_AUTOMAKE(-W -Wall -Wno-extra-portability)
CXXFLAGS="-O3 -Wall -Wextra $CXXFLAGS"

dnl-----------------------------------------------------------
dnl -- Version
dnl-----------------------------------------------------------

MXFTOOLS_MAJORMINOR=m4_defn([m4_MXFTOOLS_VERSION_MAJOR]).m4_defn([m4_MXFTOOLS_VERSION_MINOR])
AC_SUBST(MXFTOOLS_MAJORMINOR)

dnl-----------------------------------------------------------
dnl -- Checks for programs 
dnl-----------------------------------------------------------

AC_PROG_CXX
AC_LANG_CPLUSPLUS
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_CHECK_PROGS(MD5TOOL, [md5sum md5])
if test x"$MD5TOOL" = x"" ; then
	AC_MSG_WARN(Missing md5sum/md5 tool. Cannot make check.)
fi

PKG_CHECK_MODULES(GTKMM30, [gtkmm-3.0 >= 3.6.0])
AC_SUBST(GTKMM30_CFLAGS)
AC_SUBST(GTKMM30_LIBS)
PKG_CHECK_MODULES(GDKMM30, [gdkmm-3.0 >= 3.6.0])
AC_SUBST(GDKMM30_CFLAGS)
AC_SUBST(GDKMM30_LIBS)
PKG_CHECK_MODULES(GSTREAMERMM010, [gstreamermm-0.10 >= 0.10.11])
AC_SUBST(GSTREAMERMM010_CFLAGS)
AC_SUBST(GSTREAMERMM010_LIBS)
PKG_CHECK_MODULES(XERCESC, [xerces-c >= 3.1.1])
AC_SUBST(XERCESC_CFLAGS)
AC_SUBST(XERCESC_LIBS)
PKG_CHECK_MODULES(LIBMXF10, [libMXF-1.0 >= 1.0.0])
AC_SUBST(LIBMXF10_CFLAGS)
AC_SUBST(LIBMXF10_LIBS)
PKG_CHECK_MODULES(LIBMXF10, [libMXF++-1.0 >= 1.0.0])
AC_SUBST(LIBMXF10_CFLAGS)
AC_SUBST(LIBMXF10_LIBS)
PKG_CHECK_MODULES(BMX01, [bmx-0.1 >= 0.1.0])
AC_SUBST(BMX01_CFLAGS)
AC_SUBST(BMX01_LIBS)
PKG_CHECK_MODULES(EBUMXFSDK10, [ebu-mxfsdk-1.0 >= 1.0.0])
AC_SUBST(EBUMXFSDK10_CFLAGS)
AC_SUBST(EBUMXFSDK10_LIBS)

dnl-----------------------------------------------------------
dnl -- Checks for header files
dnl-----------------------------------------------------------

AC_CHECK_HEADERS([ctime cstring cstdio cstdlib])

dnl-----------------------------------------------------------
dnl--Checks for typedefs, structures, compiler characteristics
dnl-----------------------------------------------------------

AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_HEADER_TIME
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

dnl -- mingw doesn't support the 'z' printf format length modifier for size_t
case "$host" in
	*-*-*mingw32) AC_DEFINE([PRIszt], ["u"], [Define printf size_t format specifier]) ;;
			   *) AC_DEFINE([PRIszt], ["zu"], [Define printf size_t format specifier])
esac

dnl-----------------------------------------------------------
dnl -- Checks for library functions
dnl-----------------------------------------------------------

AC_SYS_LARGEFILE
AC_FUNC_FSEEKO
AC_CHECK_FUNCS([getcwd gettimeofday memmove memset mkdir strerror])

dnl-----------------------------------------------------------
dnl -- Package Config
dnl-----------------------------------------------------------

pkgconfigdir="\$(libdir)/pkgconfig"
AC_SUBST(pkgconfigdir)

dnl-----------------------------------------------------------
dnl -- Config files
dnl-----------------------------------------------------------

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT