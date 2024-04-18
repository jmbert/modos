
#include <string.h>


int strlen(char *str) {
	int i = 0;
	for (; str[i] != '\0'; i++);
	return i;
}

void *memcpy(void *dest, void *src, size_t n) {
	for (size_t i = 0; i < n; i++){
		((char*)(dest))[i] = ((char*)(src))[i];
	}
	return dest;
}