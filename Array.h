#ifndef ARRAY_H_DEFINED
#define ARRAY_H_DEFINED


#include <stddef.h>
#include <stdarg.h>



typedef void (*generic_proc_t) (void *);

typedef struct {
	/**
	 * The space required to allocate one value of this class.
	 * Like sizeof(), but user defined.
	 */
	const size_t size;
	
	/**
	 * Below are function pointers to user defined procedures.
	 * Their first parameter is a pointer to a allocated and initialized
	 * memory; they may take zero or any additional parameters.
	 */

	/**
	 * Initialize unitialized data. The optional arguments are user-defined.
	 */
	void (*new) (void *, ...);

	/**
	 * Unitialize structure.
	 */
	void (*delete) (void *);

	/**
	 * Unitialize structure, and data referenced by it.
	 * May be some recursive deletion procedure for example.
	 */
	void (*deep_delete) (void *);

	/**
	 * Copy data.
	 */
	void (*copy) (const void *src, void *dest);

	/**
	 * Recursively copy all of the referenced data.
	 */
	void (*deep_copy) (const void *src, void *dest);

} Class;


typedef struct {
	/* A pointer to the allocated memory */
	void *ptr;
	/* The amount of elements stored */
	size_t size;
	/* The amount of elements that can be stored */
	size_t allocated;
	/* The class of the Array items */
	const Class *class;
} Array;


/**
 * Create an Array of empty length with the given class.
 */
Array *Array_new(const Class *class);

/**
 * Clear the Array and free it.
 */
void Array_delete(Array *this);


/**
 * Return a pointer to the element at position idx.
 */
void *Array_at(const Array *, size_t idx);

/**
 * Resize the Array to the targeted size. If it is not enough to keep existing
 * items, do nothing.
 */
void Array_resizeTo(Array *, size_t target);

/**
 * Like Array_resizeTo, except that it converges by gradually doubling or
 * halving the size.
 */
void Array_resizeNear(Array *, size_t target);


/**
 * Call a procedure on each item of the array from the first to the last.
 */
void Array_forEach(Array *, void (*proc) (void *item));

/**
 * Delete every elements referenced by the Array.
 */
void Array_clear(Array *);

/**
 * Copy the item at the end of the Array.
 */
void *Array_push(Array *this, const void *item);

/**
 * Return the last item of the array and decrement the array size by one.
 */
void *Array_pop(Array *this);



#endif /* ARRAY_H_DEFINED */

