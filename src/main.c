#include <stdio.h>
#include "lexer.h"

int main(void) {
    unsigned char lut[256] = {0};
    make_lut(lut, "Hello",  sizeof(lut));
    printf("%d\n", lut['H']); // 1
    printf("%d\n", lut['h']); // 0
    return 0;
}
