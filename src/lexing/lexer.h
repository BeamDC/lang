#ifndef LANG_LEXER_H
#define LANG_LEXER_H

#include "token.h"
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "token.h"

Token next_token(char** input);
TokenList tokenize(char* input);

#endif