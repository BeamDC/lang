#include "token.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

// char* precedence_to_string(Precedence prec) {
//     static char* strings[] = {
//         "Prec_None",
//         "Prec_Assignment",
//         "Prec_Or",
//         "Prec_And",
//         "Prec_Equality",
//         "Prec_Comparison",
//         "Prec_Term",
//         "Prec_Factor",
//         "Prec_Unary",
//         "Prec_Call",
//         "Prec_Primary"
//     };
//     return strings[prec];
// }

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
        "null",
        "return",
    };

    static const TokenType keywords[] = {
        Fn,
        Bool, True, False,
        Let, Const,
        If, Else,
        For, In,
        While, Loop,
        Null,
        Return,
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
        // printf("| Precedence : %-33s |\n", precedence_to_string(tok.precedence));
        printf("| line : %-39llu |\n", tok.line);
        printf("| col : %-40llu |\n", tok.col);
    }
    printf("__________________________________________________\n");
}