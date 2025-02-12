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
    // Result result;
} Parser;
///////////////////////////
AstNode* statement(Parser* parser);

// expression forming - lowest to highest precedence
AstNode* logical_or(Parser* parser);
AstNode* logical_and(Parser* parser);
AstNode* bitwise_or(Parser* parser);
AstNode* bitwise_xor(Parser* parser);
AstNode* bitwise_and(Parser* parser);
AstNode* equality_comparison(Parser* parser);
AstNode* relational_comparison(Parser* parser);
AstNode* expr(Parser* parser);
AstNode* term(Parser* parser);
AstNode* factor(Parser* parser);

AstNode** scope(Parser* parser);
AstNode* if_statement(Parser* parser);
AstNode* let_statement(Parser* parser);
Parser parse(TokenList tokens);

#endif