#ifndef ARRAY_H_DEFINED
#define ARRAY_H_DEFINED


#include <stddef.h>
#include <stdarg.h>



typedef struct {
	void *(*new)(void *dummy, ...);
	void (*delete)(void *this);
} ArrayClass;


typedef struct {
	void **ptr;
	size_t size, allocated;
	ArrayClass *class;
} Array;


/**
 * Create an empty Array with the given class.
 */
Array *Array_new(ArrayClass *class);

/**
 * Clear the Array and free it.
 */
void Array_delete(Array *this);


/**
 * Delete every elements referenced by the Array and free the pointer.
 */
void Array_clear(Array *this);


/**
 * Allocate as much memory as needed to store <target> elements.
 */
void Array_max_resize(Array *this, size_t target);

/**
 * Reduce the size of the array to a minimum, unallocating as much unused
 * memory as possible.
 */
void Array_min_resize(Array *this);


/**
 * Push an existing item at the end of the array.
 */
void **Array_push(Array *this, void *item);

/**
 * Return the last item of the array after unreferencing it.
 */
void *Array_pop(Array *this);



#endif /* ARRAY_H_DEFINED */

