//  Copyright (c) 1999-2014  Pavel Rychly, Milos Jakubicek

tree grammar cqpTreeWalker;

options {
	language = C;
	tokenVocab = cqp;
	ASTLabelType = pANTLR3_BASE_TREE;
}

@includes {
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
}

@header {
    // Copyright (c) 1999-2013  Pavel Rychly, Milos Jakubicek
}

@apifuncs {
    RECOGNIZER->displayRecognitionError = throwEvalQueryException;
}



prog [Corpus &c] returns [RangeStream *r = NULL]
	: query[c];

query [Corpus &c] returns [RangeStream *r = NULL]
	: within_containing[c] {$r=$within_containing.r;}
	| globcond[c] {$r=$globcond.r;}
	;

within_containing [Corpus &c] returns [RangeStream *r = NULL]
@init{
	bool within_containing_not = false;
	bool not_within_containing = false;
}
	: ^(KW_WITHIN (NOT {not_within_containing=true;})? p=query[c]
            (NOT {within_containing_not=true;})?
            (
				( q=structure[c] | q=query[c] )
			| 
				q=alignedpart[c] 
			)
        )
	    {if (within_containing_not)
            q = RQoutsideNode::create (q, c.size());
         if (not_within_containing)
            r = new RQnotInNode (p, q);
         else
            r = new RQinNode (p, q);}
	| ^(KW_CONTAINING (NOT {not_within_containing=true;})? p=query[c]
            (NOT {within_containing_not=true;})?
            (
				( q=structure[c] | q=query[c] )
			| 
				q=alignedpart[c] 
			)
        )
	    {if (within_containing_not)
            q = RQoutsideNode::create (q, c.size());
         if (not_within_containing)
            r = new RQnotContainNode (p, q);
         else
            r = new RQcontainNode (p, q);}
	;

alignedpart [Corpus &c] returns [RangeStream *r = NULL]
@init{
	Corpus *al_corp;
}    
	: 
	  ^(COLON al_name=ATTR {al_corp = c.get_aligned((const char *) $al_name.text->chars); al_corp->set_default_attr (c.get_default_attr()->name); }
        q=bigseq[*al_corp]
        {r = c.map_aligned (al_corp, q);}
        )
    ;

globcond [Corpus &c] returns [RangeStream *r = NULL]
@init{
	FastStream *a, *b; RQFilterPos::Op posOp; RQFilterFreq::Op freqOp; int neg = 0;
}
    : 
        ^(BINAND bigseq[c] {$r=$bigseq.r;} 
            (^(DOT n1=NUMBER a1=ATTR 
                    (EQ  {posOp=RQFilterPos::F_EQ;}
                    |NOT EQ {posOp=RQFilterPos::F_NEQ;}
                    )
                    n2=NUMBER a2=ATTR)
                {r= new RQFilterPos (r, c.get_attr ((const char *) $a1.text->chars),
                        c.get_attr ((const char *) $a2.text->chars), posOp,
                        atol ((const char *) $n1.text->chars),
                        atol ((const char *) $n2.text->chars));
                }
			|^(KW_FREQ n1=NUMBER a1=ATTR (NOT { neg=1; })?
					(EQ {freqOp=RQFilterFreq::F_EQ;}
					|LEQ {freqOp=RQFilterFreq::F_LEQ;}
					|GEQ {freqOp=RQFilterFreq::F_GEQ;}
					|LSTRUCT {freqOp=RQFilterFreq::F_GEQ; neg=1;}
					|RSTRUCT {freqOp=RQFilterFreq::F_LEQ; neg=1;}
					) f=NUMBER)
				{r= new RQFilterFreq(r, c.get_attr ((const char *) $a1.text->chars),
						freqOp, neg, atoll ((const char *) $n1.text->chars),
						STR2NUMPOS ((const char *) $f.text->chars));}
            )+
        )
    |
        bigseq[c] {$r = $bigseq.r;}
    ;

structure [Corpus &c] returns [RangeStream *r = NULL]
@init{
	Structure *strc;
}	
	: 
	  sn=ATTR {strc = c.get_struct ((const char *) $sn.text->chars);}
	    (
			{r = strc->rng->whole();}
	    | fs = onepos[*strc]
			{r = strc->rng->part(fs);}
	    )
	;

bigseq [Corpus &c] returns [RangeStream *r = NULL]
@init{
	bool usesort=false;
}	
	:
	  ^(SEQ (s=seqpart[c] {r = s;})? 
			(
				(
	  			  fs=muquery[c] {p = new Pos2Range (fs, 0, 1);}
				| p=repeat[c]
				| p=within_containing[c]
				| p=bigseq[c]
				| ^(BEGSTRUCT p=structure[c])  
					{p = new Pos2Range (new BegsOfRStream (p), 0, 0);}
				| ^(ENDSTRUCT p=structure[c])  
					{p = new Pos2Range (new EndsOfRStream (p), 0, 0);}
				| ^(WHOLESTRUCT p=structure[c])
				)
					{if (r) {
                        r = RQConcatNodeUnsort (r, p);
                        usesort = true;
                    } else {
                        r = p;
                    }}
				(p=seqpart[c]  
					{r = RQConcatNodeUnsort (r,p); usesort=true;}
				)?
			)*
		)
        {if (usesort) r = new RQSortBeg (r);}

	| ^(BINOR p=bigseq[c] q=bigseq[c]) {r = new RQUnionNode (p,q); }
	;

seqpart [Corpus &c] returns [RangeStream *r = NULL]
@init{
	int count = 1;
}	
	: 
	  ^(LBRACKET res=labeledpos[c])
		(
			^(LBRACKET s=labeledpos[c]) 
			{res = new QAndNode(res, new QMoveNode (s, -count)); count++;}
		)*
		{r = new Pos2Range (res, 0, count);}
	;

labeledpos [Corpus &c] returns [FastStream *ls = NULL]
@init{
	int label=0;
}    
	: 
	  (
        ^(COLON n=NUMBER ^(LBRACKET s=onepos[c]))
            {label = atol ((const char *) $n.text->chars);}
      | s = onepos[c]
      )
        {ls = new AddLabel (s, label);}

    ;

repeat [Corpus &c] returns [RangeStream *r = NULL]
@init{
	int rn1, rn2;
}	
	: 
	  ^(REPOPT 
			(
				p=query[c] ^(REPOPT n1=NUMBER n2=NUMBER)
				{
					rn1 = atol ((const char *) $n1.text->chars);
					rn2 = atol ((const char *) $n2.text->chars);
					if (rn1 == 0 && rn2 == 1) {
						r = new RQOptionalNode (p);
					} else {
						r = new RQRepeatNode (p, rn1, rn2);
					}
				}
			|
				^(LBRACKET fs=onepos[c]) ^(REPOPT n3=NUMBER n4=NUMBER)
				{
					rn1 = atol ((const char *) $n3.text->chars);
					rn2 = atol ((const char *) $n4.text->chars);
					if (rn1 == 0 && rn2 == 1) {
						r = new RQOptionalNode (new Pos2Range (fs));
					} else {
						r = new RQRepeatFSNode (fs, rn1, rn2);
					}
				}
			)
		)
	;


onepos [Corpus &c] returns [FastStream *r = NULL]
@init {
	int neg = 0, cmp = 0, thesnum = 0;
}
	: 
	  ^(BINAND a=onepos[c] b=onepos[c])   {r = new QAndNode (a,b);}
	| ^(BINOR a=onepos[c] b=onepos[c])	{r = new QOrNode (a,b);}
	| ^((EQ|LEQ { cmp = -1; } |GEQ { cmp = 1; }|EEQ { cmp = 2; }|TEQ { cmp = 3; })
		a1=ATTR n=NUMBER? (NOT { neg = 1; })? v1=REGEXP)
		{
			if (n)
				thesnum = atoi ((const char *) $n.text->chars);
			if (!cmp) {
				r = c.get_attr ((const char *) $a1.text->chars)->regexp2poss ((const char *) $v1.text->chars, false);
			} else if (cmp == 2) {
				PosAttr *pa = c.get_attr ((const char *) $a1.text->chars);
				r = pa->id2poss (pa->str2id ((const char *) $v1.text->chars));
#ifdef MANATEE_SKETCH_ENGINE
			} else if (cmp == 3) {
				r = id2thesposs (&c, (const char *) $a1.text->chars, (const char *) $v1.text->chars, thesnum);
#endif
			} else {
				r = c.get_attr ((const char*) $a1.text->chars)->compare2poss ((const char *) $v1.text->chars, cmp, false);
			}
			if (neg) {
				if (r->final() == 0) {
					// empty stream
					delete r;
					r = new SequenceStream (0, c.size() -1, c.size());
				} else {
					r = new QNotNode (r, c.size());
				}
			}
		}
    | ^(NOT a=onepos[c])       {r = new QNotNode (a, c.size());}
	| ^(ANYPOS ())	{r = new SequenceStream (0, c.size() -1, c.size());}
	| ^(n1=NUMBER n2=NNUMBER
		{
		Position beg = atoll((const char *) $n1.text->chars);
		Position end = llabs(atoll((const char *) $n2.text->chars));
		r = new SequenceStream (beg, end, c.size());
		})
	| ^(KW_SWAP n=NUMBER a=onepos[c])
		{
		Position coll = atol((const char *) $n.text->chars);
		r = new SwapKwicColl (a, coll);
		}
	| ^(KW_CCOLL f=NUMBER t=NUMBER a=onepos[c])
		{
		int from = atol((const char *) $f.text->chars);
		int to = atol((const char *) $t.text->chars);
		r = new ChangeLabel (a, from, to);
		}
	| ^(KW_WS level=NUMBER seek=NUMBER)
		{
#ifdef MANATEE_SKETCH_ENGINE
		int l = atoi((const char *) $level.text->chars);
		Position s = atoll((const char *) $seek.text->chars);
		WMap *ws = new_WMap (c.get_conf("WSBASE"), c.get_confpath(), l, s);
		std::vector<FastStream*> *svec = new std::vector<FastStream*>();
		svec->push_back (ws->poss());
		r = new QOrVNode_clean_wmap (svec, ws);
#else
        throw EvalQueryException (": word sketches not supported");
#endif
		}
	| ^(KW_WS w1=REGEXP gr=REGEXP w2=REGEXP)
		{
#ifdef MANATEE_SKETCH_ENGINE
		std::vector<FastStream*> *svec = new std::vector<FastStream*>();
		PosAttr *pa = c.get_attr (c.get_conf("WSATTR"));
		const char *locale = c.get_conf(c.get_conf("WSATTR") + ".LOCALE").c_str();
		WMap *ws0 = new_WMap (c.get_conf("WSBASE"), c.get_confpath());
		const char *pat1 = (const char *) $w1.text->chars;
		const char *pat2 = (const char *) $w2.text->chars;
		regexp_pattern re1 (pat1, locale, c.get_conf("ENCODING").c_str());
		re1.compile();
		regexp_pattern re2 (pat2, locale, c.get_conf("ENCODING").c_str());
		re2.compile();
		do {
			if (re1.no_meta_chars()) {
				if (!ws0->findid (pa->str2id (pat1)))
					break;
			} else if (!re1.match (pa->id2str (ws0->getid())))
				continue;
			WMap *ws1 = ws0->nextlevel();
			Generator<int> *gen = ws1->regexp2ids ((const char *) $gr.text->chars);
			while (!gen->end()) {
				int id = gen->next();
				if (!ws1->findid(id))
					continue;
				WMap *ws2 = ws1->nextlevel();
				do {
					if (re2.no_meta_chars()) {
						if (!ws2->findid (pa->str2id (pat2)))
							break;
					} else if (!re2.match (pa->id2str (ws2->getid())))
						continue;
					svec->push_back (ws2->poss());
				} while (ws2->next());
				delete ws2;
			}
			delete gen;
			delete ws1;
		} while (ws0->next());
		if (svec->empty()) {
			delete ws0;
			delete svec;
			r = new EmptyStream();
		} else
			r = new QOrVNode_clean_wmap (svec, ws0);
#else
        throw EvalQueryException (": word sketches not supported");
#endif
		}
	;


muquery [Corpus &c] returns [FastStream *r = NULL]
	: 
	  ^(KW_UNION  a=mupart[c] b=mupart[c]) {r = new QOrNode (a,b);}
	| ^(KW_MEET a=mupart[c] b=mupart[c]
			(n1=NUMBER n2=NUMBER
				{
					int i1 = atol ((const char *) $n1.text->chars);
					int i2 = atol ((const char *) $n2.text->chars);
					if (i1 == i2)
						r = new QAndNode(a, new QMoveNode (b, -i1));
					else
						r = new BegsOfRStream (new RQinNode (new Pos2Range (a),
									new Pos2Range (b, -i2, -i1+1)));
				}
			|
				{r = new QAndNode(a, new QMoveNode (b, -1));}
			)
		)
	;

mupart [Corpus &c] returns [FastStream *r = NULL]
	:
	  muquery[c] { $r=$muquery.r; }
	| ^(LBRACKET labeledpos[c]) {$r=$labeledpos.ls;}
	;

// vim: syntax=antlr3
