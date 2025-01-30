#ifndef PARSER_H
#define PARSER_H
#include "token.h"


typedef enum {
    OP_None,
    OP_Assignment,
    OP_And,
    OP_Or,
    OP_Eq,
    OP_Comp,
    OP_Term,
    OP_Factor,
    OP_Unary,
    OP_Call,
    OP_Primary
} Precedence;

typedef struct {
    TokenList tokens;
    // not yet in use
    // Token* current;
    // Token* previous;
    bool error_occurred;
} Parser;

double expr(Parser* parser);
double term(Parser* parser);
double factor(Parser* parser);
double parse(Parser* parser);

#endif
