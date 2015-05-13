# Copyright (c) 2003-2013  Pavel Rychly

ANTLR_GENERATED = corpconfLexer.hpp corpconfLexer.cpp corpconfParser.hpp corpconfParser.cpp corpconf.tokens

noinst_LTLIBRARIES = libcorp.la
libcorp_la_SOURCES = corpconf.cc corpus.cc dynattr.cc dynfun.cc \
	loadconf.cc posattr.cc struct.cc subcorp.cc bgrstat.cc calign.cc \
	$(ANTLR_GENERATED) dynattr.hh dynfun.hh virtcorp.cc utf8.cc \
	pauniq.cc pauniq.hh levels.cc virtstruc.cc

corp_headers = corpus.hh corpconf.hh posattr.hh bgrstat.hh subcorp.hh \
	virtcorp.hh utf8.hh excep.hh levels.hh

if MANATEE_CXXLIB
pkginclude_HEADERS = $(corp_headers)
else
libcorp_la_SOURCES += $(corp_headers)
endif


libcorp_la_LIBADD = $(LIBLTDL)

EXTRA_DIST = stddynfun.c langcodes.cc

AM_CPPFLAGS = -I$(top_srcdir)/query $(INCLTDL)

if MANATEE_REGISTRY_STR
AM_CPPFLAGS += -DMANATEE_REGISTRY_STR='"@MANATEE_REGISTRY@"'
endif


MAINTAINERCLEANFILES = $(ANTLR_GENERATED)

loadconf.lo: $(ANTLR_GENERATED)

if LOCALBZR

corpconf.tokens corpconfLexer.hpp corpconfLexer.cpp corpconfParser.hpp: corpconfParser.cpp
corpconfParser.cpp: corpconf.g
	$(ANTLR_BIN) $(ANTLROPTS) corpconf.g
	for cppsource in corpconf*.c; do mv $$cppsource `basename $$cppsource .c`.cpp; done
	for cppheader in corpconf*.h; do awk '/#ifdef __cplusplus/ {skip=3};!skip {print};skip {skip=skip-1};' $$cppheader > `basename $$cppheader .h`.hpp; rm $$cppheader; done
	sed -i 's/"corpconfLexer.h"/"corpconfLexer.hpp"/' corpconfLexer.cpp
	sed -i 's/"corpconfParser.h"/"corpconfParser.hpp"/' corpconfParser.cpp
endif
