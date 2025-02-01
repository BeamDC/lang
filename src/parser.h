#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "token.h"

/// Parser Construction ///
typedef enum {
    Res_Ok,
    Res_Error,
} ResultType;

typedef struct {
    ResultType type;
    const char* msg;
} Result;

typedef struct {
    TokenList tokens;
    AstNode* ast;
    size_t code;
    Result result;
} Parser;
///////////////////////////

AstNode* expr(Parser* parser);
AstNode* term(Parser* parser);
AstNode* factor(Parser* parser);
Parser parse(TokenList tokens);

#endif