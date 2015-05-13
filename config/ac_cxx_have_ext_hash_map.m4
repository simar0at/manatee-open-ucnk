dnl @synopsis AC_CXX_HAVE_EXT_HASH_MAP
dnl
dnl Check if the compiler has ext/hash_map Eg:
dnl
dnl   #if defined HAVE_EXT_HASH_MAP
dnl   #include <ext/hash_map>
dnl   #   if defined HAVE_GNU_CXX_NS
dnl           namespace std {using namespace __gnu_cxx;}
dnl   #   fi
dnl   #else
dnl   #include <hash_map>
dnl   #endif
dnl
dnl This file is Alain BARBET's AC_CXX_HAVE_EXT_HASH_SET 1.1 with
dnl s/set/map/g :)
dnl
dnl @category Cxx
dnl @author Perceval ANICHINI <perceval.anichini@epita.fr>, Pavel Rychly <pary@fi.muni.cz/>
dnl @version 2005-06-23
dnl @license GPLWithACException

AC_DEFUN([AC_CXX_HAVE_EXT_HASH_MAP],[

AC_CACHE_CHECK([if the __gnu_cxx namespace exists],
	ac_cv_gnuextns,[
	ac_cv_gnuextns=no
  	AC_LANG_SAVE
  	AC_LANG_CPLUSPLUS
	AC_TRY_COMPILE([#include <ext/hash_map>], [using namespace __gnu_cxx;],
		ac_cv_gnuextns=yes)
	AC_TRY_COMPILE([#include <hash_map>], [using namespace __gnu_cxx;],
		ac_cv_gnuextns=yes)
	AC_LANG_RESTORE
	])
if test "x$ac_cv_gnuextns" = xyes; then
	AC_DEFINE(HAVE_GNU_CXX_NS,1,[Define to 1 if g++ supports __gnu_cxx namespace])
fi


AC_CACHE_CHECK([whether the compiler has ext/hash_map],
ac_cv_cxx_have_ext_hash_map,
[ AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <ext/hash_map>
#ifdef HAVE_GNU_CXX_NS
using namespace __gnu_cxx;
#else
using naemsapce std;
#endif],[hash_map<int, int> t; return 0;],
  ac_cv_cxx_have_ext_hash_map=yes, ac_cv_cxx_have_ext_hash_map=no)
  AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_ext_hash_map" = yes; then
   AC_DEFINE(HAVE_EXT_HASH_MAP,,[define if the compiler has ext/hash_map])
fi
])
