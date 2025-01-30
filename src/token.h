#ifndef LANG_TOKEN_H
#define LANG_TOKEN_H

#include<stdbool.h>
#include <stddef.h>


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
    Add, Inc, CompAdd,
    Sub, Dec, CompSub,
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

static char* token_to_string(TokenType t) {
    static char* strings[] = {
        // Single char
        "Comma",
        "Colon",
        "Semicolon",
        "Pound",
        "Dollar",
        "At",
        "Lparen",
        "Rparen",
        "Lsquare",
        "Rsquare",
        "Lcurly",
        "Rcurly",
        "Langled",
        "Rangled",
        "Assign",

        // Uncategorized Ops
        "Dot", "Range",

        // Mathematical
        "Add", "Inc", "CompAdd",
        "Sub", "Dec", "CompSub",
        "Mul", "Exp", "CompMul",
        "Div", "CompDiv",
        "Mod", "CompMod",

        // Bitwise
        "Lshift", "CompLshift",
        "Rshift", "CompRshift",
        "BitAnd", "CompBitAnd",
        "BitOr", "CompBitOr",
        "BitXor", "CompBitXor",
        "BitNot", "CompBitNot",

        // Logical
        "Bang",
        "Neq",
        "Eq",
        "Gt",
        "Lt",
        "Ge",
        "Le",
        "And",
        "Or",

        // Code Symbols
        "Function",
        "Comment",

        // Keywords
        "Fn",
        "Bool", "True", "False",
        "Let", "Const",
        "If", "Else",
        "For", "While", "Loop",
        "Null",
        "Return",

        // Literals
        "Ident",
        "String",
        "Char",
        "Numeric",

        // Misc
        "Arrow",
        "Error", "Eof", "Whitespace", "Unkown",
    };

    return strings[t];
}

static bool is_kword(char* s) {
    static const char* strings[] = {
        "Fn",
        "Bool", "True", "False",
        "Let", "Const",
        "If", "Else",
        "For", "While", "Loop",
        "Null",
        "Return",
    };
}

// todo : add row and column data for error handling
typedef struct {
    TokenType type;
    char* content;
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

#endif