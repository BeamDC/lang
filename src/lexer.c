#include<stdlib.h>
#include<string.h>


/*
 * Example Use:
 *
 * unsigned char lut[256] = {0};
 * make_lut(lut, "hello", sizeof(lut));
 * lut['h'] == 1; // 1
 *
 */
void make_lut(unsigned char* lut, const char* accepted_chars, size_t lut_size) {
    // Initialize all entries to false
    memset(lut, 0, lut_size);

    // Set entries for accepted characters to true
    while (*accepted_chars) {
        lut[(unsigned char)*accepted_chars] = 1;
        accepted_chars++;
    }
}