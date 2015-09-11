/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : regexopt.g
 *     -                            On : 2015-04-08 21:26:51
 *     -                for the parser : regexoptParserParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The parser 
regexoptParser

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
 * a parser context typedef pregexoptParser, which is returned from a call to regexoptParserNew().
 *
 * The methods in pregexoptParser are  as follows:
 *
 *  - 
 regexoptParser_regex_return
      pregexoptParser->regex(pregexoptParser)
 *  - 
 regexoptParser_regalt_return
      pregexoptParser->regalt(pregexoptParser)
 *  - 
 regexoptParser_regpart_return
      pregexoptParser->regpart(pregexoptParser)
 *  - 
 regexoptParser_regterm_return
      pregexoptParser->regterm(pregexoptParser)
 *  - 
 regexoptParser_re_str_return
      pregexoptParser->re_str(pregexoptParser)
 *  - 
 regexoptParser_repet_one_return
      pregexoptParser->repet_one(pregexoptParser)
 *  - 
 regexoptParser_repet_zero_return
      pregexoptParser->repet_zero(pregexoptParser)
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

#ifndef	_regexoptParser_H
#define _regexoptParser_H
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
typedef struct regexoptParser_Ctx_struct regexoptParser, * pregexoptParser;



    // Copyright (c) 2014-2015 Milos Jakubicek


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
typedef struct regexoptParser_regex_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    regexoptParser_regex_return;



typedef struct regexoptParser_regalt_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    regexoptParser_regalt_return;



typedef struct regexoptParser_regpart_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    regexoptParser_regpart_return;



typedef struct regexoptParser_regterm_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    regexoptParser_regterm_return;



typedef struct regexoptParser_re_str_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    regexoptParser_re_str_return;



typedef struct regexoptParser_repet_one_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    regexoptParser_repet_one_return;



typedef struct regexoptParser_repet_zero_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    regexoptParser_repet_zero_return;




/** Context tracking structure for 
regexoptParser

 */
struct regexoptParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_PARSER   pParser;

     regexoptParser_regex_return
     (*regex)	(struct regexoptParser_Ctx_struct * ctx);

     regexoptParser_regalt_return
     (*regalt)	(struct regexoptParser_Ctx_struct * ctx);

     regexoptParser_regpart_return
     (*regpart)	(struct regexoptParser_Ctx_struct * ctx);

     regexoptParser_regterm_return
     (*regterm)	(struct regexoptParser_Ctx_struct * ctx);

     regexoptParser_re_str_return
     (*re_str)	(struct regexoptParser_Ctx_struct * ctx);

     regexoptParser_repet_one_return
     (*repet_one)	(struct regexoptParser_Ctx_struct * ctx);

     regexoptParser_repet_zero_return
     (*repet_zero)	(struct regexoptParser_Ctx_struct * ctx);
    // Delegated rules

    const char * (*getGrammarFileName)();
    void            (*reset)  (struct regexoptParser_Ctx_struct * ctx);
    void	    (*free)   (struct regexoptParser_Ctx_struct * ctx);
/* @headerFile.members() */
pANTLR3_BASE_TREE_ADAPTOR	adaptor;
pANTLR3_VECTOR_FACTORY		vectors;
/* End @headerFile.members() */
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pregexoptParser regexoptParserNew         (
pANTLR3_COMMON_TOKEN_STREAM
 instream);
ANTLR3_API pregexoptParser regexoptParserNewSSD      (
pANTLR3_COMMON_TOKEN_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
parser
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
#define ALNUM      4
#define ALPHA      5
#define AND      6
#define BACKREF      7
#define BINOR      8
#define BLANK      9
#define CHARCLASS      10
#define CNTRL      11
#define DIGIT      12
#define DOT      13
#define ENUM      14
#define ESC      15
#define GRAPH      16
#define LBRACE      17
#define LBRACKET      18
#define LOWER      19
#define LPAREN      20
#define NOMETA      21
#define ONE      22
#define ONEANDMORE      23
#define PLUS      24
#define PRINT      25
#define PUNCT      26
#define QUEST      27
#define RBRACE      28
#define RBRACKET      29
#define RPAREN      30
#define SEPARATOR      31
#define SPACE      32
#define SPECIAL      33
#define STAR      34
#define TWOANDMORE      35
#define UPPER      36
#define WS_      37
#define XDIGIT      38
#define ZEROANDMORE      39
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for regexoptParser
 * =============================================================================
 */
/** } */


#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */