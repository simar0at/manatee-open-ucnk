/** \file
 *  This C source file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : regexoptTreeWalker.g
 *     -                            On : 2015-04-08 21:26:52
 *     -           for the tree parser : regexoptTreeWalkerTreeParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
*/
// [The "BSD license"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


/* =============================================================================
 * This is what the grammar programmer asked us to put at the top of every file.
 */

    // Copyright (c) 2014-2015 Milos Jakubicek

/* End of Header action.
 * =============================================================================
 */

/* -----------------------------------------
 * Include the ANTLR3 generated header file.
 */
#include    "regexoptTreeWalker.hh"
/* ----------------------------------------- */





/* MACROS that hide the C interface implementations from the
 * generated code, which makes it a little more understandable to the human eye.
 * I am very much against using C pre-processor macros for function calls and bits
 * of code as you cannot see what is happening when single stepping in debuggers
 * and so on. The exception (in my book at least) is for generated code, where you are
 * not maintaining it, but may wish to read and understand it. If you single step it, you know that input()
 * hides some indirect calls, but is always referring to the input stream. This is
 * probably more readable than ctx->input->istream->input(snarfle0->blarg) and allows me to rejig
 * the runtime interfaces without changing the generated code too often, without
 * confusing the reader of the generated output, who may not wish to know the gory
 * details of the interface inheritance.
 */

#define		CTX	ctx

/* Aids in accessing scopes for grammar programmers
 */
#undef	SCOPE_TYPE
#undef	SCOPE_STACK
#undef	SCOPE_TOP
#define	SCOPE_TYPE(scope)   pregexoptTreeWalker_##scope##_SCOPE
#define SCOPE_STACK(scope)  pregexoptTreeWalker_##scope##Stack
#define	SCOPE_TOP(scope)    ctx->pregexoptTreeWalker_##scope##Top
#define	SCOPE_SIZE(scope)		ctx->pregexoptTreeWalker_##scope##Stack_limit
#define SCOPE_INSTANCE(scope, i)	(ctx->SCOPE_STACK(scope)->get(ctx->SCOPE_STACK(scope),i))

/* Macros for accessing things in the parser
 */

#undef	    PARSER
#undef	    RECOGNIZER
#undef	    HAVEPARSEDRULE
#undef	    INPUT
#undef	    STRSTREAM
#undef	    HASEXCEPTION
#undef	    EXCEPTION
#undef	    MATCHT
#undef	    MATCHANYT
#undef	    FOLLOWSTACK
#undef	    FOLLOWPUSH
#undef	    FOLLOWPOP
#undef	    PRECOVER
#undef	    PREPORTERROR
#undef	    LA
#undef	    LT
#undef	    CONSTRUCTEX
#undef	    CONSUME
#undef	    MARK
#undef	    REWIND
#undef	    REWINDLAST
#undef	    PERRORRECOVERY
#undef	    HASFAILED
#undef	    FAILEDFLAG
#undef	    RECOVERFROMMISMATCHEDSET
#undef	    RECOVERFROMMISMATCHEDELEMENT
#undef	    BACKTRACKING
#undef      ADAPTOR
#undef	    RULEMEMO
#undef		SEEK
#undef		INDEX
#undef		DBG

#define	    PARSER							ctx->pTreeParser
#define	    RECOGNIZER						PARSER->rec
#define		PSRSTATE						RECOGNIZER->state
#define	    HAVEPARSEDRULE(r)				RECOGNIZER->alreadyParsedRule(RECOGNIZER, r)
#define	    INPUT							PARSER->ctnstream
#define		ISTREAM							INPUT->tnstream->istream
#define	    STRSTREAM						INPUT->tnstream
#define	    HASEXCEPTION()					(PSRSTATE->error == ANTLR3_TRUE)
#define	    EXCEPTION						PSRSTATE->exception
#define	    MATCHT(t, fs)					RECOGNIZER->match(RECOGNIZER, t, fs)
#define	    MATCHANYT()						RECOGNIZER->matchAny(RECOGNIZER)
#define	    FOLLOWSTACK					    PSRSTATE->following
#define	    FOLLOWPUSH(x)					FOLLOWSTACK->push(FOLLOWSTACK, ((void *)(&(x))), NULL)
#define	    FOLLOWPOP()						FOLLOWSTACK->pop(FOLLOWSTACK)
#define	    PRECOVER()						RECOGNIZER->recover(RECOGNIZER)
#define	    PREPORTERROR()					RECOGNIZER->reportError(RECOGNIZER)
#define	    LA(n)							ISTREAM->_LA(ISTREAM, n)
#define	    LT(n)							INPUT->tnstream->_LT(INPUT->tnstream, n)
#define	    CONSTRUCTEX()					RECOGNIZER->exConstruct(RECOGNIZER)
#define	    CONSUME()						ISTREAM->consume(ISTREAM)
#define	    MARK()							ISTREAM->mark(ISTREAM)
#define	    REWIND(m)						ISTREAM->rewind(ISTREAM, m)
#define	    REWINDLAST()					ISTREAM->rewindLast(ISTREAM)
#define	    PERRORRECOVERY					PSRSTATE->errorRecovery
#define	    FAILEDFLAG						PSRSTATE->failed
#define	    HASFAILED()						(FAILEDFLAG == ANTLR3_TRUE)
#define	    BACKTRACKING					PSRSTATE->backtracking
#define	    RECOVERFROMMISMATCHEDSET(s)		RECOGNIZER->recoverFromMismatchedSet(RECOGNIZER, s)
#define	    RECOVERFROMMISMATCHEDELEMENT(e)	RECOGNIZER->recoverFromMismatchedElement(RECOGNIZER, s)
#define     ADAPTOR                         INPUT->adaptor
#define		RULEMEMO						PSRSTATE->ruleMemo
#define		SEEK(n)							ISTREAM->seek(ISTREAM, n)
#define		INDEX()							ISTREAM->index(ISTREAM)
#define		DBG								RECOGNIZER->debugger



#define		TOKTEXT(tok, txt)				tok, (pANTLR3_UINT8)txt

/* The 4 tokens defined below may well clash with your own #defines or token types. If so
 * then for the present you must use different names for your defines as these are hard coded
 * in the code generator. It would be better not to use such names internally, and maybe
 * we can change this in a forthcoming release. I deliberately do not #undef these
 * here as this will at least give you a redefined error somewhere if they clash.
 */
#define	    UP	    ANTLR3_TOKEN_UP
#define	    DOWN    ANTLR3_TOKEN_DOWN
#define	    EOR	    ANTLR3_TOKEN_EOR
#define	    INVALID ANTLR3_TOKEN_INVALID


/* =============================================================================
 * Functions to create and destroy scopes. First come the rule scopes, followed
 * by the global declared scopes.
 */



/* ============================================================================= */

/* =============================================================================
 * Start of recognizer
 */


/** \brief Table of all token names in symbolic order, mainly used for
 *         error reporting.
 */
pANTLR3_UINT8   regexoptTreeWalkerTokenNames[36+4]
     = {
        (pANTLR3_UINT8) "<invalid>",       /* String to print to indicate an invalid token */
        (pANTLR3_UINT8) "<EOR>",
        (pANTLR3_UINT8) "<DOWN>",
        (pANTLR3_UINT8) "<UP>",
        (pANTLR3_UINT8) "ALNUM",
        (pANTLR3_UINT8) "ALPHA",
        (pANTLR3_UINT8) "AND",
        (pANTLR3_UINT8) "BACKREF",
        (pANTLR3_UINT8) "BINOR",
        (pANTLR3_UINT8) "BLANK",
        (pANTLR3_UINT8) "CHARCLASS",
        (pANTLR3_UINT8) "CNTRL",
        (pANTLR3_UINT8) "DIGIT",
        (pANTLR3_UINT8) "DOT",
        (pANTLR3_UINT8) "ENUM",
        (pANTLR3_UINT8) "ESC",
        (pANTLR3_UINT8) "GRAPH",
        (pANTLR3_UINT8) "LBRACE",
        (pANTLR3_UINT8) "LBRACKET",
        (pANTLR3_UINT8) "LOWER",
        (pANTLR3_UINT8) "LPAREN",
        (pANTLR3_UINT8) "NOMETA",
        (pANTLR3_UINT8) "ONE",
        (pANTLR3_UINT8) "ONEANDMORE",
        (pANTLR3_UINT8) "PLUS",
        (pANTLR3_UINT8) "PRINT",
        (pANTLR3_UINT8) "PUNCT",
        (pANTLR3_UINT8) "QUEST",
        (pANTLR3_UINT8) "RBRACE",
        (pANTLR3_UINT8) "RBRACKET",
        (pANTLR3_UINT8) "RPAREN",
        (pANTLR3_UINT8) "SEPARATOR",
        (pANTLR3_UINT8) "SPACE",
        (pANTLR3_UINT8) "SPECIAL",
        (pANTLR3_UINT8) "STAR",
        (pANTLR3_UINT8) "TWOANDMORE",
        (pANTLR3_UINT8) "UPPER",
        (pANTLR3_UINT8) "WS_",
        (pANTLR3_UINT8) "XDIGIT",
        (pANTLR3_UINT8) "ZEROANDMORE"
       };



// Forward declare the locally static matching functions we have generated.
//
static 
 FastStream *
	regexopt    (pregexoptTreeWalker ctx, PosAttr &a);
static 
 FastStream *
	regex    (pregexoptTreeWalker ctx, PosAttr &a, bool first, bool last);
static 
 const char *
	str    (pregexoptTreeWalker ctx, bool &is_regex);
static void	regexoptTreeWalkerFree(pregexoptTreeWalker ctx);
static void     regexoptTreeWalkerReset (pregexoptTreeWalker ctx);

/* For use in tree output where we are accumulating rule labels via label += ruleRef
 * we need a function that knows how to free a return scope when the list is destroyed.
 * We cannot just use ANTLR3_FREE because in debug tracking mode, this is a macro.
 */
static	void ANTLR3_CDECL freeScope(void * scope)
{
    ANTLR3_FREE(scope);
}

/** \brief Name of the grammar file that generated this code
 */
static const char fileName[] = "regexoptTreeWalker.g";

/** \brief Return the name of the grammar file that generated this code.
 */
static const char * getGrammarFileName()
{
	return fileName;
}
/** \brief Create a new regexoptTreeWalker parser and return a context for it.
 *
 * \param[in] instream Pointer to an input stream interface.
 *
 * \return Pointer to new parser context upon success.
 */
ANTLR3_API pregexoptTreeWalker
regexoptTreeWalkerNew   (pANTLR3_COMMON_TREE_NODE_STREAM instream)
{
	// See if we can create a new parser with the standard constructor
	//
	return regexoptTreeWalkerNewSSD(instream, NULL);
}

/** \brief Create a new regexoptTreeWalker parser and return a context for it.
 *
 * \param[in] instream Pointer to an input stream interface.
 *
 * \return Pointer to new parser context upon success.
 */
ANTLR3_API pregexoptTreeWalker
regexoptTreeWalkerNewSSD   (pANTLR3_COMMON_TREE_NODE_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state)
{
    pregexoptTreeWalker ctx;	    /* Context structure we will build and return   */

    ctx	= (pregexoptTreeWalker) ANTLR3_CALLOC(1, sizeof(regexoptTreeWalker));

    if	(ctx == NULL)
    {
		// Failed to allocate memory for parser context
		//
        return  NULL;
    }

    /* -------------------------------------------------------------------
     * Memory for basic structure is allocated, now to fill in
     * the base ANTLR3 structures. We initialize the function pointers
     * for the standard ANTLR3 parser function set, but upon return
     * from here, the programmer may set the pointers to provide custom
     * implementations of each function.
     *
     * We don't use the macros defined in regexoptTreeWalker.h here, in order that you can get a sense
     * of what goes where.
     */

    /* Create a base Tree parser/recognizer, using the supplied tree node stream
     */
    ctx->pTreeParser		= antlr3TreeParserNewStream(ANTLR3_SIZE_HINT, instream, state);

    /* Install the implementation of our regexoptTreeWalker interface
     */
    ctx->regexopt	= regexopt;
    ctx->regex	= regex;
    ctx->str	= str;
    ctx->free			= regexoptTreeWalkerFree;
    ctx->reset			= regexoptTreeWalkerReset;
    ctx->getGrammarFileName	= getGrammarFileName;

    /* Install the scope pushing methods.
     */


        RECOGNIZER->displayRecognitionError = throwEvalQueryException;

    /* Install the token table
     */
    PSRSTATE->tokenNames   = regexoptTreeWalkerTokenNames;


    /* Return the newly built parser to the caller
     */
    return  ctx;
}

static void
regexoptTreeWalkerReset (pregexoptTreeWalker ctx)
{
    RECOGNIZER->reset(RECOGNIZER);
}

/** Free the parser resources
 */
 static void
 regexoptTreeWalkerFree(pregexoptTreeWalker ctx)
 {
    /* Free any scope memory
     */

	// Free this parser
	//
    ctx->pTreeParser->free(ctx->pTreeParser);

    ANTLR3_FREE(ctx);

    /* Everything is released, so we can return
     */
    return;
 }

/** Return token names used by this 
tree parser

 *
 * The returned pointer is used as an index into the token names table (using the token
 * number as the index).
 *
 * \return Pointer to first char * in the table.
 */
static pANTLR3_UINT8    *getTokenNames()
{
        return regexoptTreeWalkerTokenNames;
}


/* Declare the bitsets
 */
/** Bitset defining follow set for error recovery in rule state: FOLLOW_regex_in_regexopt83  */
static	ANTLR3_BITWORD FOLLOW_regex_in_regexopt83_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000002) };
static  ANTLR3_BITSET_LIST FOLLOW_regex_in_regexopt83	= { FOLLOW_regex_in_regexopt83_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_AND_in_regex120  */
static	ANTLR3_BITWORD FOLLOW_AND_in_regex120_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000004) };
static  ANTLR3_BITSET_LIST FOLLOW_AND_in_regex120	= { FOLLOW_AND_in_regex120_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_str_in_regex154  */
static	ANTLR3_BITWORD FOLLOW_str_in_regex154_bits[]	= { ANTLR3_UINT64_LIT(0x000000008060C148) };
static  ANTLR3_BITSET_LIST FOLLOW_str_in_regex154	= { FOLLOW_str_in_regex154_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_ONE_in_regex190  */
static	ANTLR3_BITWORD FOLLOW_ONE_in_regex190_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000004) };
static  ANTLR3_BITSET_LIST FOLLOW_ONE_in_regex190	= { FOLLOW_ONE_in_regex190_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_str_in_regex211  */
static	ANTLR3_BITWORD FOLLOW_str_in_regex211_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000008) };
static  ANTLR3_BITSET_LIST FOLLOW_str_in_regex211	= { FOLLOW_str_in_regex211_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_regex_in_regex267  */
static	ANTLR3_BITWORD FOLLOW_regex_in_regex267_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000008) };
static  ANTLR3_BITSET_LIST FOLLOW_regex_in_regex267	= { FOLLOW_regex_in_regex267_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_SEPARATOR_in_regex334  */
static	ANTLR3_BITWORD FOLLOW_SEPARATOR_in_regex334_bits[]	= { ANTLR3_UINT64_LIT(0x000000008060C148) };
static  ANTLR3_BITSET_LIST FOLLOW_SEPARATOR_in_regex334	= { FOLLOW_SEPARATOR_in_regex334_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_regex_in_regex370  */
static	ANTLR3_BITWORD FOLLOW_regex_in_regex370_bits[]	= { ANTLR3_UINT64_LIT(0x000000008060C148) };
static  ANTLR3_BITSET_LIST FOLLOW_regex_in_regex370	= { FOLLOW_regex_in_regex370_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_BINOR_in_regex425  */
static	ANTLR3_BITWORD FOLLOW_BINOR_in_regex425_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000004) };
static  ANTLR3_BITSET_LIST FOLLOW_BINOR_in_regex425	= { FOLLOW_BINOR_in_regex425_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_regex_in_regex429  */
static	ANTLR3_BITWORD FOLLOW_regex_in_regex429_bits[]	= { ANTLR3_UINT64_LIT(0x0000000080000140) };
static  ANTLR3_BITSET_LIST FOLLOW_regex_in_regex429	= { FOLLOW_regex_in_regex429_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_regex_in_regex434  */
static	ANTLR3_BITWORD FOLLOW_regex_in_regex434_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000008) };
static  ANTLR3_BITSET_LIST FOLLOW_regex_in_regex434	= { FOLLOW_regex_in_regex434_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_SEPARATOR_in_regex449  */
static	ANTLR3_BITWORD FOLLOW_SEPARATOR_in_regex449_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000002) };
static  ANTLR3_BITSET_LIST FOLLOW_SEPARATOR_in_regex449	= { FOLLOW_SEPARATOR_in_regex449_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_set_in_str481  */
static	ANTLR3_BITWORD FOLLOW_set_in_str481_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000002) };
static  ANTLR3_BITSET_LIST FOLLOW_set_in_str481	= { FOLLOW_set_in_str481_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_ENUM_in_str498  */
static	ANTLR3_BITWORD FOLLOW_ENUM_in_str498_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000002) };
static  ANTLR3_BITSET_LIST FOLLOW_ENUM_in_str498	= { FOLLOW_ENUM_in_str498_bits, 1	};


/* ==============================================
 * Parsing rules
 */
/**
 * $ANTLR start regexopt
 * regexoptTreeWalker.g:27:1: regexopt[PosAttr &a] returns [FastStream *f = NULL] : p= regex[a, true, true] ;
 */
static FastStream *
regexopt(pregexoptTreeWalker ctx, PosAttr &a)
{
    FastStream * f;


    FastStream * p;
    #undef	RETURN_TYPE_p
    #define	RETURN_TYPE_p FastStream *

    /* Initialize rule variables
     */
    f =  NULL;


    {
        // regexoptTreeWalker.g:28:5: (p= regex[a, true, true] )
        // regexoptTreeWalker.g:28:7: p= regex[a, true, true]
        {
            FOLLOWPUSH(FOLLOW_regex_in_regexopt83);
            p=regex(ctx, a, true, true);

            FOLLOWPOP();
            if  (HASEXCEPTION())
            {
                goto ruleregexoptEx;
            }


            {
                f = p;
            }


        }

    }

    // This is where rules clean up and exit
    //
    goto ruleregexoptEx; /* Prevent compiler warnings */
    ruleregexoptEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return f;
}
/* $ANTLR end regexopt */

/**
 * $ANTLR start regex
 * regexoptTreeWalker.g:31:1: regex[PosAttr &a, bool first, bool last] returns [FastStream *f = NULL] : ( ^( AND (s= str[is_regex] | ^( ONE (s= str[is_regex] |n= regex[a, first && c == 0, last && c == child_count - 1] ) ) | SEPARATOR |n= regex[a, first && c == 0, last && c == child_count - 1] )+ ) | ^( BINOR p= regex[a, first, last] q= regex[a, first, last] ) | SEPARATOR );
 */
static FastStream *
regex(pregexoptTreeWalker ctx, PosAttr &a, bool first, bool last)
{
    FastStream * f;


    const char * s;
    #undef	RETURN_TYPE_s
    #define	RETURN_TYPE_s const char *

    FastStream * n;
    #undef	RETURN_TYPE_n
    #define	RETURN_TYPE_n FastStream *

    FastStream * p;
    #undef	RETURN_TYPE_p
    #define	RETURN_TYPE_p FastStream *

    FastStream * q;
    #undef	RETURN_TYPE_q
    #define	RETURN_TYPE_q FastStream *

    /* Initialize rule variables
     */
    f =  NULL;



        vector<pair<const char*, bool> > nodes;
        nodes.reserve (32);
        bool is_regex;
        pANTLR3_BASE_TREE tree = (pANTLR3_BASE_TREE) LT(1);
        tree->freshenPACIndexesAll (tree); // known ANTLR3 C runtime bug :(
        int c, child_count = tree->getChildCount (tree);

    {
        {
            //  regexoptTreeWalker.g:40:5: ( ^( AND (s= str[is_regex] | ^( ONE (s= str[is_regex] |n= regex[a, first && c == 0, last && c == child_count - 1] ) ) | SEPARATOR |n= regex[a, first && c == 0, last && c == child_count - 1] )+ ) | ^( BINOR p= regex[a, first, last] q= regex[a, first, last] ) | SEPARATOR )

            ANTLR3_UINT32 alt3;

            alt3=3;

            switch ( LA(1) )
            {
            case AND:
            	{
            		alt3=1;
            	}
                break;
            case BINOR:
            	{
            		alt3=2;
            	}
                break;
            case SEPARATOR:
            	{
            		alt3=3;
            	}
                break;

            default:
                CONSTRUCTEX();
                EXCEPTION->type         = ANTLR3_NO_VIABLE_ALT_EXCEPTION;
                EXCEPTION->message      = (void *)"";
                EXCEPTION->decisionNum  = 3;
                EXCEPTION->state        = 0;


                goto ruleregexEx;

            }

            switch (alt3)
            {
        	case 1:
        	    // regexoptTreeWalker.g:41:7: ^( AND (s= str[is_regex] | ^( ONE (s= str[is_regex] |n= regex[a, first && c == 0, last && c == child_count - 1] ) ) | SEPARATOR |n= regex[a, first && c == 0, last && c == child_count - 1] )+ )
        	    {
        	         MATCHT(AND, &FOLLOW_AND_in_regex120);
        	        if  (HASEXCEPTION())
        	        {
        	            goto ruleregexEx;
        	        }


        	        MATCHT(ANTLR3_TOKEN_DOWN, NULL);
        	        if  (HASEXCEPTION())
        	        {
        	            goto ruleregexEx;
        	        }

        	        // regexoptTreeWalker.g:42:13: (s= str[is_regex] | ^( ONE (s= str[is_regex] |n= regex[a, first && c == 0, last && c == child_count - 1] ) ) | SEPARATOR |n= regex[a, first && c == 0, last && c == child_count - 1] )+
        	        {
        	            int cnt2=0;

        	            for (;;)
        	            {
        	                int alt2=5;
        	        	switch ( LA(1) )
        	        	{
        	        	case ENUM:
        	        	case ESC:
        	        	case NOMETA:
        	        		{
        	        			alt2=1;
        	        		}
        	        	    break;
        	        	case ONE:
        	        		{
        	        			alt2=2;
        	        		}
        	        	    break;
        	        	case SEPARATOR:
        	        		{
        	        			alt2=3;
        	        		}
        	        	    break;
        	        	case AND:
        	        	case BINOR:
        	        		{
        	        			alt2=4;
        	        		}
        	        	    break;

        	        	}

        	        	switch (alt2)
        	        	{
        	        	    case 1:
        	        	        // regexoptTreeWalker.g:42:15: s= str[is_regex]
        	        	        {
        	        	            {
        	        	                c = CHILDINDEX;
        	        	            }


        	        	            FOLLOWPUSH(FOLLOW_str_in_regex154);
        	        	            s=str(ctx, is_regex);

        	        	            FOLLOWPOP();
        	        	            if  (HASEXCEPTION())
        	        	            {
        	        	                goto ruleregexEx;
        	        	            }


        	        	            {

        	        	                              if (first && c == 0)
        	        	                                  nodes.push_back (make_pair<const char *, bool> ("^", false));
        	        	                              nodes.push_back (make_pair<const char *, bool> (s, is_regex));
        	        	                              
        	        	            }


        	        	        }
        	        	        break;
        	        	    case 2:
        	        	        // regexoptTreeWalker.g:48:15: ^( ONE (s= str[is_regex] |n= regex[a, first && c == 0, last && c == child_count - 1] ) )
        	        	        {
        	        	            {
        	        	                c = CHILDINDEX;
        	        	            }


        	        	             MATCHT(ONE, &FOLLOW_ONE_in_regex190);
        	        	            if  (HASEXCEPTION())
        	        	            {
        	        	                goto ruleregexEx;
        	        	            }


        	        	            MATCHT(ANTLR3_TOKEN_DOWN, NULL);
        	        	            if  (HASEXCEPTION())
        	        	            {
        	        	                goto ruleregexEx;
        	        	            }

        	        	            // regexoptTreeWalker.g:50:16: (s= str[is_regex] |n= regex[a, first && c == 0, last && c == child_count - 1] )
        	        	            {
        	        	                int alt1=2;
        	        	                switch ( LA(1) )
        	        	                {
        	        	                case ENUM:
        	        	                case ESC:
        	        	                case NOMETA:
        	        	                	{
        	        	                		alt1=1;
        	        	                	}
        	        	                    break;
        	        	                case AND:
        	        	                case BINOR:
        	        	                case SEPARATOR:
        	        	                	{
        	        	                		alt1=2;
        	        	                	}
        	        	                    break;

        	        	                default:
        	        	                    CONSTRUCTEX();
        	        	                    EXCEPTION->type         = ANTLR3_NO_VIABLE_ALT_EXCEPTION;
        	        	                    EXCEPTION->message      = (void *)"";
        	        	                    EXCEPTION->decisionNum  = 1;
        	        	                    EXCEPTION->state        = 0;


        	        	                    goto ruleregexEx;

        	        	                }

        	        	                switch (alt1)
        	        	                {
        	        	            	case 1:
        	        	            	    // regexoptTreeWalker.g:50:18: s= str[is_regex]
        	        	            	    {
        	        	            	        FOLLOWPUSH(FOLLOW_str_in_regex211);
        	        	            	        s=str(ctx, is_regex);

        	        	            	        FOLLOWPOP();
        	        	            	        if  (HASEXCEPTION())
        	        	            	        {
        	        	            	            goto ruleregexEx;
        	        	            	        }


        	        	            	        {

        	        	            	                            if (first && c == 0)
        	        	            	                                nodes.push_back (make_pair<const char *, bool> ("^", false));
        	        	            	                            pair<const char*, bool> val = make_pair<const char *, bool> (s, is_regex);
        	        	            	                            nodes.push_back (val);
        	        	            	                            FastStream *n = nodes2fs (&a, &nodes);
        	        	            	                            f = f ? new QAndNode (f, n) : n;
        	        	            	                            nodes.clear();
        	        	            	                            nodes.push_back (val);
        	        	            	                           
        	        	            	        }


        	        	            	    }
        	        	            	    break;
        	        	            	case 2:
        	        	            	    // regexoptTreeWalker.g:61:16: n= regex[a, first && c == 0, last && c == child_count - 1]
        	        	            	    {
        	        	            	        {
        	        	            	            c = CHILDINDEX;
        	        	            	        }


        	        	            	        FOLLOWPUSH(FOLLOW_regex_in_regex267);
        	        	            	        n=regex(ctx, a, first && c == 0, last && c == child_count - 1);

        	        	            	        FOLLOWPOP();
        	        	            	        if  (HASEXCEPTION())
        	        	            	        {
        	        	            	            goto ruleregexEx;
        	        	            	        }


        	        	            	        {
        	        	            	            f = f ? new QAndNode (f, n) : n;
        	        	            	        }


        	        	            	    }
        	        	            	    break;

        	        	                }
        	        	            }

        	        	            MATCHT(ANTLR3_TOKEN_UP, NULL);
        	        	            if  (HASEXCEPTION())
        	        	            {
        	        	                goto ruleregexEx;
        	        	            }



        	        	        }
        	        	        break;
        	        	    case 3:
        	        	        // regexoptTreeWalker.g:66:15: SEPARATOR
        	        	        {
        	        	             MATCHT(SEPARATOR, &FOLLOW_SEPARATOR_in_regex334);
        	        	            if  (HASEXCEPTION())
        	        	            {
        	        	                goto ruleregexEx;
        	        	            }


        	        	            {

        	        	                                if (!nodes.empty()) {
        	        	                                    FastStream *n = nodes2fs (&a, &nodes);
        	        	                                    f = f ? new QAndNode (f, n) : n;
        	        	                                    nodes.clear();
        	        	                                }
        	        	                              
        	        	            }


        	        	        }
        	        	        break;
        	        	    case 4:
        	        	        // regexoptTreeWalker.g:73:15: n= regex[a, first && c == 0, last && c == child_count - 1]
        	        	        {
        	        	            {
        	        	                c = CHILDINDEX;
        	        	            }


        	        	            FOLLOWPUSH(FOLLOW_regex_in_regex370);
        	        	            n=regex(ctx, a, first && c == 0, last && c == child_count - 1);

        	        	            FOLLOWPOP();
        	        	            if  (HASEXCEPTION())
        	        	            {
        	        	                goto ruleregexEx;
        	        	            }


        	        	            {
        	        	                f = f ? new QAndNode (f, n) : n;
        	        	            }


        	        	        }
        	        	        break;

        	        	    default:

        	        		if ( cnt2 >= 1 )
        	        		{
        	        		    goto loop2;
        	        		}
        	        		/* mismatchedSetEx()
        	        		 */
        	        		CONSTRUCTEX();
        	        		EXCEPTION->type = ANTLR3_EARLY_EXIT_EXCEPTION;
        	        		EXCEPTION->name = (void *)ANTLR3_EARLY_EXIT_NAME;


        	        		goto ruleregexEx;
        	        	}
        	        	cnt2++;
        	            }
        	            loop2: ;	/* Jump to here if this rule does not match */
        	        }

        	        {

        	                            if (!nodes.empty()) {
        	                                if (last)
        	                                    nodes.push_back (make_pair<const char *, bool> ("$", false));
        	                                FastStream *n = nodes2fs (&a, &nodes);
        	                                f = f ? new QAndNode (f, n) : n;
        	                            } else if (!f)
        	                                f = new SequenceStream (0, a.id_range() - 1, a.id_range());
        	                           
        	        }


        	        MATCHT(ANTLR3_TOKEN_UP, NULL);
        	        if  (HASEXCEPTION())
        	        {
        	            goto ruleregexEx;
        	        }



        	    }
        	    break;
        	case 2:
        	    // regexoptTreeWalker.g:86:10: ^( BINOR p= regex[a, first, last] q= regex[a, first, last] )
        	    {
        	         MATCHT(BINOR, &FOLLOW_BINOR_in_regex425);
        	        if  (HASEXCEPTION())
        	        {
        	            goto ruleregexEx;
        	        }


        	        MATCHT(ANTLR3_TOKEN_DOWN, NULL);
        	        if  (HASEXCEPTION())
        	        {
        	            goto ruleregexEx;
        	        }

        	        FOLLOWPUSH(FOLLOW_regex_in_regex429);
        	        p=regex(ctx, a, first, last);

        	        FOLLOWPOP();
        	        if  (HASEXCEPTION())
        	        {
        	            goto ruleregexEx;
        	        }


        	        FOLLOWPUSH(FOLLOW_regex_in_regex434);
        	        q=regex(ctx, a, first, last);

        	        FOLLOWPOP();
        	        if  (HASEXCEPTION())
        	        {
        	            goto ruleregexEx;
        	        }


        	        MATCHT(ANTLR3_TOKEN_UP, NULL);
        	        if  (HASEXCEPTION())
        	        {
        	            goto ruleregexEx;
        	        }



        	        {

        	                     if (!p)
        	                         f = q;
        	                     else if (!q)
        	                         f = p;
        	                     else
        	                         f = new QOrNode (p, q);
        	                     
        	        }


        	    }
        	    break;
        	case 3:
        	    // regexoptTreeWalker.g:94:10: SEPARATOR
        	    {
        	         MATCHT(SEPARATOR, &FOLLOW_SEPARATOR_in_regex449);
        	        if  (HASEXCEPTION())
        	        {
        	            goto ruleregexEx;
        	        }


        	        {

        	                     if (!nodes.empty()) {
        	                         FastStream *n = nodes2fs (&a, &nodes);
        	                         f = f ? new QAndNode (f, n) : n;
        	                         nodes.clear();
        	                     }
        	                     
        	        }


        	    }
        	    break;

            }
        }
    }

    // This is where rules clean up and exit
    //
    goto ruleregexEx; /* Prevent compiler warnings */
    ruleregexEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return f;
}
/* $ANTLR end regex */

/**
 * $ANTLR start str
 * regexoptTreeWalker.g:103:1: str[bool &is_regex] returns [const char *s = NULL] : (t= ( NOMETA | ESC ) |t= ENUM ) ;
 */
static const char *
str(pregexoptTreeWalker ctx, bool &is_regex)
{
    const char * s;


    pANTLR3_BASE_TREE    t;

    /* Initialize rule variables
     */
    s =  NULL;



        is_regex = false;

    t       = NULL;

    {
        // regexoptTreeWalker.g:107:5: ( (t= ( NOMETA | ESC ) |t= ENUM ) )
        // regexoptTreeWalker.g:107:7: (t= ( NOMETA | ESC ) |t= ENUM )
        {
            // regexoptTreeWalker.g:107:7: (t= ( NOMETA | ESC ) |t= ENUM )
            {
                int alt4=2;
                switch ( LA(1) )
                {
                case ESC:
                case NOMETA:
                	{
                		alt4=1;
                	}
                    break;
                case ENUM:
                	{
                		alt4=2;
                	}
                    break;

                default:
                    CONSTRUCTEX();
                    EXCEPTION->type         = ANTLR3_NO_VIABLE_ALT_EXCEPTION;
                    EXCEPTION->message      = (void *)"";
                    EXCEPTION->decisionNum  = 4;
                    EXCEPTION->state        = 0;


                    goto rulestrEx;

                }

                switch (alt4)
                {
            	case 1:
            	    // regexoptTreeWalker.g:107:8: t= ( NOMETA | ESC )
            	    {
            	        t=(pANTLR3_BASE_TREE)LT(1);

            	        if ( LA(1) == ESC || LA(1) == NOMETA )
            	        {
            	            CONSUME();
            	            PERRORRECOVERY=ANTLR3_FALSE;
            	        }
            	        else
            	        {
            	            CONSTRUCTEX();
            	            EXCEPTION->type         = ANTLR3_MISMATCHED_SET_EXCEPTION;
            	            EXCEPTION->name         = (void *)ANTLR3_MISMATCHED_SET_NAME;

            	            goto rulestrEx;
            	        }


            	        {
            	            s = (const char*) (t->getText(t))->chars;
            	        }


            	    }
            	    break;
            	case 2:
            	    // regexoptTreeWalker.g:108:8: t= ENUM
            	    {
            	        t = (pANTLR3_BASE_TREE) MATCHT(ENUM, &FOLLOW_ENUM_in_str498);
            	        if  (HASEXCEPTION())
            	        {
            	            goto rulestrEx;
            	        }


            	        {
            	            s = (const char*) (t->getText(t))->chars; is_regex = true;
            	        }


            	    }
            	    break;

                }
            }

        }

    }

    // This is where rules clean up and exit
    //
    goto rulestrEx; /* Prevent compiler warnings */
    rulestrEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return s;
}
/* $ANTLR end str */
/* End of parsing rules
 * ==============================================
 */

/* ==============================================
 * Syntactic predicates
 */
/* End of syntactic predicates
 * ==============================================
 */






/* End of code
 * =============================================================================
 */