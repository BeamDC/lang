#include "parser.h"
#include "ast.h"

void advance_parer(Parser* parser) {
    parser->tokens.current++;
}

void skip_whitespace(Parser* parser) {
    while (parser->tokens.current->type == Whitespace) {
        advance_parer(parser);
    }
}

TokenType peek(const Parser* parser) {
    if (parser->tokens.current + 1 == NULL) {
        fprintf(stderr, "Peeked at NULL! *arc of covenant face melting*");
        exit(1);
    }
    return parser->tokens.current->type;
}

Token* consume_token(Parser* parser, TokenType type) {
    // advance the parser if and only if the current token is of a specified type
    // this helps a lot with debugging and error handling
    Token* current = parser->tokens.current;
    if (current->type != type) {
        fprintf(stderr, "Expected '%s', got '%s'.", token_to_string(type), token_to_string(current->type));
        exit(1);
    }
    advance_parer(parser);
    skip_whitespace(parser);
    return current;
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
        fprintf(stderr, "No closing parenthesis");
        exit(1);
    }

    // Unary Operations
    AstNode* unary;
    switch (parser->tokens.current->type) {
        case Add:
            advance_parer(parser);
            unary = node_unary(Add, factor(parser));
            break;
        case Sub:
            advance_parer(parser);
            unary = node_unary(Sub, factor(parser));
            break;
        case Bang:
            advance_parer(parser);
            unary = node_unary(Bang, factor(parser));
            break;
        case BitNot:
            advance_parer(parser);
            unary = node_unary(BitNot, factor(parser));
            break;
        default:
            fprintf(stderr, "Unexpected Token:\n");
            fprintf(stderr, "  Type    : %s\n", token_to_string(parser->tokens.current->type));
            fprintf(stderr, "  Content : '%s'", parser->tokens.current->content);
            exit(1);
            // break;
    }
    return unary;
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

AstNode* scope(Parser* parser) {
    // todo: currently scopes can only contain a single statement,
    // the if body will need to change to allow multiple statements
    consume_token(parser, Lcurly);
    AstNode* statements  = statement(parser);
    consume_token(parser, Rcurly);
    return statements;
}

AstNode* if_statement(Parser* parser) {
    consume_token(parser, If);

    // for now conditions will be very specific,
    // in the future we will allow for more complex conditionals
    consume_token(parser, Lparen);
    Token* left = consume_token(parser, Ident);
    Token* op = consume_token(parser, Eq);
    Token* right = consume_token(parser, Numeric);
    consume_token(parser, Rparen);

    AstNode* condition = node_binary(op->type, node_ident(left->content), node_numeric(atof(right->content)));
    AstNode* body = scope(parser);

    if (peek(parser) == Else) {
        consume_token(parser, Else);
        AstNode* else_body = scope(parser);
        return node_if_statement(condition, body, else_body);
    }
    return node_if_statement(condition, body, NULL);
}

AstNode* let_statement(Parser* parser) {
    consume_token(parser, Let);
    Token* ident = consume_token(parser, Ident);
    consume_token(parser, Assign);
    AstNode* expression = expr(parser);
    consume_token(parser, Semicolon); // end of assignment
    return node_assignment(ident->content, expression);
}

AstNode* statement(Parser* parser) {
    skip_whitespace(parser);
    switch (parser->tokens.current->type) {
        case If:
            return if_statement(parser);
        case Let:
            return let_statement(parser);
        default:
            return expr(parser);
    }
    /// fails when called at EOF
}

AstNode* build_tree(Parser* parser) {
    AstNode* root = node_root();
    while (parser->tokens.current->type != Eof) {
        AstNode* stmt = statement(parser);
        if (root->data.root.used == root->data.root.size) {
            root->data.root.size *= 2;
            root->data.root.items = realloc(root->data.root.items, sizeof(AstNode*) * root->data.root.size);
        }
        // print_ast(stmt, 0);
        root->data.root.items[root->data.root.used++] = stmt;
    }
    return root;
}

Parser parse(TokenList tokens) {
    Parser parser;
    parser.code = 0;
    parser.tokens = tokens;
    parser.ast = build_tree(&parser);
    // parser.ast = statement(&parser);
    // parser.ast = statement(&parser);
    return parser;
}