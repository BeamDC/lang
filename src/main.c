#include <stdio.h>
#include "lexer.h"

int main(void) {
    char* test = "  test1 23";
    Token t = next_token(&test);
    while (t.type != Eof) {
        printf("================\n");
        printf("tok_t: %s\n", to_string(t.type));
        printf("tok_c: \'%s\'\n", t.content);
        printf("================\n");
        t = next_token(&test);
    }
    return 0;
}
