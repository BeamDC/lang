#ifndef LANG_TOKEN_H
#define LANG_TOKEN_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
    For, In,
    While, Loop,
    None,
    Return,

    // Data Types
    // pointer
    isize, usize,

    // ints
    i8, i16, i32, i64,
    u8, u16, u32, u64,

    // floats
    f16, f32, f64 , f128,

    // Literals
    Ident,
    String,
    Char,
    Numeric,

    // Misc
    Arrow,
    Error, Eof, Whitespace, Unkown,
}TokenType;

typedef enum {
    t_None = 0, // also allows us to do if(datatype), could be useful idk

    // ints
    t_i8, t_i16, t_i32, t_i64,
    t_u8, t_u16, t_u32, t_u64,

    // floats
    t_f16, t_f32, t_f64 , t_f128,
} DataType;

typedef struct {
    TokenType type;
    char* content;
    DataType data;
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