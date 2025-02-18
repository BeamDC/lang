#ifndef LANG_LEXER_H
#define LANG_LEXER_H

#include "token.h"

Token next_token(char** input);
TokenList tokenize(char* input);

#endif