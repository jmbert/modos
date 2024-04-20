
#include <string.h>
#include <stdint.h>

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


int memcmp(void *p1, void *p2, size_t n) {
	for (size_t i = 0; i < n; i++) {
		int8_t p1_c = ((int8_t*)(p1))[i];
		int8_t p2_c = ((int8_t*)(p2))[i];
		if (p1_c != p2_c) {
			return p1_c - p2_c;
		}
	}
	return 0;
}