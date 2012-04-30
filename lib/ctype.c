#include <zion/ctype.h>

inline int isdigit(int c)
{
	return c >= '0' && c <= '9';
}

inline int isxdigit(int c)
{
	return isdigit(c)
		|| (c >= 'a' && c <= 'f')
		|| (c >= 'A' && c <= 'F');
}

inline int isalpha(int c)
{
	return islower(c) || isupper(c);
}

inline int isalnum(int c)
{
	return isalpha(c) || isdigit(c);
}

inline int islower(int c)
{
    return c >= 'a' && c <= 'z';
}

inline int isupper(int c)
{
    return c >= 'A' && c <= 'Z';
}

inline int toupper(int c)
{
    if (!islower(c))
    	return c;
    return c - 'a' + 'A';
}

inline int tolower(int c)
{
	if (!isupper(c))
		return c;
	return c - 'A' + 'a';
}
