/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : cqpTreeWalker.g
 *     -                            On : 2015-04-08 21:27:02
 *     -           for the tree parser : cqpTreeWalkerTreeParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The tree parser 
cqpTreeWalker

has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 *
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pcqpTreeWalker, which is returned from a call to cqpTreeWalkerNew().
 *
 * The methods in pcqpTreeWalker are  as follows:
 *
 *  - 
 RangeStream *
      pcqpTreeWalker->prog(pcqpTreeWalker)
 *  - 
 RangeStream *
      pcqpTreeWalker->query(pcqpTreeWalker)
 *  - 
 RangeStream *
      pcqpTreeWalker->within_containing(pcqpTreeWalker)
 *  - 
 RangeStream *
      pcqpTreeWalker->alignedpart(pcqpTreeWalker)
 *  - 
 RangeStream *
      pcqpTreeWalker->globcond(pcqpTreeWalker)
 *  - 
 RangeStream *
      pcqpTreeWalker->structure(pcqpTreeWalker)
 *  - 
 RangeStream *
      pcqpTreeWalker->bigseq(pcqpTreeWalker)
 *  - 
 RangeStream *
      pcqpTreeWalker->seqpart(pcqpTreeWalker)
 *  - 
 FastStream *
      pcqpTreeWalker->labeledpos(pcqpTreeWalker)
 *  - 
 RangeStream *
      pcqpTreeWalker->repeat(pcqpTreeWalker)
 *  - 
 FastStream *
      pcqpTreeWalker->onepos(pcqpTreeWalker)
 *  - 
 FastStream *
      pcqpTreeWalker->muquery(pcqpTreeWalker)
 *  - 
 FastStream *
      pcqpTreeWalker->mupart(pcqpTreeWalker)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
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

#ifndef	_cqpTreeWalker_H
#define _cqpTreeWalker_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */

	#include <finlib/fsop.hh>
	#include <finlib/regpref.hh>
	#include "frsop.hh"
	#include "corpus.hh"
	#include "parsop.hh"
	#include "cqpeval.hh"
	#include "config.hh"
#ifdef MANATEE_SKETCH_ENGINE
	#include "wmap.hh"
#endif
	#include <cstdlib>



// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct cqpTreeWalker_Ctx_struct cqpTreeWalker, * pcqpTreeWalker;



    // Copyright (c) 1999-2013  Pavel Rychly, Milos Jakubicek


#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif

/** Context tracking structure for 
cqpTreeWalker

 */
struct cqpTreeWalker_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_TREE_PARSER	    pTreeParser;


     RangeStream *
     (*prog)	(struct cqpTreeWalker_Ctx_struct * ctx, Corpus &c);

     RangeStream *
     (*query)	(struct cqpTreeWalker_Ctx_struct * ctx, Corpus &c);

     RangeStream *
     (*within_containing)	(struct cqpTreeWalker_Ctx_struct * ctx, Corpus &c);

     RangeStream *
     (*alignedpart)	(struct cqpTreeWalker_Ctx_struct * ctx, Corpus &c);

     RangeStream *
     (*globcond)	(struct cqpTreeWalker_Ctx_struct * ctx, Corpus &c);

     RangeStream *
     (*structure)	(struct cqpTreeWalker_Ctx_struct * ctx, Corpus &c);

     RangeStream *
     (*bigseq)	(struct cqpTreeWalker_Ctx_struct * ctx, Corpus &c);

     RangeStream *
     (*seqpart)	(struct cqpTreeWalker_Ctx_struct * ctx, Corpus &c);

     FastStream *
     (*labeledpos)	(struct cqpTreeWalker_Ctx_struct * ctx, Corpus &c);

     RangeStream *
     (*repeat)	(struct cqpTreeWalker_Ctx_struct * ctx, Corpus &c);

     FastStream *
     (*onepos)	(struct cqpTreeWalker_Ctx_struct * ctx, Corpus &c);

     FastStream *
     (*muquery)	(struct cqpTreeWalker_Ctx_struct * ctx, Corpus &c);

     FastStream *
     (*mupart)	(struct cqpTreeWalker_Ctx_struct * ctx, Corpus &c);
    // Delegated rules

    const char * (*getGrammarFileName)();
    void            (*reset)  (struct cqpTreeWalker_Ctx_struct * ctx);
    void	    (*free)   (struct cqpTreeWalker_Ctx_struct * ctx);
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pcqpTreeWalker cqpTreeWalkerNew         (
pANTLR3_COMMON_TREE_NODE_STREAM
 instream);
ANTLR3_API pcqpTreeWalker cqpTreeWalkerNewSSD      (
pANTLR3_COMMON_TREE_NODE_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
tree parser
 will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif
#define EOF      -1
#define ANYPOS      4
#define ATTR      5
#define BEGSTRUCT      6
#define BINAND      7
#define BINOR      8
#define COLON      9
#define COMMA      10
#define DOT      11
#define EEQ      12
#define ENDSTRUCT      13
#define EQ      14
#define GEQ      15
#define KW_CCOLL      16
#define KW_CONTAINING      17
#define KW_FREQ      18
#define KW_MEET      19
#define KW_MU      20
#define KW_SWAP      21
#define KW_UNION      22
#define KW_WITHIN      23
#define KW_WS      24
#define LBRACE      25
#define LBRACKET      26
#define LEQ      27
#define LPAREN      28
#define LSTRUCT      29
#define NNUMBER      30
#define NOT      31
#define NUMBER      32
#define OPT      33
#define PLUS      34
#define POSNUM      35
#define QUEST      36
#define RBRACE      37
#define RBRACKET      38
#define REGEXP      39
#define REPOPT      40
#define RPAREN      41
#define RSTRUCT      42
#define SEMI      43
#define SEQ      44
#define SLASH      45
#define STAR      46
#define TEQ      47
#define WHOLESTRUCT      48
#define WS_      49
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for cqpTreeWalker
 * =============================================================================
 */
/** } */


#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
