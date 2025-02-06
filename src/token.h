#ifndef LANG_TOKEN_H
#define LANG_TOKEN_H

#include <stdio.h>


typedef enum {
    // Single char
    Comma,
    Colon,
    Semicolon,
    Pound,
    Dollar,
    At,
    Lparen,
    Rparen,
    Lsquare,
    Rsquare,
    Lcurly,
    Rcurly,
    Langled,
    Rangled,
    Assign,

    // Uncategorized Ops
    Dot, Range,

    // Mathematical
    Add, CompAdd,
    Sub, CompSub,
    Mul, Exp, CompMul,
    Div, CompDiv,
    Mod, CompMod,

    // Bitwise
    Lshift, CompLshift,
    Rshift, CompRshift,
    BitAnd, CompBitAnd,
    BitOr, CompBitOr,
    BitXor, CompBitXor,
    BitNot, CompBitNot,

    // Logical
    Bang,
    Neq,
    Eq,
    Gt,
    Lt,
    Ge,
    Le,
    And,
    Xor,
    Or,

    // Code Symbols
    Function,
    Comment,

    // Keywords
    Fn,
    Bool, True, False,
    Let, Const,
    If, Else,
    For, While, Loop,
    Null,
    Return,

    // Literals
    Ident,
    String,
    Char,
    Numeric,

    // Misc
    Arrow,
    Error, Eof, Whitespace, Unkown,
}TokenType;

// typedef enum {
//     Prec_None,
//     Prec_Assignment,  // = += etc.
//     Prec_Or,          // || |
//     Prec_Xor,         // ^^ ^
//     Prec_And,         // && &
//     Prec_Equality,    // == !=
//     Prec_Comparison,  // < > <= >=
//     Prec_Shift,       // << >>
//     Prec_Term,        // + -
//     Prec_Factor,      // * / %
//     Prec_Unary,       // ! -
//     Prec_Acess,       // .
//     Prec_Call,        // ()
//     Prec_Primary
// } Precedence;

// todo : add row and column data for error handling
typedef struct {
    TokenType type;
    char* content;
    // Precedence precedence;
    size_t line;
    size_t col;
} Token;

// todo : maybe change name to lexer, and put in lexer.h
typedef struct {
    Token* current;
    size_t len;
} TokenList;

typedef struct {
    int precedence;
    int operands;
} Operator;

void print_token_list(TokenList tl);
char* token_to_string(TokenType t);
TokenType get_ident_or_keyword(char** input, const char* start);

#endif