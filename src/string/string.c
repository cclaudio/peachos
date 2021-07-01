/*
 * String
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#include <stddef.h>

#include "string.h"

size_t strlen(const char *str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

size_t strnlen(const char *str, size_t max)
{
	size_t len = 0;
	while (str[len]) {
        if (len == max)
            return max;
		len++;
    }

	return len;
}

char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while (*src != 0) {
		*dest = *src;
		src++;
		dest++;
	}
	*dest = 0x00;

	return tmp;
}

int to_numeric_digit(char c)
{
    return c - 48;
}

bool is_digit(char c)
{
    return c >= 48 && c <= 57;
}