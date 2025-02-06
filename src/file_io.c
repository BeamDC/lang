#include "file_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StringView file_read(const char* path) {
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        fprintf(stderr, "File could not be opened: %s", path);
        exit(1); // todo : return empty view with error message instead
    }
    StringView str;
    size_t size = 1;
    str.length = 0;
    str.string = malloc(sizeof(char));
    char ch;
    while((ch=fgetc(f)) != EOF) {
        if (str.length == size) {
            size *= 2;
            str.string = realloc(str.string, size * sizeof(char));
        }
        str.string[str.length++] = ch;
    }
    // cut off at end
    str.string[str.length] = 0;
    return str;
}
