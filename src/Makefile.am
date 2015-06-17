# Copyright (c) 2003-2015  Pavel Rychly, Milos Jakubicek

bin_PROGRAMS = manateesrv corpinfo encodevert mkdynattr lscbgr lsclex mknorms \
	mkvirt mkalign dumpattrtext dumpattrrev dumpstructrng mkregexattr lsalsize
noinst_PROGRAMS = tstcqpgr tstcqpgr2 dumplevel dumpvirt tstregexopt
corpinfo_SOURCES = corpinfo.cc
encodevert_SOURCES = encodevert.cc
mkdynattr_SOURCES = mkdynattr.cc
mkregexattr_SOURCES = mkregexattr.cc
lsalsize_SOURCES = lsalsize.cc
lscbgr_SOURCES = lscbgr.cc
lsclex_SOURCES = lsclex.cc
manateesrv_SOURCES = manateesrv.cc
mknorms_SOURCES = mknorms.cc
mkvirt_SOURCES = mkvirt.cc
mkalign_SOURCES = mkalign.cc
dumpattrtext_SOURCES = dumpattrtext.cc
dumpattrrev_SOURCES = dumpattrrev.cc
dumpstructrng_SOURCES = dumpstructrng.cc
dumplevel_SOURCES = dumplevel.cc
dumplevel_LDADD = $(top_builddir)/corp/libcorp.la
dumpvirt_SOURCES = dumpvirt.cc
dumpvirt_LDADD = $(top_builddir)/corp/libcorp.la $(ANTLR_LDFLAG) \
	$(FINLIB_LDFLAG) $(top_builddir)/query/libquery.la

lib_LTLIBRARIES = libmanatee.la
libmanatee_la_SOURCES = 
libmanatee_la_LIBADD = $(top_builddir)/concord/libconcord.la \
	$(top_builddir)/query/libquery.la $(top_builddir)/corp/libcorp.la \
	$(FINLIB_LDFLAG) $(ANTLR_LDFLAG) $(REGEX_LDFLAG)

noinst_LTLIBRARIES = libbuiltinmanatee.la
libbuiltinmanatee_la_SOURCES = 
libbuiltinmanatee_la_LIBADD = $(top_builddir)/concord/libconcord.la \
	$(top_builddir)/query/libquery.la $(top_builddir)/corp/libcorp.la

AM_CPPFLAGS += -I$(top_srcdir)/corp -I$(top_srcdir)/query \
	-I$(top_srcdir)/concord

LDADD = libbuiltinmanatee.la $(FINLIB_LDFLAG) $(ANTLR_LDFLAG) $(REGEX_LDFLAG)

manateesrv_LDADD = $(LDADD) $(PTHREAD_LIBS)
manateesrv_LDFLAGS = $(LDFLAGS) 

tstcqpgr_SOURCES = tstcqpgr.cc
tstcqpgr_LDADD = $(top_builddir)/query/libquery.la $(top_builddir)/corp/libcorp.la \
	$(FINLIB_LDFLAG) $(ANTLR_LDFLAG) $(REGEX_LDFLAG)

tstcqpgr2_SOURCES = tstcqpgr2.cc
tstcqpgr2_LDADD = $(top_builddir)/query/libquery.la $(top_builddir)/corp/libcorp.la \
	$(FINLIB_LDFLAG) $(ANTLR_LDFLAG) $(REGEX_LDFLAG)

tstregexopt_SOURCES = tstregexopt.cc
tstregexopt_LDADD = $(top_builddir)/corp/libcorp.la $(top_builddir)/query/libquery.la \
	$(FINLIB_LDFLAG) $(ANTLR_LDFLAG) $(REGEX_LDFLAG)


if ADDPCREPOSIX
encodevert_LDADD = $(LDADD) -lpcreposix $(PCRE_LDFLAG)
else
encodevert_LDADD = $(LDADD)
endif
