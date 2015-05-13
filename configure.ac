# Process this file with autoconf to produce a configure script.
# Copyright (c) 2003-2013  Pavel Rychly

AC_COPYRIGHT([Copyright (c) 2003-2013, Pavel Rychly])
AC_INIT(manatee, open-2.83.3, pary@fi.muni.cz)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR(config)
AC_CONFIG_HEADER(config.hh)
AC_CONFIG_SRCDIR(corp/corpus.hh)
AM_INIT_AUTOMAKE
AC_CONFIG_SUBDIRS(libltdl)


# Checks for programs.
AC_PROG_CXX

# Check for ln -s, fall back to cp -p
AC_PROG_LN_S

# Checks for opening shared libraries suite.
AC_LIBTOOL_DLOPEN
AC_LIBLTDL_CONVENIENCE
AC_PROG_LIBTOOL
AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)

AC_ARG_ENABLE(pthread, [  --disable-pthread    Disable the threading with pthread.],,
		    enable_pthread="yes")
if test "x$enable_pthread" = "xyes" ; then
    AC_DEFINE(HAVE_PTHREAD, 1, [Define to 1 if you have the pthread library])
    PTHREAD_LIBS="-pthread"
fi
AC_SUBST(PTHREAD_LIBS)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([locale.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_OFF_T
AC_SYS_LARGEFILE
AC_CXX_HAVE_EXT_HASH_MAP

# Checks for library functions.
AC_FUNC_STAT
AC_CHECK_FUNCS([memmove setlocale strchr strdup strerror strpbrk])
AC_REPLACE_FUNCS([getopt])

case $host_os in
    *mingw* )  AM_CONDITIONAL(ADDPCREPOSIX, true) ;;
    *darwin* ) AM_CONDITIONAL(ADDPCREPOSIX, false)
               AC_DEFINE([DARWIN], [1], [MacOS/DARWIN system]) ;;
    * )        AM_CONDITIONAL(ADDPCREPOSIX, false) ;;
esac

# Pcre
if test -f pcre/configure; then
	AC_CONFIG_SUBDIRS([pcre])
	AC_SUBST([PCRE_SUBDIRS],[pcre])
	AC_SUBST([PCRE_LDFLAG],[['$(top_builddir)'/pcre/libpcre.la]])
	CPPFLAGS="$CPPFLAGS -I\$(top_srcdir)/pcre"
else
	AC_SUBST([PCRE_LDFLAG],[-lpcre])
fi
AC_MSG_NOTICE([PCRE from $PCRE_LDFLAG])

# FinLib 
if test -f finlib2/configure; then
	AC_CONFIG_SUBDIRS([finlib2])
	AC_SUBST([FINLIB_SUBDIRS],[finlib2])
	AC_SUBST([FINLIB_LDFLAG],[['$(top_builddir)'/finlib2/finlib/libfin.la]])
	CPPFLAGS="$CPPFLAGS -I\$(top_srcdir)/finlib2"
else if test -f ../finlib2/finlib/libfin.la; then
	AC_SUBST([FINLIB_LDFLAG],[$(top_srcdir)/../finlib2/finlib/libfin.la])
	CPPFLAGS="$CPPFLAGS -I../../finlib2"
else
	AC_SUBST([FINLIB_LDFLAG],[-lfin])
fi
fi
AC_MSG_NOTICE([finlib from $FINLIB_LDFLAG])

# Antlr
if test -f antlr/Makefile.am; then
	AC_CONFIG_FILES([antlr/Makefile antlr/src/Makefile antlr/antlr/Makefile])
	AC_SUBST([ANTLR_SUBDIRS],[antlr])
	AC_PATH_PROG(ANTLR_BIN, antlr3, antlr3)
	AC_SUBST([ANTLR_LDFLAG],[['$(top_builddir)'/antlr/src/libantlr3c.la]])
	CPPFLAGS="$CPPFLAGS -I\$(top_srcdir)/antlr"
else
	# XXX AC-CONFIG-FILES is evaluated always, we need AM-COND-IF in automake 1.11 (released May 2009),
	# then we can remove creating Makefile.in's in antlr/ subdir from the bootstrap script
	AC_SUBST([ANTLR_LDFLAG],[-lantlr3c])
	AC_PATH_PROG(ANTLR_BIN, antlr3, antlr3)
	AX_ABSOLUTE_HEADER([antlr3config.h])
	ANTLR_VERSION=`grep PACKAGE_VERSION $gl_cv_absolute_antlr3config_h | sed 's#.*PACKAGE_VERSION *"\([^"]\+\)".*#\1#'`
	AX_COMPARE_VERSION([$ANTLR_VERSION],[ge],[3.2],[AC_MSG_NOTICE([Found Antlr version $ANTLR_VERSION])],[AC_MSG_ERROR([Need Antlr version >= 3.2])])
	AX_COMPARE_VERSION([$ANTLR_VERSION],[ge],[3.4],[AC_DEFINE([ANTLR_34_API], [1], [Uses antlr >= 3.4 API])])
fi
AC_MSG_NOTICE([Antlr from $ANTLR_LDFLAG])

# Swig
AC_PATH_PROGS(SWIG, [swig-1.3 swig])

# Python
AC_ARG_ENABLE(python, [  --disable-python    Disable the python API.],,
		    enable_python="yes")
if test "x$enable_python" = "xyes" ; then
    AM_PATH_PYTHON
    if test -z "$PYTHON_CPPFLAGS" ; then
	AC_MSG_CHECKING([for Python include path])
	python_path=${PYTHON%/bin*}
	for i in "$python_path/include/python$PYTHON_VERSION" "$python_path/include/python" "$python_path/" ; do
	    if test -f "$i/Python.h"; then
	        AC_SUBST([PYTHON_CPPFLAGS],[-I$i])
		AC_MSG_RESULT([$i])
		break
	    fi        
	 done
    fi
    saved_CPPFLAGS=$CPPFLAGS
    CPPFLAGS="$CPPFLAGS $PYTHON_CPPFLAGS"
    AC_CHECK_HEADER(Python.h, [enable_python=yes], [
        enable_python=no
	AC_MSG_WARN([cannot find Python include path, set PYTHON_CPPFLAGS])])
    CPPFLAGS=$saved_CPPFLAGS
fi
AM_CONDITIONAL([PYTHON_API_ENABLED], [test "x$enable_python" = "xyes"])

# Java
AC_ARG_ENABLE(java, [  --disable-java      Disable the java API.],,
		    enable_java="yes")
if test "x$enable_java" = "xyes"; then
    if test -z "$JAVA_CPPFLAGS" ; then
        AC_JNI_INCLUDE_DIR
    fi
    saved_CPPFLAGS=$CPPFLAGS
    CPPFLAGS="$CPPFLAGS $JAVA_CPPFLAGS"
    AC_CHECK_HEADER(jni.h, [enable_java=yes], [
        enable_java=no
	AC_MSG_WARN([cannot find Java include path, set JAVA_CPPFLAGS])])
    CPPFLAGS=$saved_CPPFLAGS

fi
AM_CONDITIONAL([JAVA_API_ENABLED], [test "x$enable_java" = "xyes"])

# Ruby
AC_ARG_ENABLE(ruby, [  --disable-ruby      Disable the ruby API.],,
		    enable_ruby="yes")
if test "x$enable_ruby" = "xyes"; then
	# locate ruby binary, CPPFLAGS and site directory
	AX_RUBY_DEVEL()

    saved_CPPFLAGS=$CPPFLAGS
    CPPFLAGS="$CPPFLAGS $RUBY_CPPFLAGS"
	AC_CHECK_HEADER(ruby.h, [enable_ruby=yes], [
    enable_ruby=no
	AC_MSG_WARN([cannot find Ruby include path, set RUBY_CPPFLAGS])])
    CPPFLAGS=$saved_CPPFLAGS
fi
AM_CONDITIONAL([RUBY_API_ENABLED], [test "x$enable_ruby" = "xyes"])

# Perl
AC_ARG_ENABLE(perl, [  --disable-perl      Disable the perl API.],,
		    enable_perl="yes")
if test "x$enable_perl" = "xyes"; then
	# locate perl binary
	AX_WITH_PERL

	if test -z "$PERL_CPPFLAGS" ; then
		# locate header directory
	    PERL_INCLUDE_DIR=`$PERL -MConfig -e 'print "$Config{archlib}/CORE"'`
	    PERL_CPPFLAGS="-I$PERL_INCLUDE_DIR"
	fi

	# locate site directory for local modules
	case $prefix in
	NONE|/usr/local|/usr/local/)
		AC_SUBST([PERL_SITE_PKG],[[`$PERL -MConfig -e 'print "$Config{installsitearch}"'`]])
		;;
	*)
		AC_SUBST([PERL_SITE_PKG],[[`$PERL -MConfig -e 'print "$Config{installvendorarch}"' | sed "s,/usr/,$prefix/,"`]])
		;;
	esac

	AC_SUBST(PERL_CPPFLAGS)
    saved_CPPFLAGS=$CPPFLAGS
    CPPFLAGS="$CPPFLAGS $PERL_CPPFLAGS"
    AC_CHECK_HEADER(perl.h,[enable_perl=yes], [enable_perl=no
      AC_MSG_WARN([cannot find Perl include path, set PERL_CPPFLAGS])],
      [#include "EXTERN.h"
      ])
    CPPFLAGS=$saved_CPPFLAGS
fi
AM_CONDITIONAL([PERL_API_ENABLED], [test "x$enable_perl" = "xyes"])


# local development with bzr
AM_CONDITIONAL(LOCALBZR, [test -d .bzr || test -d ../.git])

# MANATEE_REGISTRY environment variable
AC_ARG_VAR(MANATEE_REGISTRY, [default MANATEE_REGISTRY path])
AM_CONDITIONAL(MANATEE_REGISTRY_STR, test -n "$MANATEE_REGISTRY")

regexplib=pcre
# --with-icu
AC_ARG_WITH(icu,
[  --with-icu          Use ICU locales [[default no]]],
[
  if test $with_icu = yes; then
     AC_DEFINE(USE_ICU, 1, [Use ICU locales instead of Unix ones (setlocale)])
     AC_MSG_NOTICE([selected ICU locales instead of Unix ones (setlocale)])
     regexplib=icu
  fi
])


if test $regexplib = icu; then
     AC_MSG_NOTICE([selected ICU regexps])
     AC_SUBST([REGEX_LDFLAG], [['-licuuc -licui18n']])
else
     AC_SUBST([REGEX_LDFLAG], [$PCRE_LDFLAG])
fi
AC_MSG_NOTICE([regexp from $REGEX_LDFLAG])


AC_DEFINE_UNQUOTED(MANATEE_VERSION_STR, ["$PACKAGE_VERSION"], [Manatee version number])
AC_DEFINE_UNQUOTED(MANATEE_VERSION_STR, ["$PACKAGE_VERSION"], [Manatee version number])

AC_ARG_ENABLE(cxxlib,
[  --enable-cxxlib         install C++ library],
[
   case "${enableval}" in
       yes) cxxlib=true 
	AC_DEFINE(MANATEE_CXXLIB, 1, [install C++ library])
	AC_MSG_NOTICE([installation will include C++ library])
	;;
       no)  cxxlib=false ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-cxxlib) ;;
     esac
],[cxxlib=false])
AM_CONDITIONAL(MANATEE_CXXLIB, test x$cxxlib = xtrue)


AC_CONFIG_FILES(Makefile src/Makefile corp/Makefile concord/Makefile query/Makefile api/Makefile config/dpkg-control)

AC_OUTPUT
