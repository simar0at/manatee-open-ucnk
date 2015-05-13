// Copyright (c) 2001-2008  Pavel Rychly

#ifndef CQPEVAL_HH
#define CQPEVAL_HH

#include <finlib/frstream.hh>
#include <antlr3.h>
#include "corpus.hh"

void throwEvalQueryException (pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames);
void unescapeString (pANTLR3_COMMON_TOKEN token);
void rangeRegexp(pANTLR3_COMMON_TOKEN token, int order);

class EvalQueryException: public std::exception {
    const std::string _what;
public:
    EvalQueryException (const std::string &name)
	:_what ("Query evaluation error" + name) {}
    virtual const char* what () const throw () {return _what.c_str();}
    virtual ~EvalQueryException() throw() {}
};

RangeStream *eval_cqpquery (const char *query, Corpus *corp);
FastStream *eval_cqponepos (const char *query, Corpus *corp);


#endif
