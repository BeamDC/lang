#include "parser.h"

void advance_parer(Parser* parser) {
    parser->tokens.current++;
}

void skip_whitespace(Parser* parser) {
    while (parser->tokens.current->type == Whitespace) {
        advance_parer(parser);
    }
}

AstNode* factor(Parser* parser) {
    skip_whitespace(parser);

    if (parser->tokens.current->type == Numeric) {
        double value = atoi(parser->tokens.current->content);
        advance_parer(parser);
        return node_numeric(value);
    }

    if (parser->tokens.current->type == Lparen) {
        advance_parer(parser);
        AstNode* expression = expr(parser);

        if (parser->tokens.current->type == Rparen) {
            advance_parer(parser);
            return expression;
        }
        // Error, no closing paren
    }
    // Error, unexpected char
}

AstNode* term(Parser* parser) {
    AstNode* left = factor(parser);
    skip_whitespace(parser);

    while (parser->tokens.current->type == Mul || parser->tokens.current->type == Div) {
        TokenType op = parser->tokens.current->type;
        advance_parer(parser);
        AstNode* right = factor(parser);
        left = node_binary(op, left, right);
        skip_whitespace(parser);
    }

    return left;
}

AstNode* expr(Parser* parser) {
    AstNode* left = term(parser);
    skip_whitespace(parser);

    while (parser->tokens.current->type == Add || parser->tokens.current->type == Sub) {
        TokenType op = parser->tokens.current->type;
        advance_parer(parser);
        AstNode* right = term(parser);
        left = node_binary(op, left, right);
        skip_whitespace(parser);
    }

    return left;
}

Parser parse(TokenList tokens) {
    Parser parser;
    parser.code = 0;
    parser.tokens = tokens;
    // expr (or some future equivalent) will return the full tree.
    parser.ast = expr(&parser);
    return parser;
}