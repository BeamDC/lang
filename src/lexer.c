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

// make a token of specified type
// token size is computed by the comparison function provided
Token make_token(char** input, const char* start, enum TokenType t, bool(*f)(char)) {
    Token tok;
    tok.type = t;

    while(f(**input)) { (*input)++; }

    size_t length = *input - start;
    tok.content = (char*)malloc(length + 1);
    strncpy(tok.content, start, length);
    tok.content[length] = '\0';

    return tok;
}

Token make_numeric(char** input) {
    char* start = *input;
    // todo : allow for float values
    Token tok = make_token(input, start, Numeric, is_digit);
    return tok;
}

Token make_ident(char** input) {
    char* start = *input;
    // todo : allow checking for keywords
    Token tok = make_token(input, start, Ident, is_ident_char);
    return tok;
}

// gets the longest string of whitespace from current pos
Token make_whitespace(char** input) {
    char* start = *input;
    Token tok = make_token(input, start, Whitespace, is_whitespace);
    return tok;
}

Token next_token(char** input) {
    if (is_digit(**input)) { return make_numeric(input); }
    if (is_alpha(**input)) { return make_ident(input);   }
    if (is_whitespace(**input)) { return make_whitespace(input); }

    // defualt
    Token t;
    t.type = Eof;
    t.content = "";
    return t;
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