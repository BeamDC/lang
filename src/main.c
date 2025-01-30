#include <stdio.h>
#include "lexer.h"
#include "parser.h"

int main(void) {
    char* test = "4/(1/10)"; // 16
    printf("src: %s\n", test);
    TokenList tokens = tokenize(test);
    // print tokens to stdout
    // printf("%llu tokens in total\n",tokens.len);
    // Token* toks = tokens.current;
    // for (int i = 0; i < tokens.len; ++i) {
    //     printf("================\n");
    //     printf("tok_t: %s\n", token_to_string(toks[i].type));
    //     printf("tok_c: \'%s\'\n", toks[i].content);
    //     printf("================\n");
    // }
    // parse the tokens and get the result
    Parser parser;
    parser.tokens = tokens;
    parser.error_occurred = false;
    double res = parse(&parser);
    printf("Parsing Result : %f", res);
    return 0;
}
