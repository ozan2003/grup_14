#ifndef SPLIT_WHITESPACE_H
#define SPLIT_WHITESPACE_H

#include <stdio.h>
#include <stdlib.h>

#define TOKEN_DELIMITERS " \t\r\n\a"
#define TOKEN_BUFFER_SIZE 32

char** split_whitespace(char* line);

#endif // SPLIT_WHITESPACE_H