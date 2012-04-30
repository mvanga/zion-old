#ifndef ZION_STDLIB_H
#define ZION_STDLIB_H

int atoi(const char* s);
unsigned long strtoul(const char *cp,char **endp,unsigned int base);
long strtol(const char *cp,char **endp,unsigned int base);
int ustrtoul(const char *cp, char **endp, unsigned int base);

#define NULL ((void *)0)

#endif
