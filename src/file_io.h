#ifndef FILE_IO_H
#define FILE_IO_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char* string;
  size_t length;
} StringView;

StringView file_read(const char* path);

#endif //FILE_IO_H
