//  Copyright (c) 2014-2015 Milos Jakubicek

tree grammar regexoptTreeWalker;

options {
    language = C;
    tokenVocab = regexopt;
    ASTLabelType = pANTLR3_BASE_TREE;
}

@includes {
    #include <finlib/fsop.hh>
    #include "cqpeval.hh"
    FastStream *nodes2fs (PosAttr *a, vector<pair<const char*, bool> > *nodes);
    #define CHILDINDEX ((ANTLR3_COMMON_TREE_struct *) (pANTLR3_BASE_TREE)LT(1)->super)->childIndex
}

@header {
    // Copyright (c) 2014-2015 Milos Jakubicek
}

@apifuncs {
    RECOGNIZER->displayRecognitionError = throwEvalQueryException;
}


regexopt [PosAttr &a] returns [FastStream *f = NULL]
    : p=regex[a, true, true] {f = p;}
    ;

regex [PosAttr &a, bool first, bool last] returns [FastStream *f = NULL]
@init{
    vector<pair<const char*, bool> > nodes;
    nodes.reserve (32);
    bool is_regex;
    pANTLR3_BASE_TREE tree = (pANTLR3_BASE_TREE) LT(1);
    tree->freshenPACIndexesAll (tree); // known ANTLR3 C runtime bug :(
    int c, child_count = tree->getChildCount (tree);
}
    :
      ^(AND
            ( {c = CHILDINDEX;}
              s=str[is_regex] {
              if (first && c == 0)
                  nodes.push_back (make_pair<const char *, bool> ("^", false));
              nodes.push_back (make_pair<const char *, bool> (s, is_regex));
              }
            | {c = CHILDINDEX;}
              ^(ONE
               ( s=str[is_regex] {
                if (first && c == 0)
                    nodes.push_back (make_pair<const char *, bool> ("^", false));
                pair<const char*, bool> val = make_pair<const char *, bool> (s, is_regex);
                nodes.push_back (val);
                FastStream *n = nodes2fs (&a, &nodes);
                f = f ? new QAndNode (f, n) : n;
                nodes.clear();
                nodes.push_back (val);
               }
              |
               {c = CHILDINDEX;}
                n=regex[a, first && c == 0, last && c == child_count - 1]
                {f = f ? new QAndNode (f, n) : n;}
              )
              )
            | SEPARATOR {
                if (!nodes.empty()) {
                    FastStream *n = nodes2fs (&a, &nodes);
                    f = f ? new QAndNode (f, n) : n;
                    nodes.clear();
                }
              }
            | {c = CHILDINDEX;}
              n=regex[a, first && c == 0, last && c == child_count - 1]
              {f = f ? new QAndNode (f, n) : n;}
            )+ {
                if (!nodes.empty()) {
                    if (last)
                        nodes.push_back (make_pair<const char *, bool> ("$", false));
                    FastStream *n = nodes2fs (&a, &nodes);
                    f = f ? new QAndNode (f, n) : n;
                } else if (!f)
                    f = new SequenceStream (0, a.id_range() - 1, a.id_range());
               }
       )
       | ^(BINOR p=regex[a, first, last] q=regex[a, first, last]) {
         if (!p)
             f = q;
         else if (!q)
             f = p;
         else
             f = new QOrNode (p, q);
         }
       | SEPARATOR {
         if (!nodes.empty()) {
             FastStream *n = nodes2fs (&a, &nodes);
             f = f ? new QAndNode (f, n) : n;
             nodes.clear();
         }
         }
    ;

str [bool &is_regex] returns [const char *s = NULL]
@init{
    is_regex = false;
}
    : (t=(NOMETA|ESC) {s = (const char*) $t.text->chars;}
      |t=ENUM {s = (const char*) $t.text->chars; is_regex = true;}
      )
    ;

// vim: syntax=antlr3 et ts=4 sw=4
