#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"

int main(void) {
    char* test = "1+2*3-4";
    TokenList tokens = tokenize(test);
    Parser parser = parse(tokens);
    printf("src: %s\n", test);
    print_ast(parser.ast, 0);
    return 0;
}
