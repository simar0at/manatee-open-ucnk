# Copyright (c) 2003-2008  Pavel Rychly

pkglib_LTLIBRARIES =

dist_bin_SCRIPTS = addwcattr  concinfo  corpquery  decodevert  freqs  lslexarf  lsslex  mksubc  compilecorp

if PYTHON_API_ENABLED
pyexec_LTLIBRARIES = _manatee.la
dist_pyexec_PYTHON = manatee.py
endif
if JAVA_API_ENABLED
pkglib_LTLIBRARIES += libmanatee_jni.la
endif
if RUBY_API_ENABLED
rubysitepkgdir=@RUBY_SITE_PKG@
rubysitepkg_LTLIBRARIES = manatee_ruby.la
endif
if PERL_API_ENABLED
perlsitepkgdir=@PERL_SITE_PKG@
perlsitepkg_LTLIBRARIES = manatee.la
dist_perlsitepkg_DATA = manatee.pm
endif

_manatee_la_SOURCES = manatee_py_wrap.cc
_manatee_la_LDFLAGS = -module -avoid-version -no-undefined $(PTHREAD_LIBS)

manatee_la_SOURCES = manatee_pe_wrap.cc
manatee_la_LDFLAGS = -module -avoid-version -no-undefined $(PTHREAD_LIBS)

manatee_ruby_la_SOURCES = manatee_rb_wrap.cc
manatee_ruby_la_LDFLAGS = -module -avoid-version -no-undefined $(PTHREAD_LIBS)

libmanatee_jni_la_SOURCES = manatee_java_wrap.cc
libmanatee_jni_la_LDFLAGS = -module -avoid-version -no-undefined $(PTHREAD_LIBS)

if LOCALBZR
SWIGAPI = manatee.i common.i
manatee_py_wrap.cc manatee.py: $(SWIGAPI)
	$(SWIG) -python -c++ -shadow -o $@ $<

manatee_pe_wrap.cc manatee.pm: $(SWIGAPI)
	$(SWIG) -perl  -c++ -shadow -o $@ $<

manatee_rb_wrap.cc: $(SWIGAPI)
	$(SWIG) -ruby  -c++ -o $@ $<

JAVA_PACKAGE_NAME=com.sketchengine.manatee

manatee_java_wrap.cc: $(SWIGAPI)
	mkdir -p java
	$(SWIG) -java -c++ -package $(JAVA_PACKAGE_NAME)  -o $@ -outdir java $<
endif

EXTRA_DIST = corpquery.pl java example.java

NEEDED_LIBS =  $(FINLIB_LDFLAG) $(ANTLR_LDFLAG) $(REGEX_LDFLAG)

_manatee_la_CPPFLAGS = $(AM_CPPFLAGS) $(PYTHON_CPPFLAGS) \
	-I$(top_srcdir)/corp -I$(top_srcdir)/query \
	-I$(top_srcdir)/concord -I$(top_srcdir)/stat
_manatee_la_LIBADD = ../src/libbuiltinmanatee.la $(NEEDED_LIBS)

manatee_la_CPPFLAGS = $(AM_CPPFLAGS) $(PERL_CPPFLAGS) \
	-I$(top_srcdir)/corp -I$(top_srcdir)/query \
	-I$(top_srcdir)/concord -I$(top_srcdir)/stat
manatee_la_LIBADD = ../src/libbuiltinmanatee.la $(NEEDED_LIBS)

manatee_ruby_la_CPPFLAGS = $(AM_CPPFLAGS) $(RUBY_CPPFLAGS) \
	-I$(top_srcdir)/corp -I$(top_srcdir)/query \
	-I$(top_srcdir)/concord -I$(top_srcdir)/stat
manatee_ruby_la_LIBADD = ../src/libbuiltinmanatee.la $(NEEDED_LIBS)

libmanatee_jni_la_CPPFLAGS = $(AM_CPPFLAGS) $(JAVA_CPPFLAGS) \
	-I$(top_srcdir)/corp -I$(top_srcdir)/query \
	-I$(top_srcdir)/concord -I$(top_srcdir)/stat
libmanatee_jni_la_LIBADD = ../src/libbuiltinmanatee.la $(NEEDED_LIBS)
