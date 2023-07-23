#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);
void *xreallocarray(void *ptr, size_t nmemb, size_t size);

#endif /* UTILS_H_INCLUDED */

