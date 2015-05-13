# Copyright (c) 2003-2008  Pavel Rychly

ANTLR_GENERATED = cqpLexer.hpp cqpLexer.cpp cqpParser.hpp cqpParser.cpp cqp.tokens cqpTreeWalker.hpp cqpTreeWalker.cpp

noinst_LTLIBRARIES = libquery.la
noinst_PROGRAMS = tstcqpgr tstcqpgr2
libquery_la_SOURCES = cqpeval.cc frconcat.cc frstream.cc parsop.cc \
	frsop.hh parsop.hh $(ANTLR_GENERATED)


query_headers = cqpeval.hh

if MANATEE_CXXLIB
pkginclude_HEADERS = $(query_headers)
else
libquery_la_SOURCES += $(query_headers)
endif


MAINTAINERCLEANFILES = 	$(ANTLR_GENERATED)

cqpeval.lo: $(ANTLR_GENERATED)

test-grammar:
	./tstquery.sh

AM_CPPFLAGS = -I$(top_srcdir)/corp -I$(top_builddir)


ANTLROPTS = -make -verbose
# '-make' remakes the grammar only if it is newer than the generated files
# this makes it possible to parallelize the build (otherwise make gets into trouble
# when multiple subgoals induce a command making multiple goals).

tstcqpgr_SOURCES = tstcqpgr.cc
tstcqpgr_CPPFGLAGS = 
tstcqpgr_LDADD = libquery.la $(top_builddir)/corp/libcorp.la \
	$(FINLIB_LDFLAG) $(ANTLR_LDFLAG) $(REGEX_LDFLAG)

tstcqpgr2_SOURCES = tstcqpgr2.cc
tstcqpgr2_CPPFGLAGS = 
tstcqpgr2_LDADD = libquery.la $(top_builddir)/corp/libcorp.la \
	$(FINLIB_LDFLAG) $(ANTLR_LDFLAG) $(REGEX_LDFLAG)

if LOCALBZR
cqpLexer.hpp cqpLexer.cpp cqpParser.hpp cqp.tokens cqpParser.cpp cqpTreeWalker.hpp cqpTreeWalker.cpp cqpTreeWalker.tokens: grammars

grammars: 
	$(ANTLR_BIN) $(ANTLROPTS) cqp.g cqpTreeWalker.g | tee antlr.out
	@if [ `grep -c skipped antlr.out` -ne 2 ]; then \
		for cppsource in cqp*.c; do ln -sf $$cppsource `basename $$cppsource .c`.cpp; done ; \
		for cppheader in cqp*.h; do awk '/#ifdef __cplusplus/ {skip=3};!skip {print};skip {skip=skip-1};' $$cppheader > `basename $$cppheader .h`.hpp; done ; \
		sed -i 's/"cqpLexer.h"/"cqpLexer.hpp"/' cqpLexer.cpp ; \
		sed -i 's/"cqpParser.h"/"cqpParser.hpp"/' cqpParser.cpp ; \
		sed -i 's/"cqpTreeWalker.h"/"cqpTreeWalker.hpp"/' cqpTreeWalker.cpp ; \
	fi
endif

.PHONY: grammars test-grammar
