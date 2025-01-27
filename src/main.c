#include <stdio.h>
#include "lexer.h"

int main(void) {
    char* test = "3+2*1-4"; // = 3+2-4 = 5-4 = 1
    Token* tokens = tokenize(test);
    while (tokens->type != Eof) {
        printf("================\n");
        printf("tok_t: %s\n", token_to_string(tokens->type));
        printf("tok_c: \'%s\'\n", tokens->content);
        printf("================\n");
        tokens++;
    }
    return 0;
}
