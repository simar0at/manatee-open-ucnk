# Copyright (c) 2003-2014  Pavel Rychly, Milos Jakubicek

ANTLR_CORPCONF_GENERATED = corpconfLexer.hh corpconfLexer.cc corpconfParser.hh corpconfParser.cc corpconf.tokens
ANTLR_REGEXOPT_GENERATED = regexoptLexer.hh regexoptLexer.cc regexoptParser.hh regexoptParser.cc regexopt.tokens regexoptTreeWalker.cc regexoptTreeWalker.hh

noinst_LTLIBRARIES = libcorp.la libantlrgen.la
libcorp_la_SOURCES = corpconf.cc corpus.cc dynattr.cc dynfun.cc \
	loadconf.cc posattr.cc struct.cc subcorp.cc bgrstat.cc calign.cc \
	dynattr.hh dynfun.hh virtcorp.cc utf8.cc corpstat.cc \
	pauniq.cc pauniq.hh levels.cc virtstruc.cc regexopt.cc regexopt.hh
libcorp_la_LIBADD = $(LIBLTDL) libantlrgen.la
libantlrgen_la_SOURCES = $(ANTLR_CORPCONF_GENERATED) $(ANTLR_REGEXOPT_GENERATED)
libantlrgen_la_CPPFLAGS = $(AM_CPPFLAGS) -Wno-unused-but-set-variable \
                          -Wno-unused-variable -Wno-unused-function
corp_headers = corpus.hh corpconf.hh posattr.hh bgrstat.hh subcorp.hh \
	virtcorp.hh utf8.hh excep.hh levels.hh

if MANATEE_CXXLIB
pkginclude_HEADERS = $(corp_headers)
else
libcorp_la_SOURCES += $(corp_headers)
endif

EXTRA_DIST = stddynfun.c langcodes.cc corpconf.g regexopt.g regexoptTreeWalker.g

AM_CPPFLAGS += -I$(top_srcdir)/query -I$(top_srcdir)/stat $(INCLTDL)

if MANATEE_REGISTRY_STR
AM_CPPFLAGS += -DMANATEE_REGISTRY_STR='"@MANATEE_REGISTRY@"'
endif


MAINTAINERCLEANFILES = $(ANTLR_CORPCONF_GENERATED) $(ANTLR_REGEXOPT_GENERATED)

loadconf.lo: $(ANTLR_CORPCONF_GENERATED)
regexopt.lo: $(ANTLR_REGEXOPT_GENERATED)

corpconf.tokens: corpconf.g
	$(ANTLR_BIN) -verbose corpconf.g
	for cppsource in corpconf*.c; do mv $$cppsource `basename $$cppsource .c`.cc; done ; \
	for cppheader in corpconf*.h; do awk '/#ifdef __cplusplus/ {skip=3};!skip {print};skip {skip=skip-1};' $$cppheader > `basename $$cppheader .h`.hh; rm $$cppheader; done ; \
	sed -i 's/"corpconfLexer.h"/"corpconfLexer.hh"/' corpconfLexer.cc
	sed -i 's/"corpconfParser.h"/"corpconfParser.hh"/' corpconfParser.cc

regexopt.tokens: regexopt.g regexoptTreeWalker.g
	$(ANTLR_BIN) -verbose regexopt.g regexoptTreeWalker.g
	for cppsource in regexopt*.c; do mv $$cppsource `basename $$cppsource .c`.cc; done ; \
	for cppheader in regexopt*.h; do awk '/#ifdef __cplusplus/ {skip=3};!skip {print};skip {skip=skip-1};' $$cppheader > `basename $$cppheader .h`.hh; rm $$cppheader; done ; \
	sed -i 's/"regexoptLexer.h"/"regexoptLexer.hh"/' regexoptLexer.cc ; \
	sed -i 's/"regexoptParser.h"/"regexoptParser.hh"/' regexoptParser.cc ; \
	sed -i 's/"regexoptTreeWalker.h"/"regexoptTreeWalker.hh"/' regexoptTreeWalker.cc ;


# fix parallel build:
$(filter-out corpconf.tokens,$(ANTLR_CORPCONF_GENERATED)): corpconf.tokens
	@if test -f $@; then \
	  touch $@; \
	else \
## Recover from the removal of $@
	rm -f corpconf.tokens; \
	$(MAKE) $(AM_MAKEFLAGS) corpconf.tokens; \
	fi
$(filter-out regexopt.tokens,$(ANTLR_REGEXOPT_GENERATED)): regexopt.tokens
	@if test -f $@; then \
	  touch $@; \
	else \
## Recover from the removal of $@
	rm -f regexopt.tokens; \
	$(MAKE) $(AM_MAKEFLAGS) regexopt.tokens; \
	fi
