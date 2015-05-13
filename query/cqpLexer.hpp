/** \file
 *  This C header file was generated by $ANTLR version 3.2 Fedora release 15 (Rawhide) Tue Feb  8 02:02:23 UTC 2011
 *
 *     -  From the grammar source file : cqp.g
 *     -                            On : 2013-05-10 15:01:50
 *     -                 for the lexer : cqpLexerLexer *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The lexer cqpLexer has the callable functions (rules) shown below,
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
 * a parser context typedef pcqpLexer, which is returned from a call to cqpLexerNew().
 *
 * As this is a generated lexer, it is unlikely you will call it 'manually'. However
 * the methods are provided anyway.
 * * The methods in pcqpLexer are  as follows:
 *
 *  -  void      pcqpLexer->KW_MEET(pcqpLexer)
 *  -  void      pcqpLexer->KW_UNION(pcqpLexer)
 *  -  void      pcqpLexer->KW_WITHIN(pcqpLexer)
 *  -  void      pcqpLexer->KW_CONTAINING(pcqpLexer)
 *  -  void      pcqpLexer->KW_MU(pcqpLexer)
 *  -  void      pcqpLexer->KW_FREQ(pcqpLexer)
 *  -  void      pcqpLexer->WS_(pcqpLexer)
 *  -  void      pcqpLexer->NUMBER(pcqpLexer)
 *  -  void      pcqpLexer->NNUMBER(pcqpLexer)
 *  -  void      pcqpLexer->ATTR(pcqpLexer)
 *  -  void      pcqpLexer->REGEXP(pcqpLexer)
 *  -  void      pcqpLexer->LPAREN(pcqpLexer)
 *  -  void      pcqpLexer->RPAREN(pcqpLexer)
 *  -  void      pcqpLexer->LBRACKET(pcqpLexer)
 *  -  void      pcqpLexer->RBRACKET(pcqpLexer)
 *  -  void      pcqpLexer->LBRACE(pcqpLexer)
 *  -  void      pcqpLexer->RBRACE(pcqpLexer)
 *  -  void      pcqpLexer->STAR(pcqpLexer)
 *  -  void      pcqpLexer->PLUS(pcqpLexer)
 *  -  void      pcqpLexer->QUEST(pcqpLexer)
 *  -  void      pcqpLexer->BINOR(pcqpLexer)
 *  -  void      pcqpLexer->BINAND(pcqpLexer)
 *  -  void      pcqpLexer->DOT(pcqpLexer)
 *  -  void      pcqpLexer->COMMA(pcqpLexer)
 *  -  void      pcqpLexer->SEMI(pcqpLexer)
 *  -  void      pcqpLexer->COLON(pcqpLexer)
 *  -  void      pcqpLexer->EQ(pcqpLexer)
 *  -  void      pcqpLexer->EEQ(pcqpLexer)
 *  -  void      pcqpLexer->NOT(pcqpLexer)
 *  -  void      pcqpLexer->LEQ(pcqpLexer)
 *  -  void      pcqpLexer->GEQ(pcqpLexer)
 *  -  void      pcqpLexer->LSTRUCT(pcqpLexer)
 *  -  void      pcqpLexer->RSTRUCT(pcqpLexer)
 *  -  void      pcqpLexer->SLASH(pcqpLexer)
 *  -  void      pcqpLexer->POSNUM(pcqpLexer)
 *  -  void      pcqpLexer->Tokens(pcqpLexer)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD licence"]
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

#ifndef	_cqpLexer_H
#define _cqpLexer_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */

	#include "cqpeval.hh"

 

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct cqpLexer_Ctx_struct cqpLexer, * pcqpLexer;



	// Copyright (c) 1999-2010 Pavel Rychly


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

/** Context tracking structure for cqpLexer
 */
struct cqpLexer_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_LEXER    pLexer;


     void (*mKW_MEET)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mKW_UNION)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mKW_WITHIN)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mKW_CONTAINING)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mKW_MU)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mKW_FREQ)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mWS_)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mNUMBER)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mNNUMBER)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mATTR)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mREGEXP)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mLPAREN)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mRPAREN)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mLBRACKET)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mRBRACKET)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mLBRACE)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mRBRACE)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mSTAR)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mPLUS)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mQUEST)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mBINOR)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mBINAND)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mDOT)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mCOMMA)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mSEMI)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mCOLON)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mEQ)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mEEQ)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mNOT)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mLEQ)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mGEQ)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mLSTRUCT)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mRSTRUCT)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mSLASH)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mPOSNUM)	(struct cqpLexer_Ctx_struct * ctx);
     void (*mTokens)	(struct cqpLexer_Ctx_struct * ctx);    const char * (*getGrammarFileName)();
    void	    (*free)   (struct cqpLexer_Ctx_struct * ctx);
        
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pcqpLexer cqpLexerNew         (pANTLR3_INPUT_STREAM instream);
ANTLR3_API pcqpLexer cqpLexerNewSSD      (pANTLR3_INPUT_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the lexer will work with.
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
#define KW_WITHIN      13
#define STAR      28
#define EEQ      38
#define LBRACE      26
#define ANYPOS      7
#define BINAND      32
#define POSNUM      45
#define NOT      39
#define EOF      -1
#define ENDSTRUCT      9
#define LPAREN      22
#define QUEST      30
#define LBRACKET      24
#define RPAREN      23
#define SLASH      44
#define WS_      17
#define KW_MU      15
#define WHOLESTRUCT      10
#define SEQ      6
#define COMMA      34
#define KW_MEET      11
#define REGEXP      21
#define PLUS      29
#define GEQ      41
#define RBRACKET      25
#define EQ      37
#define DOT      33
#define ATTR      20
#define BEGSTRUCT      8
#define BINOR      31
#define RBRACE      27
#define KW_UNION      12
#define OPT      4
#define NNUMBER      19
#define KW_CONTAINING      14
#define NUMBER      18
#define KW_FREQ      16
#define SEMI      35
#define REPOPT      5
#define COLON      36
#define LSTRUCT      42
#define RSTRUCT      43
#define LEQ      40
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for cqpLexer
 * =============================================================================
 */
/** \} */


#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
