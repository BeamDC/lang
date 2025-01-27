#include <stdio.h>
#include "lexer.h"

int main(void) {
    char* test = "A<<=B+++3/(n%8);";
    Token t;

    do {
        t = next_token(&test);
        printf("================\n");
        printf("tok_t: %s\n", to_string(t.type));
        printf("tok_c: \'%s\'\n", t.content);
        printf("================\n");
    } while (t.type != Eof && t.type != Error);

    return 0;
}
