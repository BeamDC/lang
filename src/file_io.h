#ifndef FILE_IO_H
#define FILE_IO_H
#include <stddef.h>

typedef struct {
  char* string;
  size_t length;
} StringView;

StringView file_read(const char* path);

#endif //FILE_IO_H
