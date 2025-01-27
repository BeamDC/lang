#include<stdlib.h>
#include "token.h"
#include <stdbool.h>
#include <string.h>

// todo : potentially a better way to do this
bool is_in_str(const char c, const char* str) {
    int chars[256] = {0};

    while(*str) {
        chars[(unsigned char)*str] = 1;
        str++;
    }

    return chars[c];
}

bool is_digit(char c) {
    return is_in_str(c, "0123456789");
}

bool is_whitespace(char c) {
    return is_in_str(c, " \n\r\t\v\f");
}

bool is_alpha(char c) {
    return is_in_str(c, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

bool is_ident_char(char c) {
    return is_in_str(c, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789");
}

bool is_operator(char c) {
    return is_in_str(c, ".+-*/%<>&|^!=");
}

// make a token of specified type
// token size is computed by the comparison function provided
Token make_token(char** input, const char* start, TokenType type) {
    Token tok;
    tok.type = type;

    size_t length = *input - start;
    tok.content = (char*)malloc(length + 1);
    strncpy(tok.content, start, length);
    tok.content[length] = '\0';

    return tok;
}

Token make_token_single_char(char** input, TokenType type) {
    char* start = *input;
    (*input)++;
    return make_token(input, start, type);
}

Token make_token_with_variants(char** input, const char** variants, TokenType* types, TokenType otherwise) {
    // given a list of possible variants,
    // find the longest variant token that can be made with input,
    // if no matches are found, create a token with the otherwise type
    char* start = *input;
    size_t max_len = 0;
    TokenType current_type = otherwise;

    for (int i = 0; variants[i] != NULL; ++i) {
        size_t len = strlen(variants[i]);
        if (len > max_len && strncmp(*input, variants[i], len) == 0) {
            max_len = len;
            current_type = types[i];
        }
    }
    *input += (max_len == 0) ? 1 : max_len;
    return make_token(input, start, current_type);
}

Token make_numeric(char** input) {
    char* start = *input;
    // todo : allow for float values
    while(is_digit(**input)) { (*input)++; }
    Token tok = make_token(input, start, Numeric);
    return tok;
}

Token make_ident(char** input) {
    char* start = *input;
    // todo : allow checking for keywords
    while(is_ident_char(**input)) { (*input)++; }
    Token tok = make_token(input, start, Ident);
    return tok;
}

// gets the longest string of whitespace from current pos
Token make_whitespace(char** input) {
    char* start = *input;
    while(is_whitespace(**input)) { (*input)++; }
    Token tok = make_token(input, start, Whitespace);
    return tok;
}

Token make_operator(char**input) {
    Token tok;
    switch (**input) {
        case '.':
            tok = make_token_with_variants(
                input,
                (const char*[]) { "..", NULL },
                (TokenType[]) { Range },
                Dot
            );
            break;
        case '+':
            tok = make_token_with_variants(
                    input,
                    (const char*[]) { "++", "+=", NULL },
                    (TokenType[]) { Inc, CompAdd },
                    Add
            );
            break;
        case '-':
            tok = make_token_with_variants(
                    input,
                    (const char*[]) { "--", "-=", NULL},
                    (TokenType[]) { Dec, CompSub },
                    Sub
                    );
            break;
        case '*':
            tok = make_token_with_variants(
                    input,
                    (const char*[]) { "**", "*=",NULL},
                    (TokenType[]) { Exp, CompMul },
                    Mul
            );
            break;
        case '/':
            tok = make_token_with_variants(
                input,
                (const char*[]) { "/=", NULL },
                (TokenType[]) { CompDiv },
                Div
            );
            break;
        case '%':
            tok = make_token_with_variants(
                input,
                (const char*[]) { "%=", NULL },
                (TokenType[]) { CompMod },
                Mod
            );
            break;
        case '<':
            tok = make_token_with_variants(
                input,
                (const char*[]) { "<<=", "<=", "<<", NULL },
                (TokenType[]) { CompLshift, Le, Lshift },
                Lt
            );
            break;
        case '>':
            tok = make_token_with_variants(
                input,
                (const char*[]) { ">>=", ">=", ">>", NULL },
                (TokenType[]) { CompRshift, Ge, Rshift },
                Gt
            );
            break;
        case '&':
            tok = make_token_with_variants(
                input,
                (const char*[]) { "&&", "&=", NULL },
                (TokenType[]) { And, CompBitAnd },
                BitAnd
            );
            break;
        case '|':
            tok = make_token_with_variants(
                input,
                (const char*[]) { "||", "|=", NULL },
                (TokenType[]) { Or, CompBitOr },
                BitOr
            );
            break;
        case '^':
            tok = make_token_with_variants(
                input,
                (const char*[]) { "^=", NULL },
                (TokenType[]) { CompBitXor },
                BitXor
            );
            break;
        case '~':
            tok = make_token_with_variants(
                input,
                (const char*[]) { "~=", NULL },
                (TokenType[]) { CompBitNot },
                BitNot
            );
            break;
        case '!':
            tok = make_token_with_variants(
                input,
                (const char*[]) { "!=", NULL },
                (TokenType[]) { Neq },
                Bang
            );
            break;
        case '=':
            tok = make_token_with_variants(
                input,
                (const char*[]) { "==", NULL },
                (TokenType[]) { Eq },
                Assign
            );
            break;
        default: // todo : have a way to display cause of error
            tok = make_token_single_char(input, Error);
            break;
    }
    return tok;
}

Token next_token(char** input) {
    if (is_digit(**input)) { return make_numeric(input); }
    if (is_alpha(**input)) { return make_ident(input);   }
    if (is_whitespace(**input)) { return make_whitespace(input); }
    if (is_operator(**input)) { return make_operator(input); }

    Token tok;

    // string

    // char

    // comment

    // single char
    switch (**input) {
        case '(':
            tok = make_token_single_char(input, Lparen);
            break;
        case ')':
            tok = make_token_single_char(input, Rparen);
            break;
        case '[':
            tok = make_token_single_char(input, Lsquare);
            break;
        case ']':
            tok = make_token_single_char(input, Rsquare);
            break;
        case '{':
            tok = make_token_single_char(input, Lcurly);
            break;
        case '}':
            tok = make_token_single_char(input, Rcurly);
            break;
        case ';':
            tok = make_token_single_char(input, Semicolon);
            break;
        case ':':
            tok = make_token_single_char(input, Colon);
            break;
        case ',':
            tok = make_token_single_char(input, Comma);
            break;
        case '#':
            tok = make_token_single_char(input, Pound);
            break;
        case '$':
            tok = make_token_single_char(input, Dollar);
            break;
        case '@':
            tok = make_token_single_char(input, At);
            break;
        default: // todo : have a way to display cause of error
            tok = make_token_single_char(input, Error);
            break;
    }

    return tok;
}

Token* tokenize(char* input) {
    // call next token until eof
    // pass pointer by reference (char**) this will allow
    // changes made by next_token to remain after it is called.
    // ie. next_token(&input)
    // it can then be dereferenced in next_token
    // Token t = next_token(&input);
    // while (t.type != Eof) {
    //     // find a way to put these into a pointer
    //     t = next_token(&input);
    // }
}