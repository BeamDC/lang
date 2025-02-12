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

Token* consume_token(Parser* parser, const TokenType type) {
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

bool match_token(TokenType current, TokenType* types, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (current == types[i]) {
            return true;
        }
    }
    return false;
}

Token* consume_token_variants(Parser* parser, TokenType* types, size_t size) {
    TokenType current = peek(parser);
    for (size_t i = 0; i < size; ++i) {
        if (current == types[i]) {
            return consume_token(parser, current);
        }
    }
    fprintf(stderr, "Unexpected token : %s\n", token_to_string(current));
    exit(1);
}

AstNode* factor(Parser* parser) {
    skip_whitespace(parser);

    if (peek(parser) == Numeric) {
        double value = atoi(parser->tokens.current->content);
        advance_parer(parser);
        return node_numeric(value);
    }

    if (peek(parser) == Ident) {
        Token* var = consume_token(parser, Ident);
        return node_ident(var->content);
    }

    if (peek(parser) == Lparen) {
        consume_token(parser, Lparen);
        AstNode* expression = logical_or(parser);
        consume_token(parser, Rparen);
        return expression;
    }

    // Unary Operations
    TokenType* types = (TokenType[]) {Add, Sub, Bang, BitNot};
    size_t types_size = 4;
    TokenType type = peek(parser);

    if (match_token(type, types, types_size)) {
        advance_parer(parser);
        return node_unary(type, factor(parser));
    }

    fprintf(stderr, "Unexpected Token:\n");
    fprintf(stderr, "  Type    : %s\n", token_to_string(parser->tokens.current->type));
    fprintf(stderr, "  Content : '%s'", parser->tokens.current->content);
    fprintf(stderr, "  line : '%llu'", parser->tokens.current->line);
    fprintf(stderr, "  Col : '%llu'", parser->tokens.current->col);
    exit(1);

}

AstNode* term(Parser* parser) {
    AstNode* left = factor(parser);
    skip_whitespace(parser);
    TokenType* types = (TokenType[]) {Mul, Div};
    size_t types_size = 2;
    while (match_token(peek(parser), types, types_size)) {
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
    TokenType* types = (TokenType[]) {Add, Sub};
    size_t types_size = 2;
    while (match_token(peek(parser), types, types_size)) {
        TokenType op = parser->tokens.current->type;
        advance_parer(parser);
        AstNode* right = term(parser);
        left = node_binary(op, left, right);
        skip_whitespace(parser);
    }

    return left;
}

// todo : expressions have this bug where unexpected chars will terminate the function
//      this makes it a pain when parsing conditionals, which will end unexpectedly,
//      we want the expression handler to be more robust,
//      and allow for use of more than just numbers
AstNode* relational_comparison(Parser* parser) {
    AstNode* left = expr(parser);
    skip_whitespace(parser);

    TokenType* types = (TokenType[]) {Le, Ge, Lt, Gt};
    size_t types_size = 4;
    while (match_token(peek(parser), types, types_size)) {
        TokenType op = parser->tokens.current->type;
        advance_parer(parser);
        AstNode* right = expr(parser);
        left = node_binary(op, left, right);
        skip_whitespace(parser);
    }
    return left;
}

AstNode* equality_comparison(Parser* parser) {
    AstNode* left = relational_comparison(parser);
    skip_whitespace(parser);

    TokenType* types = (TokenType[]) {Eq, Neq};
    size_t types_size = 2;
    while (match_token(peek(parser), types, types_size)) {
        TokenType op = parser->tokens.current->type;
        advance_parer(parser);
        AstNode* right = relational_comparison(parser);
        left = node_binary(op, left, right);
        skip_whitespace(parser);
    }
    return left;
}

AstNode* bitwise_and(Parser* parser) {
    AstNode* left = equality_comparison(parser);
    skip_whitespace(parser);

    while (peek(parser) == BitAnd) {
        TokenType op = parser->tokens.current->type;
        advance_parer(parser);
        AstNode* right = equality_comparison(parser);
        left = node_binary(op, left, right);
        skip_whitespace(parser);
    }

    return left;
}

AstNode* bitwise_xor(Parser* parser) {
    AstNode* left = bitwise_and(parser);
    skip_whitespace(parser);

    while (peek(parser) == BitXor) {
        TokenType op = parser->tokens.current->type;
        advance_parer(parser);
        AstNode* right = bitwise_and(parser);
        left = node_binary(op, left, right);
        skip_whitespace(parser);
    }

    return left;
}

AstNode* bitwise_or(Parser* parser) {
    AstNode* left = bitwise_xor(parser);
    skip_whitespace(parser);

    while (peek(parser) == BitOr) {
        TokenType op = parser->tokens.current->type;
        advance_parer(parser);
        AstNode* right = bitwise_xor(parser);
        left = node_binary(op, left, right);
        skip_whitespace(parser);
    }

    return left;
}

AstNode* logical_and(Parser* parser) {
    AstNode* left = bitwise_or(parser);
    skip_whitespace(parser);

    while (peek(parser) == And) {
        TokenType op = parser->tokens.current->type;
        advance_parer(parser);
        AstNode* right = bitwise_or(parser);
        left = node_binary(op, left, right);
        skip_whitespace(parser);
    }

    return left;
}

AstNode* logical_or(Parser* parser) {
    AstNode* left = logical_and(parser);
    skip_whitespace(parser);

    while (peek(parser) == Or) {
        TokenType op = parser->tokens.current->type;
        advance_parer(parser);
        AstNode* right = logical_and(parser);
        left = node_binary(op, left, right);
        skip_whitespace(parser);
    }

    return left;
}

AstNode** scope(Parser* parser) {
    consume_token(parser, Lcurly);
    AstNode** statements = malloc(sizeof(AstNode*));
    size_t max_statements = 1;
    size_t total_statements = 0;

    while (peek(parser) != Rcurly) {
        if (total_statements == max_statements) {
            max_statements *= 2;
            statements = realloc(statements, sizeof(AstNode*) * max_statements);
        }
        AstNode* stmt = statement(parser);
        statements[total_statements++] = stmt;
    }
    statements[total_statements] = NULL;

    consume_token(parser, Rcurly);
    return statements;
}

AstNode* if_statement(Parser* parser) {
    consume_token(parser, If);

    consume_token(parser, Lparen);
    AstNode* condition = logical_or(parser);
    consume_token(parser, Rparen);

    AstNode** body = scope(parser);

    if (peek(parser) == Else) {
        consume_token(parser, Else);
        AstNode** else_body = scope(parser);
        return node_if_statement(condition, body, else_body);
    }
    return node_if_statement(condition, body, NULL);
}

AstNode* let_statement(Parser* parser) {
    consume_token(parser, Let);
    Token* ident = consume_token(parser, Ident);
    consume_token(parser, Assign);
    AstNode* expression = logical_or(parser);
    consume_token(parser, Semicolon); // end of assignment
    return node_assignment(ident->content, expression);
}

AstNode* while_loop(Parser* parser) {
    consume_token(parser, While);
    consume_token(parser, Lparen);
    AstNode* condition = logical_or(parser);
    consume_token(parser, Rparen);

    AstNode** body = scope(parser);
    return node_while_loop(condition, body);
}

AstNode* function(Parser* parser) {
    consume_token(parser, Fn);
    Token* name = consume_token(parser, Ident);
    consume_token(parser, Lparen);
    AstNode** params = malloc(sizeof(AstNode*));
    size_t max_params = 1;
    size_t total_params = 0;
    while (peek(parser) != Rparen) {
        // todo : add types for params later

        Token* param = consume_token(parser, Ident);
        if (total_params == max_params) {
            max_params *= 2;
            params = realloc(params, sizeof(AstNode*) * max_params);
        }
        params[total_params++] = node_ident(param->content);
        if (peek(parser) == Rparen) { break; }
        consume_token(parser, Comma);
    }
    if (!total_params) { params = NULL; }
    consume_token(parser, Rparen);
    AstNode** body = scope(parser);
    AstNode* func = node_function(name->content, false, total_params, params, body);
    return func;
}

AstNode* var_update(Parser* parser) {
    Token* ident = consume_token(parser, Ident);
    TokenType *types = (TokenType[]){
        Assign, CompAdd, CompSub,
        CompMul, CompDiv, CompMod,
        CompLshift, CompRshift,
        CompBitAnd, CompBitXor,
        CompBitOr, CompBitNot
    };
    size_t types_size = 12;
    TokenType assign_op = consume_token_variants(parser, types, types_size)->type;
    AstNode* expression = logical_or(parser);
    TokenType op;

    switch (assign_op) {
        case Assign:
            return node_assignment(ident->content, expression);
        case CompAdd:
            op = Add;
            break;
        case CompSub:
            op = Sub;
            break;
        case CompMul:
            op = Mul;
            break;
        case CompDiv:
            op = Div;
            break;
        case CompMod:
            op = Mod;
            break;
        case CompLshift:
            op = Lshift;
            break;
        case CompRshift:
            op = Rshift;
            break;
        case CompBitAnd:
            op = BitAnd;
            break;
        case CompBitOr:
            op = BitOr;
            break;
        case CompBitXor:
            op = BitXor;
            break;
        case CompBitNot:
            op = BitNot;
            break;
        default:
            /*
            does nothing, errors should be caught
            when assign_op is parsed.
            */;
    }

    expression = node_binary(op, node_ident(ident->content), expression);

    consume_token(parser, Semicolon);
    return node_assignment(ident->content, expression);
}

AstNode* statement(Parser* parser) {
    skip_whitespace(parser);
    switch (parser->tokens.current->type) {
        case Let:
            return let_statement(parser);
        case Ident:
            return var_update(parser);
        case If:
            return if_statement(parser);
        case While:
            return while_loop(parser);
        case Fn:
            return function(parser);
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
        root->data.root.items[root->data.root.used++] = stmt;
    }
    return root;
}

Parser parse(TokenList tokens) {
    Parser parser;
    parser.code = 0;
    parser.tokens = tokens;
    parser.ast = build_tree(&parser);
    return parser;
}