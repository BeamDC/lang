#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void skip_whitespace(Parser* parser) {
    while(parser->tokens.current->type == Whitespace) {
        parser->tokens.current++;
    }
}

double factor(Parser* parser) {
    skip_whitespace(parser);

    if (parser->tokens.current->type == Numeric) {
        // currently we only support ints, will expand later
        double value = atoi(parser->tokens.current->content);
        parser->tokens.current++;
        return value;
    }

    if (parser->tokens.current->type == Lparen) {
        parser->tokens.current++;
        double value = expr(parser);
        skip_whitespace(parser);
        if (parser->tokens.current->type == Rparen) {
            parser->tokens.current++;
            return value;
        }
        fprintf(stderr, "Error : No Closing Parenthesis");
        return -1; // current exit code for error
    }
    fprintf(stderr, "Error : Unexpected Char");
    return -1; // current exit code for error
}

double term(Parser* parser) {
    double value = factor(parser);

    for(;;) {
        skip_whitespace(parser);
        TokenType op = parser->tokens.current->type;

        if (op != Mul && op != Div) {
            break;
        }

        parser->tokens.current++;
        double fact = factor(parser);

        // make functions to handle the math later (want to do it from scratch lol)
        switch (op) {
            case Mul:
                value *= fact;
                break;
            case Div:
                if (fact == 0) {
                    fprintf(stderr, "Error : Division by Zero");
                    fact = 1;
                }
                value /= fact;
                break;
            default:
                break;
        }
    }
    return value;
}

double expr(Parser* parser) {
    double value = term(parser);

    for(;;) {
        skip_whitespace(parser);
        TokenType op = parser->tokens.current->type;

        if (op != Add && op != Sub) {
            break;
        }

        parser->tokens.current++;
        double trm = term(parser);

        // make functions to handle the math later (want to do it from scratch lol)
        switch (op) {
            case Add:
                value += trm;
                break;
            case Sub:
            value -= trm;
                break;
            default:
                break;
        }
    }

    return value;
}

double parse(Parser* parser) {
    double result = expr(parser);
    skip_whitespace(parser);

    if (parser->tokens.current->type != Eof) {
        fprintf(
            stderr,
            "Error : Unexpected content at end of expr '%s' with value '%s'",
            token_to_string(parser->tokens.current->type),
            parser->tokens.current->content
            );
        parser->error_occurred = true;
        result = -1;
    }

    return result;
}