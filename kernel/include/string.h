

#ifndef _STRING_H
#define _STRING_H

#include <compiler.h>
#include <stddef.h>

int strlen(char *str);

void *memcpy(void *dest, void *src, size_t n);

int memcmp(void *p1, void *p2, size_t n);

int strcmp(void *dest, void *src);

char *strrchr(char *s, char c);

void *memchr(void *ptr, int value, size_t num);

void *memrchr(void *s, int c, size_t n);

char *strtok(char *s, char *delim);

#endif