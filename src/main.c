#include "lexer.h"
#include <stdio.h>
#include "parser.h"
#include "ast.h"
#include "file_io.h"

int main(void) {
    StringView buf = file_read("../test_code.txt");
    TokenList tokens = tokenize(buf.string);
    Parser parser = parse(tokens);

    printf("src:\n%s\n\n", buf.string);
    print_ast(parser.ast, 0);

    return 0;
}