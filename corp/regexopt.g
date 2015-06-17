//  Copyright (c) 2014-2015 Milos Jakubicek

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

grammar regexopt;

options
{
    output = AST;
    language = C;
    ASTLabelType = pANTLR3_BASE_TREE;
    k = 2;
}

tokens {
    ALNUM='[:alnum:]';
    ALPHA='[:alpha:]';
    BLANK='[:blank:]';
    CNTRL='[:cntrl:]';
    DIGIT='[:digit:]';
    GRAPH='[:graph:]';
    LOWER='[:lower:]';
    PRINT='[:print:]';
    PUNCT='[:punct:]';
    SPACE='[:space:]';
    UPPER='[:upper:]';
    XDIGIT='[:xdigit:]';
    ONE;
    SEPARATOR;
    AND;
}

/*
 * If you omit the 'parser::' or 'lexer::' prefix, parser is assumed, I'm rather explicit always.
 */
@parser::includes {
    #include "cqpeval.hh"
}

@lexer::includes {
    #include "cqpeval.hh"
    #define _empty NULL
}

@lexer::header {
    // Copyright (c) 2014-2015 Milos Jakubicek
}

@parser::header {
    // Copyright (c) 2014-2015 Milos Jakubicek
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

LPAREN:      '(';
RPAREN:      ')';
LBRACKET:    '[';
RBRACKET:    ']';
LBRACE:      '{';
RBRACE:      '}';
BINOR:       '|';
STAR:        '*';
PLUS:        '+';
QUEST:       '?';
DOT:         '.';
ZEROANDMORE: '{0' ','? '}' | '{0,' ('0'..'9')+ '}'
            |'{,' ('0'..'9')+ '}';
ONEANDMORE:  '{1' ','? '}' | '{1,' ('0'..'9')+ '}';
TWOANDMORE:  '{' ('2'..'9')('0'..'9')* ','? '}' | '{' ('2'..'9')('0'..'9')*',' ('0'..'9')+ '}'
            |'{1' ('0'..'9')+ ','? '}' | '{1' ('0'..'9')+ ',' ('0'..'9')+ '}';
ESC:         '\\' (STAR | PLUS | QUEST | LBRACE | RBRACE | LBRACKET | RBRACKET
                  | LPAREN | RPAREN | DOT | BINOR | '\\' | '^' | '$'
                  );
BACKREF:     '\\' ('0'..'9')+;
SPECIAL:     '\\' .;
NOMETA:      ~(STAR | PLUS | QUEST | LBRACE | RBRACE | LBRACKET | RBRACKET 
              | LPAREN | RPAREN | DOT | BINOR | '\uFFFF'
              );
ENUM: LBRACKET CHARCLASS+ RBRACKET;
fragment CHARCLASS: ( (NOMETA|DOT) '-' (NOMETA|DOT)
           | ALNUM | ALPHA | BLANK | CNTRL | DIGIT | GRAPH | LOWER
           | PRINT | PUNCT | SPACE | UPPER | XDIGIT
           | (NOMETA|DOT)
           );


regex 
    : regalt (BINOR^ regalt)* EOF!
    ;

regalt
    : regpart+ -> ^(AND regpart+)
    ;

regpart
    : regterm
      ( ( repet_one | TWOANDMORE ) -> ^(ONE regterm)
      | repet_zero -> SEPARATOR
      | -> regterm
      )
    ;

regterm
    :
    (   LPAREN! regalt (BINOR^ regalt)* RPAREN!
    |   ENUM
    |   re_str
    )
    ;

re_str
    : (DOT|BACKREF|SPECIAL) -> SEPARATOR
    | NOMETA -> NOMETA
    | ESC -> ESC
    ;

repet_one
    : PLUS -> PLUS
    | ONEANDMORE -> ONEANDMORE
    ;

repet_zero
    : QUEST -> QUEST
    | STAR -> STAR
    | ZEROANDMORE -> ZEROANDMORE
    ;

// vim: syntax=antlr3 et ts=4 sw=4
