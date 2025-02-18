#include <stdio.h>

#include "lexing/lexer.h"
#include "parsing/parser.h"
#include "parsing/ast.h"
#include "file_io.h"

int main(void) {
    StringView buf = file_read("../src/test_code.txt");
    TokenList tokens = tokenize(buf.string);
    Parser parser = parse(tokens);

    printf("src:\n%s\n\n", buf.string);
    // print_token_list(tokens);
    print_ast(parser.ast, 0);

    return 0;
}