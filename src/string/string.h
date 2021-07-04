/*
 * String
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdbool.h>

size_t strlen(const char *str);
size_t strnlen(const char *str, size_t max);
int strnlen_terminator(const char *str, int max, char terminator);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t count);
int strncmp(const char *str1, const char *str2, size_t n);
int istrncmp(const char *s1, const char *s2, size_t n); // insensitive comparison
int to_numeric_digit(char c);
bool is_digit(char c);
char tolower(char s1);

#endif // STRING_H