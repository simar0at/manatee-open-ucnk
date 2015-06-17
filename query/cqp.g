//  Copyright (c) 1999-2013 Pavel Rychly, Milos Jakubicek

/*
 * Quick help
 *
 * I - resolving ambiguities:
 * 1) syntactic predicates
 *    (nterm) => nterm
 *    - if nterm matches, use it (instead of alternatives) => order of alternatives matters
 *
 * 2) semantic predicates
 *    a) disambiguating: {test==1}? nterm1 nterm2 // can be used only for disambiguating
 *    b) gated: {test==1}? => nterm1 nterm2 // works even for deterministic alternatives, can be used for turning on/off rules
 *    c) validating: nterm1 {valid==1}? // throws exception if predicate fails
 *
 *    When using disambiguating predicates, it is much better to use:
 *    {LT(1)->getText(LT(1))->compare(LT(1)->getText(LT(1)), "some_token") == 0}? a=ATTR^ nterm1
 *    than
 *    a=ATTR^ {$a.text->compare($a.text, "some_token") == 0}? nterm1
 *    since the first variant disambiguates while the second one doesn't.
 * II - building trees
 * 1) AST operators (old antlr2 way): ^ denotes root, ! doesn't include
 * 2) rewriting rules: nterm1 nterm2 -> ^(MYROOT nterm1 nterm2) // creates tree rooted by MYROOT with 2 leaves
 *    You cannot mix 1) and 2) within a single rule alternative!!!
 */

grammar cqp;

options
{
	output = AST;
	language = C;
	ASTLabelType = pANTLR3_BASE_TREE;
	k = 2;
}

tokens {
	OPT;
	REPOPT;
	SEQ;
	ANYPOS;
	BEGSTRUCT;
	ENDSTRUCT;
	WHOLESTRUCT;
	KW_MEET='meet';
	KW_UNION='union';
	KW_WITHIN='within';
	KW_CONTAINING='containing';
	KW_MU='MU';
	KW_FREQ='f';
	KW_WS='ws';
	KW_SWAP='swap';
	KW_CCOLL='ccoll';
}

/*
 * If you omit the 'parser::' or 'lexer::' prefix, parser is assumed, I'm rather explicit always.
 */
@parser::includes {
	#include <finlib/fsop.hh>
	#include <cstdlib>
	#include "parsop.hh"
	#include "frsop.hh"
	#include "corpus.hh"
	#include "cqpeval.hh"
}

@lexer::includes {
	#include "cqpeval.hh"
}

@lexer::header {
	// Copyright (c) 1999-2013  Pavel Rychly, Milos Jakubicek
}

@parser::header {
	// Copyright (c) 1999-2013  Pavel Rychly, Milos Jakubicek
}

@lexer::apifuncs {
	RECOGNIZER->displayRecognitionError = throwEvalQueryException;
}

@parser::apifuncs {
	RECOGNIZER->displayRecognitionError = throwEvalQueryException;
}

WS_ :   (' '
    |   '\t'
    |   '\n'
    |   '\r')
        { SKIP(); }
    ;

NUMBER:   ('0'..'9')+ ;
NNUMBER:   '-' ('0'..'9')+ ;

ATTR :     ('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'_'|'0'..'9')* ;
REGEXP: '"' ('\\' . |~('"'|'\\'))* '"'
		{ 
			//remove surrounding quotes
			SETTEXT(GETTEXT()->subString(GETTEXT(),1,GETTEXT()->size-2));
		}
		;
LPAREN: '(';
RPAREN: ')';
LBRACKET: '[';
RBRACKET: ']';
LBRACE: '{';
RBRACE: '}';

STAR:   '*';
PLUS:   '+';
QUEST:  '?';

BINOR:  '|';
BINAND: '&';
DOT:    '.';
COMMA:  ',';
SEMI:   ';';
COLON:  ':';
EQ:     '=';
EEQ:    '==';
TEQ:    '~';
NOT:    '!';
LEQ:    '<=';
GEQ:    '>=';
LSTRUCT:'<';
RSTRUCT:'>';
SLASH:  '/';
POSNUM: '#';

query 
	:
    // standard query sequence, including MU queries
    (sequence -> sequence)
    // possible global constraints (e.g. '& 1.tag = 2.tag')
    (BINAND globpart -> ^(BINAND $query globpart))?
    // possible within/containing suffix
    (NOT? (kw=KW_WITHIN|kw=KW_CONTAINING) wc=within_containing_part -> ^($kw NOT? $query $wc))*
    //every query ends by ;
    SEMI
    ;

globpart
    : globcond (BINAND! globcond)*
    ;

globcond
    : NUMBER DOT^ ATTR NOT? EQ NUMBER DOT! ATTR
    | KW_FREQ^ LPAREN! NUMBER DOT! ATTR RPAREN! NOT? (EQ|LEQ|GEQ|LSTRUCT|RSTRUCT) NUMBER
    ; 

within_containing_part 
	: (NOT)?
        ( (LSTRUCT! structure RSTRUCT!) => {throw EvalQueryException (": Deprecated query syntax: use <str/> instead of <str> for matching the content of a structure. <str> will match only the beginning.");} LSTRUCT! structure RSTRUCT! // DEPRECATED. TO BE REMOVED.
        | sequence
        | within_number
        | alignedpart
        )       
	;

within_number // translate "within NUMBER" as "within []{NUMBER}"
	: n=NUMBER -> ^(SEQ ^(REPOPT["REp"] ^(LBRACKET["["] ^(ANYPOS["[]"])) ^(REPOPT["RO"] $n $n))) 
	;
	
structure
	: ATTR attvallist?
	;

oneposonly
    : attvallist SEMI!
    ;
	
alignedpart
    : ATTR COLON^  sequence   // parallel alignment
    ;
//-------------------- meet/union query --------------------
mupart
	: LPAREN! (unionop | meetop) RPAREN!
	;

integer
	: NUMBER
	| n=NNUMBER -> ^(NUMBER[$n.text->chars])
	;

meetop
	: KW_MEET^ position position (integer integer)?
	;

unionop
	: KW_UNION^ position position
	;

//-------------------- regular expression query --------------------
sequence
	: seq (BINOR^ seq)*
	;

seq 
	: repetition+ -> ^(SEQ repetition+)
	;

repetition 
	: atomquery (
		repopt 	-> ^(REPOPT["REp"] atomquery repopt)
		| 		-> atomquery
	)
	| LSTRUCT
		( structure 
			( SLASH   -> ^(WHOLESTRUCT["<str/>"] structure)
			|         -> ^(BEGSTRUCT["<str>"] structure)
			)
		| SLASH structure 
			          -> ^(ENDSTRUCT["</str>"] structure)
		)
		RSTRUCT
	;

attvallist 
	: attvaland (BINOR^ attvaland)*
	;

attvaland 
	: attval (BINAND^ attval)*
	;

attval
	: ATTR NOT? 
	  ( (EQ^|LEQ^|GEQ^|TEQ^ NUMBER?) REGEXP
	  | e=EEQ^ r=REGEXP { unescapeString($r); }
	  )
	| POSNUM n=NUMBER (NNUMBER -> ^($n NNUMBER)
					  |        -> ^($n NNUMBER[$n.text->chars])
					  )
	| NOT^ attval
	| LPAREN! attvallist RPAREN!
	| KW_WS LPAREN
		( l=NUMBER COMMA s=NUMBER -> ^(KW_WS $l $s)
		| w1=REGEXP COMMA r=REGEXP COMMA w2=REGEXP -> ^(KW_WS $w1 $r $w2)
		) RPAREN
	| KW_SWAP^ LPAREN! NUMBER COMMA! attvallist RPAREN!
	| KW_CCOLL^ LPAREN! NUMBER COMMA! NUMBER COMMA! attvallist RPAREN!
	;

atomquery
	: position
	| LPAREN (sequence -> sequence)
		(NOT? KW_WITHIN  within_containing_part -> ^(KW_WITHIN NOT? $atomquery within_containing_part) //$atomquery = the part of the rule matched as far, i.e. sequence
		|NOT? KW_CONTAINING within_containing_part -> ^(KW_CONTAINING NOT? $atomquery within_containing_part)
		)* RPAREN 
	;

position
	: oneposition
	| n=NUMBER c=COLON p=oneposition -> ^(LBRACKET["["] ^($c $n $p))
	;

oneposition 
	: LBRACKET	( attvallist -> ^(LBRACKET["["] attvallist)
				| -> ^(LBRACKET["["] ^(ANYPOS["[]"]))
				) RBRACKET
	| REGEXP -> ^(LBRACKET["["] ^(EQ["="]  ATTR["-"] REGEXP))
	| TEQ NUMBER? REGEXP -> ^(LBRACKET["["] ^(TEQ["~"]  ATTR["-"] NUMBER REGEXP))
	| KW_MU {throw EvalQueryException (": Deprecated query syntax: remove the 'MU' keyword from your query");}// KW_MU! mupart //DEPRECATED
	| mupart
	;

repopt
	: STAR   -> ^(REPOPT["RO"] NUMBER["0"] NUMBER["-1"])
	| PLUS   -> ^(REPOPT["RO"] NUMBER["1"] NUMBER["-1"])
	| QUEST  -> ^(REPOPT["RO"] NUMBER["0"] NUMBER["1"])
	| LBRACE n1=NUMBER 
		(COMMA 
			( n2=NUMBER
				-> ^(REPOPT["RO"] $n1 $n2)
			|
				-> ^(REPOPT["RO"] $n1 NUMBER["-1"])
			)
		|
			-> ^(REPOPT["RO"] $n1 $n1)
		) RBRACE
	;

// vim: syntax=antlr3
