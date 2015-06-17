//  Copyright (c) 1999-2013  Pavel Rychly, Milos Jakubicek

grammar corpconf;

options {
	language = C;
	output = AST;
	ASTLabelType = pANTLR3_BASE_TREE;
}

@header {
// Copyright (c) 1999-2013  Pavel Rychly, Milos Jakubicek
#include "corpconf.hh"
#include <string>
}

@lexer::header {
// Copyright (c) 1999-2013  Pavel Rychly, Milos Jakubicek
}

@lexer::members {
	char paraphrase[100];
}

WS_	:	(' '
		|'\t'
		|'#' (~'\n')*
		)
		{ SKIP(); }
	;

STR:   ('"' (~('"'|'\n'))* '"'
		|'\'' (~('\''|'\n'))* '\'')
	   { 
		//remove surrounding quotes
		SETTEXT(GETTEXT()->subString(GETTEXT(),1,GETTEXT()->size-2));
	   }
	   ;
NL
@init {
	strcpy(paraphrase, "end of line");
}
@after {
	paraphrase[0] = 0;
}
	:     ('\n'|'\r');
ATTR:   ('A'..'Z')('A'..'Z'|'0'..'9')* ;
PATH:   ('a'..'z'|'0'..'9'|'/'|'.') 
		('a'..'z'|'A'..'Z'|'0'..'9'|'/'|'.'|'-'|'_')* ;

LBRACE: '{';
RBRACE
@init {
	strcpy(paraphrase, "`}'");
}
@after {
	paraphrase[0] = 0;
}	
	: '}';


config [CorpInfo &c]
	: block[c] EOF
	;
	catch [RecognitionError] {
		PREPORTERROR();
		RECOGNIZER->consumeUntil(RECOGNIZER, NL);
		config(CTX, $c);
	}

block [CorpInfo &c]
	: (line[c]|NL)*
	;

line [CorpInfo &c]
@init{CorpInfo *b = NULL;}
	:
		( 'ATTRIBUTE' v=value
			{ b = new CorpInfo();
			  $c.attrs.push_back (std::pair<std::string,CorpInfo*>($v.v, b)); 
			}
		| 'STRUCTURE' v=value
			{ b = new CorpInfo();
			  $c.structs.push_back (std::pair<std::string,CorpInfo*>($v.v, b)); 
			}
		| 'PROCESS' v=value
			{ b = new CorpInfo();
			  $c.procs.push_back (std::pair<std::string,CorpInfo*>($v.v, b)); 
			}
		)
		(LBRACE NL block[*b] r=RBRACE)? (NL | EOF)
	| a=ATTR v=value (NL | EOF)
		{$c.opts [std::string((const char *) $a.text->chars)] = $v.v;}
	; 
	catch [RecognitionError] {
		PREPORTERROR();
		RECOGNIZER->consumeUntil(RECOGNIZER, NL);
		block(CTX, *b);
	}

value returns [std::string v]
	: s=STR            {$v = std::string((const char *) $s.text->chars);}
	| p=PATH           {$v = std::string((const char *) $p.text->chars);}
	| 'AT' a=PATH       {$v = std::string("@") + std::string((const char *) $a.text->chars);}
	;
