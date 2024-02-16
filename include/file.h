#include "macro.h"
#include <stdio.h>

char *getExtension(const char *path);

char *path_basename(const char *path, int extension);

ssize_t getLine(char **line, size_t *size, BOOL *last_line, FILE *file);