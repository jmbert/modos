
#include <string.h>
#include <stdint.h>
#include <log.h>

int strlen(char *str) {
	int i = 0;
	for (; str[i] != '\0'; i++);
	return i;
}

void *memcpy(void *dest, void *src, size_t n) {
 	char *d = dest;
 	char *s = src;
 	while (n--) {
 		*d++ = *s++;
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

int strcmp(void *p1, void *p2) {
	for (size_t i = 0; ((int8_t*)(p1))[i] != '\0' || ((int8_t*)(p2))[i] != '\0'; i++) {
		int8_t p1_c = ((int8_t*)(p1))[i];
		int8_t p2_c = ((int8_t*)(p2))[i];
		if (p1_c != p2_c) {
			return p1_c - p2_c;
		}
	}
	return 0;
}

char *strrchr(char *s, char c) {
  	char *rtnval = 0;

  	do {
  	  	if (*s == c)
  		    rtnval = (char*) s;
  	} while (*s++);
  	return (rtnval);
}

void *memchr(void *ptr, int value, size_t num) {
	char *big = (char *)ptr;
	size_t n;
	for (n = 0; n < num; n++)
		if (big[n] == value)
			return (void *)&big[n];
	return NULL;
}
void *memrchr(void *s, int c, size_t n) {
	unsigned char *cp;

    	if (n != 0) {
		cp = (unsigned char *)s + n;
		do {
		    if (*(--cp) == (unsigned char)c)
			return((void *)cp);
		} while (--n != 0);
    	}
    	return((void *)0);
}


char *strtok(char *s, char *delim) {
	char *spanp;
	int c, sc;
	char *tok;
	static char *last;


	if (s == NULL && (s = last) == NULL)
		return (NULL);

	/*
	 * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
	 */
cont:
	c = *s++;
	for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
		if (c == sc)
			goto cont;
	}

	if (c == 0) {		/* no non-delimiter characters */
		last = NULL;
		return (NULL);
	}
	tok = s - 1;

	/*
	 * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	 * Note that delim must have one NUL; we stop if we see that, too.
	 */
	for (;;) {
		c = *s++;
		spanp = (char *)delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				last = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}

