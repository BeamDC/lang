#include "lexer.h"

// tracking lexer pos
size_t line = 1;
size_t col = 1;

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
    return is_in_str(c, ".+-*/%<>&|^!=~");
}

char peek_char(char** input) {
    if (*input + 1 == NULL) {
        fprintf(stderr, "Peeked at NULL! *arc of covenant face melting*");
        exit(1);
    }
    return *(*input + 1);
}

// make a token of specified type
// token size is computed by the comparison function provided
Token make_token(char** input, const char* start, TokenType type) {
    Token tok;
    tok.type = type;

    size_t len = *input - start;
    tok.content = (char*)malloc(len + 1);
    strncpy(tok.content, start, len);
    tok.content[len] = '\0';
    tok.line = line;
    tok.col = col;

    col += len;

    return tok;
}

Token make_token_single_char(char** input, TokenType type) {
    char* start = *input;
    (*input)++;
    return make_token(input, start, type);
}

Token make_token_with_variants(char** input, const char** variants, TokenType* types, TokenType otherwise)
{
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
    *input += max_len == 0 ? 1 : max_len;
    return make_token(input, start, current_type);
}

Token make_error_token(char** input, char* message) {
    // todo : later we will have to determine the error and display a message
    // in addition we also need to determine where code becomes legible again
    // for example in 8+??2-4 gives the error "??2-4", when it should be "??"
    char* start = *input;
    while(!is_whitespace(**input) && **input!='\0') { (*input)++; }
    return make_token(input, start, Error);
}

Token make_numeric(char** input) {
    char* start = *input;
    while(is_digit(**input)) { (*input)++; }
    // check for decimals or range
    char peeked = peek_char(input);
    // todo : this needs a fix
    //      it should be peeking twice, as current
    //      will be added to the token.
    //      proper structure would be to peek and peek next
    if (**input == '.' && peeked == '.') { // range
        Token tok = make_token(input, start, Numeric);
        return tok;
    }

    if (**input == '.') {
        (*input)++;
        while (is_digit(**input)) {
            (*input)++;
        }
    }
    Token tok = make_token(input, start, Numeric);
    return tok;
}

Token make_ident(char** input) {
    char* start = *input;
    while(is_ident_char(**input)) { (*input)++; }
    TokenType type = get_ident_or_keyword(input, start);
    Token tok = make_token(input, start, type);
    return tok;
}

Token make_whitespace(char** input) {
    char* start = *input;
    while(is_whitespace(**input)) {
        if (**input == '\n') {
            line++;
            col = 0;
        }
        (*input)++;
    }
    Token tok = make_token(input, start, Whitespace);
    return tok;
}

Token make_comment(char** input) {
    char * start = *input;
    while(**input != '\n') { (*input)++; }
    return make_token(input, start, Comment);
}

Token make_operator(char** input) {
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
                    (const char*[]) { "+=", NULL },
                    (TokenType[]) { CompAdd },
                    Add
            );
            break;
        case '-':
            tok = make_token_with_variants(
                    input,
                    (const char*[]) { "-=", "->", NULL},
                    (TokenType[]) { CompSub, Arrow },
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

Token make_string(char** input) {
    // consume first "
    (*input)++;
    char* start = *input;
    while(**input != '"' && **input != '\0') {
        (*input)++;
    }
    if (**input == '\0') {
        return make_error_token(&start, "Unterminated String");
    }
    Token tok = make_token(input, start, String);
    // move forward to full consume string
    (*input)++;
    return tok;
}

Token next_token(char** input) {
    if (is_alpha(**input)) { return make_ident(input); }
    if (is_digit(**input)) { return make_numeric(input); }
    if (is_operator(**input)) { return make_operator(input); }
    if (is_whitespace(**input)) { return make_whitespace(input); }

    Token tok;

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
        case '"':
            tok = make_string(input);
            break;
        case '?':
            tok = make_comment(input);
            break;
        case '\0':
            tok = make_token_single_char(input, Eof);
            break;
        default:
            // unexpected char error
            tok = make_error_token(input, "Unexpected Char");
            break;
    }

    return tok;
}

TokenList tokenize(char* input) {
    size_t size = 1;
    TokenList toks;
    toks.current = malloc(sizeof(Token));
    toks.len = 0;
    Token current;
    do {
        if (toks.len == size) {
            size *= 2;
            toks.current = realloc(toks.current, size * sizeof(Token));
        }
        current = next_token(&input);
        toks.current[toks.len++] = current;
    } while (current.type != Eof);
    return toks;
}
