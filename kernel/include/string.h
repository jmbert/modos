

#ifndef _STRING_H
#define _STRING_H

#include <compiler.h>
#include <stddef.h>

int strlen(char *str);

void *memcpy(void *dest, void *src, size_t n);

#endif