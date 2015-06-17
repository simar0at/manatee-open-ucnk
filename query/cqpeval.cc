// Copyright (c) 2001-2012  Pavel Rychly, Milos Jakubicek

#include "cqpeval.hh"
#include "cqpLexer.hh"
#include "cqpParser.hh"
#include "cqpTreeWalker.hh"
#include "config.hh"
#include <sstream>
#include <iostream>

using namespace std;

RangeStream *eval_cqpquery (const char *query, Corpus *corp)
{
    if (!corp)
    throw EvalQueryException ("Internal Error: eval_cqpquery(corp==NULL)");
    if (!query)
    throw EvalQueryException ("Internal Error: eval_cqpquery(query==NULL)");
    if (!*query)
    throw EvalQueryException ("Internal Error: eval_cqpquery(query[0]==NULL)");

    
#ifdef MANATEE_ANTLR_34_API
    pANTLR3_INPUT_STREAM input = antlr3StringStreamNew ((pANTLR3_UINT8) query, ANTLR3_ENC_8BIT, (ANTLR3_UINT32) strlen(query), (pANTLR3_UINT8) "CQP Stream");
#else
    pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringInPlaceStream ((pANTLR3_UINT8) query, (ANTLR3_UINT32) strlen(query), (pANTLR3_UINT8) "CQP Stream");
#endif
    if (input == NULL) {
        throw std::bad_alloc();
    }

    pcqpLexer lexer = cqpLexerNew(input);
    if (lexer == NULL) {
        throw std::bad_alloc();
    }
    
    pANTLR3_COMMON_TOKEN_STREAM tokenStream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
    if (tokenStream == NULL) {
        throw std::bad_alloc();
    }

    pcqpParser parser = cqpParserNew(antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer)));
    if (parser == NULL) {
        throw std::bad_alloc();
    }
    
    RangeStream *r = NULL;
    const char *errmsg = NULL;
    
    cqpParser_query_return cqpAST = parser->query(parser);
    if (parser->pParser->rec->getNumberOfSyntaxErrors(parser->pParser->rec) > 0) {
        throw EvalQueryException ("Internal error: couldn't recover from parser failure.");
    } else {
        pANTLR3_COMMON_TREE_NODE_STREAM nodes = antlr3CommonTreeNodeStreamNewTree((ANTLR3_BASE_TREE_struct*) cqpAST.tree, ANTLR3_SIZE_HINT);
        if (nodes == NULL) {
            throw std::bad_alloc();
        }
        pcqpTreeWalker treeWalker = cqpTreeWalkerNew(nodes);
        if (treeWalker == NULL) {
            throw std::bad_alloc();
        }
        r = treeWalker->query (treeWalker, *corp);
        nodes->free(nodes);
        treeWalker->free(treeWalker);
        nodes = NULL;
        treeWalker = NULL;
    }

    parser->free(parser);
    tokenStream->free(tokenStream);
    lexer->free(lexer);
    input->free(input);
    parser = NULL;
    tokenStream = NULL;
    lexer = NULL;
    input = NULL;

    if (!r) {
        errmsg = "Internal error: couldn't recover from tree walker failure.";
    }
    if (errmsg) {
        throw EvalQueryException (errmsg);
    }

    return r;
}


FastStream *eval_cqponepos (const char *query, Corpus *corp)
{
    if (!corp)
    throw EvalQueryException ("Internal Error: eval_cqponepos(corp==NULL)");
    if (!query)
    throw EvalQueryException ("Internal Error: eval_cqponepos(query==NULL)");

#ifdef MANATEE_ANTLR_34_API
    pANTLR3_INPUT_STREAM input = antlr3StringStreamNew ((pANTLR3_UINT8) query, ANTLR3_ENC_8BIT, (ANTLR3_UINT32) strlen(query), (pANTLR3_UINT8) "CQP Stream");
#else
    pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringInPlaceStream ((pANTLR3_UINT8) query, (ANTLR3_UINT32) strlen(query), (pANTLR3_UINT8) "CQP Stream");
#endif
    if (input == NULL) {
        throw std::bad_alloc();
    }

    pcqpLexer lexer = cqpLexerNew(input);
    if (lexer == NULL) {
        throw std::bad_alloc();
    }
    
    pANTLR3_COMMON_TOKEN_STREAM tokenStream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
    if (tokenStream == NULL) {
        throw std::bad_alloc();
    }

    pcqpParser parser = cqpParserNew(antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer)));
    if (parser == NULL) {
        throw std::bad_alloc();
    }
    
    FastStream *r = NULL;
    const char *errmsg = NULL;
    
    cqpParser_oneposonly_return cqpAST = parser->oneposonly(parser);
    if (parser->pParser->rec->getNumberOfSyntaxErrors(parser->pParser->rec) > 0) {
        throw EvalQueryException ("Internal error: couldn't recover from parser failure.");
    } else if (cqpAST.tree) {
        pANTLR3_COMMON_TREE_NODE_STREAM nodes = antlr3CommonTreeNodeStreamNewTree((ANTLR3_BASE_TREE_struct*) cqpAST.tree, ANTLR3_SIZE_HINT);
        if (nodes == NULL) {
            throw std::bad_alloc();
        }
        pcqpTreeWalker treeWalker = cqpTreeWalkerNew(nodes);
        if (treeWalker == NULL) {
            throw std::bad_alloc();
        }
        r = treeWalker->onepos (treeWalker, *corp);
        nodes->free(nodes);
        treeWalker->free(treeWalker);
        nodes = NULL;
        treeWalker = NULL;
    } else {
        errmsg = "No tree produced";
    }
    
    parser->free(parser);
    tokenStream->free(tokenStream);
    lexer->free(lexer);
    input->free(input);
    parser = NULL;
    tokenStream = NULL;
    lexer = NULL;
    input = NULL;

    if (!r) {
        errmsg = "Internal error: couldn't recover from tree walker failure.";
    }
    if (errmsg) {
        throw EvalQueryException (errmsg);
    }

    return r;
}

void throwEvalQueryException (pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames) {
    
    char fullExMessage[1024];
    char *exMessage = fullExMessage;

    pANTLR3_LEXER           lexer;  
    pANTLR3_PARSER          parser;
    pANTLR3_INPUT_STREAM    ips;
    pANTLR3_STRING          ttext;
    pANTLR3_EXCEPTION       ex;
    pANTLR3_COMMON_TOKEN    theToken;
    pANTLR3_BASE_TREE       theBaseTree;
    pANTLR3_COMMON_TREE     theCommonTree;

    // Retrieve some info for easy reading.
    //
    ex      =       recognizer->state->exception;
    ttext   =       NULL;

    exMessage += sprintf(exMessage, " %d: %s", recognizer->state->exception->type, (pANTLR3_UINT8) (recognizer->state->exception->message));

    // How we determine the next piece is dependent on which thing raised the
    // error.
    //
    switch  (recognizer->type)
    {
    case    ANTLR3_TYPE_LEXER:

        // Prepare the knowledge we know we have
        //
        lexer       = (pANTLR3_LEXER) (recognizer->super);
        parser      = NULL;
        ips         = lexer->input;
        ttext       = NULL;

        exMessage += sprintf(exMessage, ", at position %d: '%s'", recognizer->state->exception->charPositionInLine, (char *) ips->nextChar);
        break;

    case    ANTLR3_TYPE_PARSER:

        // Prepare the knowledge we know we have
        //
        parser      = (pANTLR3_PARSER) (recognizer->super);
        theToken    = (pANTLR3_COMMON_TOKEN)(recognizer->state->exception->token);
        ttext       = parser->tstream->toStringTT(parser->tstream, theToken, theToken);

        exMessage += sprintf(exMessage, ", at position %d", recognizer->state->exception->charPositionInLine);
        if  (theToken != NULL)
        {
            if (theToken->type == ANTLR3_TOKEN_EOF)
            {
                exMessage += sprintf(exMessage, ", at <EOF>");
            }
            else
            {
                // Guard against null text in a token
                //
                exMessage += sprintf(exMessage, " near '%s' ", ttext == NULL ? (pANTLR3_UINT8)"<no text for the token>" : ttext->chars);
            }
        }
        break;

    case    ANTLR3_TYPE_TREE_PARSER:

        parser      = NULL;
        theBaseTree = (pANTLR3_BASE_TREE)(recognizer->state->exception->token);
        ttext       = theBaseTree->toStringTree(theBaseTree);

        if  (theBaseTree != NULL)
        {
            theCommonTree   = (pANTLR3_COMMON_TREE)     theBaseTree->super;

            if  (theCommonTree != NULL)
            {
                theToken    = (pANTLR3_COMMON_TOKEN)    theBaseTree->getToken(theBaseTree);
            }
            exMessage += sprintf(exMessage, ", at position %d", theBaseTree->getCharPositionInLine(theBaseTree));
            exMessage += sprintf(exMessage, ", near '%s'", ttext->chars);
        }
        break;

    default:
        
        exMessage += sprintf(exMessage, "Internal error: unknown parser type");
        break;
    }

    // Although this function should generally be provided by the implementation, this one
    // should be as helpful as possible for grammar developers and serve as an example
    // of what you can do with each exception type. In general, when you make up your
    // 'real' handler, you should debug the routine with all possible errors you expect
    // which will then let you be as specific as possible about all circumstances.
    //
    // Note that in the general case, errors thrown by tree parsers indicate a problem
    // with the output of the parser or with the tree grammar itself. The job of the parser
    // is to produce a perfect (in traversal terms) syntactically correct tree, so errors
    // at that stage should really be semantic errors that your own code determines and handles
    // in whatever way is appropriate.
    //
    switch  (ex->type)
    {
    case    ANTLR3_UNWANTED_TOKEN_EXCEPTION:

        // Indicates that the recognizer was fed a token which seesm to be
        // spurious input. We can detect this when the token that follows
        // this unwanted token would normally be part of the syntactically
        // correct stream. Then we can see that the token we are looking at
        // is just something that should not be there and throw this exception.
        //
        if  (tokenNames == NULL)
        {
            exMessage += sprintf(exMessage, ": Extraneous input ");
        }
        else
        {
            if  (ex->expecting == ANTLR3_TOKEN_EOF)
            {
                exMessage += sprintf(exMessage, ": Extraneous input - expected <EOF>");
            }
            else
            {
                exMessage += sprintf(exMessage, ": Extraneous input - expected %s", tokenNames[ex->expecting]);
            }
        }
        break;

    case    ANTLR3_MISSING_TOKEN_EXCEPTION:

        // Indicates that the recognizer detected that the token we just
        // hit would be valid syntactically if preceeded by a particular 
        // token. Perhaps a missing ';' at line end or a missing ',' in an
        // expression list, and such like.
        //
        if  (tokenNames == NULL)
        {
            exMessage += sprintf(exMessage, ": Missing token (%d)", ex->expecting);
        }
        else
        {
            if  (ex->expecting == ANTLR3_TOKEN_EOF)
            {
                exMessage += sprintf(exMessage, ": Missing <EOF>");
            }
            else
            {
                exMessage += sprintf(exMessage, ": Missing %s", tokenNames[ex->expecting]);
            }
        }
        break;

    case    ANTLR3_RECOGNITION_EXCEPTION:

        // Indicates that the recognizer received a token
        // in the input that was not predicted. This is the basic exception type 
        // from which all others are derived. So we assume it was a syntax error.
        // You may get this if there are not more tokens and more are needed
        // to complete a parse for instance.
        //
        exMessage += sprintf(exMessage, ": syntax error");
        break;

    case    ANTLR3_MISMATCHED_TOKEN_EXCEPTION:

        // We were expecting to see one thing and got another. This is the
        // most common error if we coudl not detect a missing or unwanted token.
        // Here you can spend your efforts to
        // derive more useful error messages based on the expected
        // token set and the last token and so on. The error following
        // bitmaps do a good job of reducing the set that we were looking
        // for down to something small. Knowing what you are parsing may be
        // able to allow you to be even more specific about an error.
        //
        if  (tokenNames == NULL)
        {
            exMessage += sprintf(exMessage, ": syntax error");
        }
        else
        {
            if  (ex->expecting == ANTLR3_TOKEN_EOF)
            {
                exMessage += sprintf(exMessage, ": expected <EOF>");
            }
            else
            {
                exMessage += sprintf(exMessage, ": expected %s", tokenNames[ex->expecting]);
            }
        }
        break;

    case    ANTLR3_NO_VIABLE_ALT_EXCEPTION:

        // We could not pick any alt decision from the input given
        // so god knows what happened - however when you examine your grammar,
        // you should. It means that at the point where the current token occurred
        // that the DFA indicates nowhere to go from here.
        //
        exMessage += sprintf(exMessage, ": cannot match to any expected input");

        break;

    case    ANTLR3_MISMATCHED_SET_EXCEPTION:

        {
            ANTLR3_UINT32     count;
            ANTLR3_UINT32     bit;
            ANTLR3_UINT32     size;
            ANTLR3_UINT32     numbits;
            pANTLR3_BITSET    errBits;

            // This means we were able to deal with one of a set of
            // possible tokens at this point, but we did not see any
            // member of that set.
            //
            exMessage += sprintf(exMessage, ": unexpected input, expected one of: ");

            // What tokens could we have accepted at this point in the
            // parse?
            //
            count   = 0;
            errBits = antlr3BitsetLoad      (ex->expectingSet);
            numbits = errBits->numBits      (errBits);
            size    = errBits->size         (errBits);

            if  (size > 0)
            {
                // However many tokens we could have dealt with here, it is usually
                // not useful to print ALL of the set here. I arbitrarily chose 8
                // here, but you should do whatever makes sense for you of course.
                // No token number 0, so look for bit 1 and on.
                //
                for (bit = 1; bit < numbits && count < 8 && count < size; bit++)
                {
                    // TODO: This doesn;t look right - should be asking if the bit is set!!
                    //
                    if  (tokenNames[bit])
                    {
                        exMessage += sprintf(exMessage, "%s%s", count > 0 ? ", " : "", tokenNames[bit]);
                        count++;
                    }
                }
            }
            else
            {
                exMessage += sprintf(exMessage, "(no tokens)");
            }
        }
        break;

    case    ANTLR3_EARLY_EXIT_EXCEPTION:

        // We entered a loop requiring a number of token sequences
        // but found a token that ended that sequence earlier than
        // we should have done.
        //
        exMessage += sprintf(exMessage, ": missing elements ");
        break;

    default:

        // We don't handle any other exceptions here, but you can
        // if you wish. If we get an exception that hits this point
        // then we are just going to report what we know about the
        // token.
        //
        exMessage += sprintf(exMessage, ": syntax not recognized ");
        break;
    }

    // Here you have the token that was in error which if this is
    // the standard implementation will tell you the line and offset
    // and also record the address of the start of the line in the
    // input stream. You could therefore print the source line and so on.
    // Generally though, I would expect that your lexer/parser will keep
    // its own map of lines and source pointers or whatever as there
    // are a lot of specific things you need to know about the input
    // to do something like that.
    // Here is where you do it though :-).

    /*
     * XXX / POSSIBLE MEMORY LEAK
     * If the parser succeeds, but tree walker fails, we are able to free only 
     * the tree walker and tree nodes, not the parser and its input stream. 
     * However, to my best opinion, the tree walker should *never* fail: if so, 
     * then it's not an invalid output, but our bug. We should consider throwing a different
     * exception for tree walker which won't be caught (so that we'll get notified).
     */
    switch (recognizer->type) {
        case ANTLR3_TYPE_LEXER:
            //free token stream
            ((pANTLR3_LEXER) recognizer->super)->input->free(((pANTLR3_LEXER) recognizer->super)->input);
            //free lexer
            recognizer->free(recognizer);
            break;
        case ANTLR3_TYPE_PARSER:
            //free lexer
            ((pANTLR3_LEXER) ((pANTLR3_PARSER) recognizer->super)->tstream->tokenSource->super)
                ->free((pANTLR3_LEXER) ((pANTLR3_PARSER) recognizer->super)->tstream->tokenSource->super);
            //free token stream
            ((pANTLR3_PARSER) recognizer->super)->tstream->free(((pANTLR3_PARSER) recognizer->super)->tstream);
            //free parser
            recognizer->free(recognizer);   
            break;
        case ANTLR3_TYPE_TREE_PARSER:
            //free node stream
            ((pANTLR3_TREE_PARSER) recognizer->super)->ctnstream->free(((pANTLR3_TREE_PARSER) recognizer->super)->ctnstream);
            //free tree walker
            recognizer->free(recognizer);
            //XXX HOW TO FREE THE PARSER AND LEXER FROM HERE?
            break;
    }
    throw EvalQueryException (fullExMessage);
}

void unescapeString(pANTLR3_COMMON_TOKEN token) {
    char* fixedString = (char *) token->getText(token)->chars; 
    size_t i, j, len = strlen (fixedString);
    for (i = 0, j = 0; i < len; i++, j++) {
        if (fixedString[i] == '\\' && i + 1 < len
            && (fixedString[i + 1] == '"' || fixedString[i + 1] == '\\'))
            i++; // \" goes to "
        fixedString[j] = fixedString[i];
    }
    fixedString[j] = '\0';
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
