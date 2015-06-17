# Copyright (c) 2003-2014  Pavel Rychly, Milos Jakubicek

AM_CPPFLAGS += -I$(top_srcdir)/corp -I$(top_builddir)
ANTLR_GENERATED = cqpLexer.hh cqpLexer.cc cqpParser.hh cqpParser.cc cqp.tokens cqpTreeWalker.hh cqpTreeWalker.cc

noinst_LTLIBRARIES = libquery.la libantlrgen.la
libquery_la_SOURCES = cqpeval.cc frconcat.cc frstream.cc parsop.cc \
	frsop.hh parsop.hh
libquery_la_LIBADD = libantlrgen.la
libantlrgen_la_SOURCES = $(ANTLR_GENERATED)
libantlrgen_la_CPPFLAGS = $(AM_CPPFLAGS) -Wno-unused-but-set-variable \
                          -Wno-unused-variable -Wno-unused-function

query_headers = cqpeval.hh

if MANATEE_CXXLIB
pkginclude_HEADERS = $(query_headers)
else
libquery_la_SOURCES += $(query_headers)
endif

EXTRA_DIST = cqp.g cqpTreeWalker.g
MAINTAINERCLEANFILES = 	$(ANTLR_GENERATED)

cqpeval.lo: $(ANTLR_GENERATED)

test-grammar:
	./tstquery.sh

cqp.tokens: cqp.g cqpTreeWalker.g
	$(ANTLR_BIN) -verbose cqp.g cqpTreeWalker.g
	for cppsource in cqp*.c; do mv $$cppsource `basename $$cppsource .c`.cc; done ; \
	for cppheader in cqp*.h; do awk '/#ifdef __cplusplus/ {skip=3};!skip {print};skip {skip=skip-1};' $$cppheader > `basename $$cppheader .h`.hh; rm $$cppheader; done ; \
	sed -i 's/"cqpLexer.h"/"cqpLexer.hh"/' cqpLexer.cc ; \
	sed -i 's/"cqpParser.h"/"cqpParser.hh"/' cqpParser.cc ; \
	sed -i 's/"cqpTreeWalker.h"/"cqpTreeWalker.hh"/' cqpTreeWalker.cc ;

# fix parallel build:
$(filter-out cqp.tokens,$(ANTLR_GENERATED)): cqp.tokens
	@if test -f $@; then \
	  touch $@; \
	else \
## Recover from the removal of $@
	rm -f cqp.tokens; \
	$(MAKE) $(AM_MAKEFLAGS) cqp.tokens; \
	fi

.PHONY: test-grammar
