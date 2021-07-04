/*
 * String
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#include <stddef.h>

#include "string.h"

char tolower(char s1)
{
	/*
	 * A == 65, Z == 90
	 * a == 97, gap of 32 units
	 */
	if (s1 >= 65 && s1 <= 90)
		s1 += 32;

	return s1;
}

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

int strnlen_terminator(const char *str, int max, char terminator)
{
	int i;

	for (i = 0; i < max; i++) {
		if (str[i] == '\0' || str[i] == terminator)
			break;
	}

	return i;
}

int strncmp(const char *str1, const char *str2, size_t n)
{
	unsigned char u1, u2;

	while (n-- > 0) {
		u1 = (unsigned char) *str1++;
		u2 = (unsigned char) *str2++;
		if (u1 != u2)
			return u1 - u2;
		if (u1 == '\0')
			return 0;
	}

	return 0;
}

int istrncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char u1, u2;

	while (n-- > 0) {
		u1 = (unsigned char) *s1++;
		u2 = (unsigned char) *s2++;
		if (u1 != u2 && tolower(u1) != tolower(u2))
			return u1 - u2;
		if (u1 == '\0')
			return 0;
	}

	return 0;
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

char *strncpy(char *dest, const char *src, size_t count)
{
	int i;

	for (i = 0; i < count-1; i++) {
		if (src[i] == '\0')
			break;
		dest[i] = src[i];
	}
	dest[i] = '\0';

	return dest;
}

int to_numeric_digit(char c)
{
    return c - 48;
}

bool is_digit(char c)
{
    return c >= 48 && c <= 57;
}