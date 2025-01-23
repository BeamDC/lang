#include "token.h"
#include "constants.h"
#include <stdbool.h>

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