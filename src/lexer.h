#ifndef LANG_LEXER_H
#define LANG_LEXER_H

#include<stdlib.h>
#include "token.h"

Token next_token(const char* input, int pos);
Token* tokenize(char* input);

#endif