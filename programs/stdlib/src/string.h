#ifndef PEACHOS_STRING_H
#define PEACHOS_STRING_H

#include <stddef.h>
#include <stdbool.h>

#include "string.h"

char tolower(char s1);
size_t strlen(const char *str);
size_t strnlen(const char *str, size_t max);
int strnlen_terminator(const char *str, int max, char terminator);
int strncmp(const char *str1, const char *str2, size_t n);
int istrncmp(const char *s1, const char *s2, size_t n);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t count);
int to_numeric_digit(char c);
bool is_digit(char c);
char *strtok(char *str, const char *delimiters);

#endif // PEACHOS_STRING_H