# Copyright (c) 2003-2012  Pavel Rychly

bin_PROGRAMS = manateesrv corpinfo encodevert mkdynattr lscbgr lsclex mknorms \
	mkvirt mkalign
corpinfo_SOURCES = corpinfo.cc
encodevert_SOURCES = encodevert.cc
mkdynattr_SOURCES = mkdynattr.cc
lscbgr_SOURCES = lscbgr.cc
lsclex_SOURCES = lsclex.cc
manateesrv_SOURCES = manateesrv.cc
mknorms_SOURCES = mknorms.cc
mkvirt_SOURCES = mkvirt.cc
mkalign_SOURCES = mkalign.cc

lib_LTLIBRARIES = libmanatee.la
libmanatee_la_SOURCES = 
libmanatee_la_LIBADD = $(top_builddir)/concord/libconcord.la \
	$(top_builddir)/query/libquery.la $(top_builddir)/corp/libcorp.la \
	$(FINLIB_LDFLAG) $(ANTLR_LDFLAG) $(REGEX_LDFLAG)


noinst_LTLIBRARIES = libbuiltinmanatee.la
libbuiltinmanatee_la_SOURCES = 
libbuiltinmanatee_la_LIBADD = $(top_builddir)/concord/libconcord.la \
	$(top_builddir)/query/libquery.la $(top_builddir)/corp/libcorp.la


AM_CPPFLAGS = -I$(top_srcdir)/corp -I$(top_srcdir)/query \
	-I$(top_srcdir)/concord

LDADD = libbuiltinmanatee.la $(FINLIB_LDFLAG) $(ANTLR_LDFLAG) $(REGEX_LDFLAG)

manateesrv_LDADD = $(LDADD) $(PTHREAD_LIBS)
manateesrv_LDFLAGS = $(LDFLAGS) 

manateesrv_CPPFLAGS = $(AM_CPPFLAGS)


if ADDPCREPOSIX
encodevert_LDADD = $(LDADD) -lpcreposix $(PCRE_LDFLAG)
else
encodevert_LDADD = $(LDADD)
endif
