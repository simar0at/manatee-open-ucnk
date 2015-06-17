/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : cqp.g
 *     -                            On : 2015-04-08 21:27:01
 *     -                for the parser : cqpParserParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The parser 
cqpParser

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
 * a parser context typedef pcqpParser, which is returned from a call to cqpParserNew().
 *
 * The methods in pcqpParser are  as follows:
 *
 *  - 
 cqpParser_query_return
      pcqpParser->query(pcqpParser)
 *  - 
 cqpParser_globpart_return
      pcqpParser->globpart(pcqpParser)
 *  - 
 cqpParser_globcond_return
      pcqpParser->globcond(pcqpParser)
 *  - 
 cqpParser_within_containing_part_return
      pcqpParser->within_containing_part(pcqpParser)
 *  - 
 cqpParser_within_number_return
      pcqpParser->within_number(pcqpParser)
 *  - 
 cqpParser_structure_return
      pcqpParser->structure(pcqpParser)
 *  - 
 cqpParser_oneposonly_return
      pcqpParser->oneposonly(pcqpParser)
 *  - 
 cqpParser_alignedpart_return
      pcqpParser->alignedpart(pcqpParser)
 *  - 
 cqpParser_mupart_return
      pcqpParser->mupart(pcqpParser)
 *  - 
 cqpParser_integer_return
      pcqpParser->integer(pcqpParser)
 *  - 
 cqpParser_meetop_return
      pcqpParser->meetop(pcqpParser)
 *  - 
 cqpParser_unionop_return
      pcqpParser->unionop(pcqpParser)
 *  - 
 cqpParser_sequence_return
      pcqpParser->sequence(pcqpParser)
 *  - 
 cqpParser_seq_return
      pcqpParser->seq(pcqpParser)
 *  - 
 cqpParser_repetition_return
      pcqpParser->repetition(pcqpParser)
 *  - 
 cqpParser_attvallist_return
      pcqpParser->attvallist(pcqpParser)
 *  - 
 cqpParser_attvaland_return
      pcqpParser->attvaland(pcqpParser)
 *  - 
 cqpParser_attval_return
      pcqpParser->attval(pcqpParser)
 *  - 
 cqpParser_atomquery_return
      pcqpParser->atomquery(pcqpParser)
 *  - 
 cqpParser_position_return
      pcqpParser->position(pcqpParser)
 *  - 
 cqpParser_oneposition_return
      pcqpParser->oneposition(pcqpParser)
 *  - 
 cqpParser_repopt_return
      pcqpParser->repopt(pcqpParser)
 * 
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

#ifndef	_cqpParser_H
#define _cqpParser_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */

	#include <finlib/fsop.hh>
	#include <cstdlib>
	#include "parsop.hh"
	#include "frsop.hh"
	#include "corpus.hh"
	#include "cqpeval.hh"



// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct cqpParser_Ctx_struct cqpParser, * pcqpParser;



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

/* ========================
 * BACKTRACKING IS ENABLED
 * ========================
 */

typedef struct cqpParser_query_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_query_return;



typedef struct cqpParser_globpart_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_globpart_return;



typedef struct cqpParser_globcond_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_globcond_return;



typedef struct cqpParser_within_containing_part_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_within_containing_part_return;



typedef struct cqpParser_within_number_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_within_number_return;



typedef struct cqpParser_structure_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_structure_return;



typedef struct cqpParser_oneposonly_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_oneposonly_return;



typedef struct cqpParser_alignedpart_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_alignedpart_return;



typedef struct cqpParser_mupart_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_mupart_return;



typedef struct cqpParser_integer_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_integer_return;



typedef struct cqpParser_meetop_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_meetop_return;



typedef struct cqpParser_unionop_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_unionop_return;



typedef struct cqpParser_sequence_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_sequence_return;



typedef struct cqpParser_seq_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_seq_return;



typedef struct cqpParser_repetition_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_repetition_return;



typedef struct cqpParser_attvallist_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_attvallist_return;



typedef struct cqpParser_attvaland_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_attvaland_return;



typedef struct cqpParser_attval_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_attval_return;



typedef struct cqpParser_atomquery_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_atomquery_return;



typedef struct cqpParser_position_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_position_return;



typedef struct cqpParser_oneposition_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_oneposition_return;



typedef struct cqpParser_repopt_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    cqpParser_repopt_return;




/** Context tracking structure for 
cqpParser

 */
struct cqpParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_PARSER   pParser;

     cqpParser_query_return
     (*query)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_globpart_return
     (*globpart)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_globcond_return
     (*globcond)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_within_containing_part_return
     (*within_containing_part)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_within_number_return
     (*within_number)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_structure_return
     (*structure)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_oneposonly_return
     (*oneposonly)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_alignedpart_return
     (*alignedpart)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_mupart_return
     (*mupart)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_integer_return
     (*integer)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_meetop_return
     (*meetop)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_unionop_return
     (*unionop)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_sequence_return
     (*sequence)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_seq_return
     (*seq)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_repetition_return
     (*repetition)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_attvallist_return
     (*attvallist)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_attvaland_return
     (*attvaland)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_attval_return
     (*attval)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_atomquery_return
     (*atomquery)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_position_return
     (*position)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_oneposition_return
     (*oneposition)	(struct cqpParser_Ctx_struct * ctx);

     cqpParser_repopt_return
     (*repopt)	(struct cqpParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred1_cqp)	(struct cqpParser_Ctx_struct * ctx);
    // Delegated rules

    const char * (*getGrammarFileName)();
    void            (*reset)  (struct cqpParser_Ctx_struct * ctx);
    void	    (*free)   (struct cqpParser_Ctx_struct * ctx);
/* @headerFile.members() */
pANTLR3_BASE_TREE_ADAPTOR	adaptor;
pANTLR3_VECTOR_FACTORY		vectors;
/* End @headerFile.members() */
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pcqpParser cqpParserNew         (
pANTLR3_COMMON_TOKEN_STREAM
 instream);
ANTLR3_API pcqpParser cqpParserNewSSD      (
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

/* End of token definitions for cqpParser
 * =============================================================================
 */
/** } */


#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */