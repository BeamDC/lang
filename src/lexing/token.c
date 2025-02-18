#include "token.h"

char* token_to_string(TokenType t) {
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
        "Add", "CompAdd",
        "Sub", "CompSub",
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
        "Xor",
        "Or",

        // Code Symbols
        "Function",
        "Comment",

        // Keywords
        "Fn",
        "Bool", "True", "False",
        "Let", "Const",
        "If", "Else",
        "For", "In",
        "While", "Loop",
        "Null",
        "Return",

        "isize", "usize",

        "int_8", "int_16", "int_32", "int_64",
        "unsigned_8", "unsigned_16", "unsigned_32", "unsigned_64",

        "float_16", "float_32", "float_64", "float_128",

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

TokenType get_ident_or_keyword(char** input, const char* start) {
    size_t len = *input - start;
    char* word = malloc(len + 1);
    strncpy(word, start, len);
    word[len] = '\0';

    static const char* strings[] = {
        "fn",
        "bool", "true", "false",
        "let", "const",
        "if", "else",
        "for", "in",
        "while", "loop",
        "none",
        "return",

        "isize", "usize",

        "i8", "i16", "i32", "i64",
        "u8", "u16", "u32", "u64",

        "f16", "f32", "f64", "f128",
    };

    static const TokenType keywords[] = {
        Fn,
        Bool, True, False,
        Let, Const,
        If, Else,
        For, In,
        While, Loop,
        None,
        Return,

        isize, usize,

        i8, i16, i32, i64,
        u8, u16, u32, u64,

        f16, f32, f64 , f128,
    };

    for (size_t i = 0; i < sizeof(strings)/sizeof(char*); ++i) {
        if (!strcmp(strings[i], word)) {
            free(word);
            return keywords[i];
        }
    }
    free(word);
    return Ident;
}

void print_token_list(TokenList tl) {
    for (int i = 0; i < tl.len; ++i) {
        Token tok = tl.current[i];
        // skip whitespace
        if (tok.type == Whitespace) { continue; }
        printf("__________________________________________________\n");
        printf("| Type : %-39s |\n", token_to_string(tok.type));
        printf("| Content : %-36s |\n", tok.content);
        printf("| line : %-39llu |\n", tok.line);
        printf("| col : %-40llu |\n", tok.col);
    }
    printf("__________________________________________________\n");
}