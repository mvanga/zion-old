#ifndef ZION_STRING_H
#define ZION_STRING_H

char *strcpy(char *, char *);
int strlen(char *);
int strnlen(char *, int);
void *memcpy(void *, void *, int);
void * memset(void *, int, int);
int strncmp(const char *, const char *, int);
char *strchr(const char *, int);

#endif
