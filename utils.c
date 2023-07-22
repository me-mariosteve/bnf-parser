#include "utils.h"



void *xmalloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return ptr;
}

void *xrealloc(void *ptr, size_t size) {
	ptr = realloc(ptr, size);
	if (ptr == NULL) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	return ptr;
}

