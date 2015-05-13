dnl @synopsis AC_JNI_INCLUDE_DIR
dnl
dnl AC_JNI_INCLUDE_DIR finds include directories needed for compiling
dnl programs using the JNI interface.
dnl
dnl JNI include directories are usually in the java distribution This
dnl is deduced from the value of JAVAC. When this macro completes, a
dnl JAVA_CPPFLAGS is left with CPPFLAGS for compiling java modules.
dnl
dnl Example automake usage follows:
dnl
dnl     libjavamodule_la_CPPFLAGS = $(AM_CPPFLAGS) $(JAVA_CPPFLAGS)
dnl
dnl If you want to force a specific compiler:
dnl
dnl - at the configure.in level, set JAVAC=yourcompiler before calling
dnl AC_JNI_INCLUDE_DIR
dnl
dnl - at the configure level, setenv JAVAC
dnl
dnl Note: This macro can work with the autoconf M4 macros for Java
dnl programs. This particular macro is not part of the original set of
dnl macros.
dnl
dnl @category InstalledPackages
dnl @author Don Anderson <dda@sleepycat.com>, Pavel Rychly <pary@fi.muni.cz>
dnl @version 2005-06-22
dnl @license AllPermissive

AC_DEFUN([AC_JNI_INCLUDE_DIR],[

JAVA_CPPFLAGS=""

if test "x$JAVAC" = x ; then
	JAVAC=javac
fi
AC_PATH_PROG(_ACJNI_JAVAC, $JAVAC, no)
if test "x$_ACJNI_JAVAC" = xno ; then
   AC_MSG_WARN([$JAVAC could not be found in path, set JAVAC])
else
_ACJNI_FOLLOW_SYMLINKS("$_ACJNI_JAVAC")
_JTOPDIR=`echo "$_ACJNI_FOLLOWED" | sed -e 's://*:/:g' -e 's:/[[^/]]*$::'`
case "$host_os" in
        darwin*)        _JTOPDIR=`echo "$_JTOPDIR" | sed -e 's:/[[^/]]*$::'`
                        _JINC="$_JTOPDIR/Headers";;
        *)              _JINC="$_JTOPDIR/include";;
esac
if test -f "$_JINC/jni.h"; then
        JAVA_CPPFLAGS="$JAVA_CPPFLAGS -I$_JINC"
else
        _JTOPDIR=`echo "$_JTOPDIR" | sed -e 's:/[[^/]]*$::'`
        if test -f "$_JTOPDIR/include/jni.h"; then
                JAVA_CPPFLAGS="$JAVA_CPPFLAGS -I$_JTOPDIR/include"
        fi
fi

# get the likely subdirectories for system specific java includes
case "$host_os" in
bsdi*)          _JNI_INC_SUBDIRS="bsdos";;
linux*)         _JNI_INC_SUBDIRS="linux genunix";;
osf*)           _JNI_INC_SUBDIRS="alpha";;
solaris*)       _JNI_INC_SUBDIRS="solaris";;
mingw*)			_JNI_INC_SUBDIRS="win32";;
cygwin*)		_JNI_INC_SUBDIRS="win32";;
*)              _JNI_INC_SUBDIRS="genunix";;
esac

# add any subdirectories that are present
for JINCSUBDIR in $_JNI_INC_SUBDIRS
do
        if test "$_JTOPDIR" != "/usr" -a -d "$_JTOPDIR/include/$JINCSUBDIR"; 
	then
                JAVA_CPPFLAGS="$JAVA_CPPFLAGS -I$_JTOPDIR/include/$JINCSUBDIR"
        fi
done
fi
AC_SUBST(JAVA_CPPFLAGS)
])

# _ACJNI_FOLLOW_SYMLINKS <path>
# Follows symbolic links on <path>,
# finally setting variable _ACJNI_FOLLOWED
# --------------------
AC_DEFUN([_ACJNI_FOLLOW_SYMLINKS],[
# find the include directory relative to the javac executable
_cur="$1"
while ls -ld "$_cur" 2>/dev/null | grep " -> " >/dev/null; do
        AC_MSG_CHECKING(symlink for $_cur)
        _slink=`ls -ld "$_cur" | sed 's/.* -> //'`
        case "$_slink" in
        /*) _cur="$_slink";;
        # 'X' avoids triggering unwanted echo options.
        *) _cur=`echo "X$_cur" | sed -e 's/^X//' -e 's:[[^/]]*$::'`"$_slink";;
        esac
        AC_MSG_RESULT($_cur)
done
_ACJNI_FOLLOWED="$_cur"
])# _ACJNI
