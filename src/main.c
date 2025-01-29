#include <stdio.h>
#include "lexer.h"

int main(void) {
    char* test = "1+2 ?im a comment\nHello";
    printf("src: %s\n", test);
    TokenList tokens = tokenize(test);
    printf("%llu tokens in total\n",tokens.len);
    Token* toks = tokens.tokens;
    for (int i = 0; i < tokens.len; ++i) {
        printf("================\n");
        printf("tok_t: %s\n", token_to_string(toks[i].type));
        printf("tok_c: \'%s\'\n", toks[i].content);
        printf("================\n");
    }
    return 0;
}
