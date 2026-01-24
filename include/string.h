#ifndef STRING_H
#define STRING_H

typedef unsigned long size_t;

size_t strlen(const char *str);
void *memset(void *dest, int val, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strcpy(char *dest, const char *src);
char *strchr(const char *s, int c);

#endif