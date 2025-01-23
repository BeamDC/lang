#include<stdlib.h>
#include<string.h>
#include "token.h"

Token next_token(char** input) {

}

Token* tokenize(char* input) {
    // call next token until eof
    // pass pointer by reference (char*) this will allow
    // changes made by next_token to remain after it is called.
    // ie. next_token(&input)
    // it can then be dereferenced in next_token
}