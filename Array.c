#include "Array.h"
#include "utils.h"


#include <stdlib.h>
#include <assert.h>
#include <string.h>



Array *Array_new(const Class *class) {
	assert(class != NULL);
	Array *this = xmalloc(sizeof(Array));
	this->ptr = NULL;
	this->size = 0;
	this->allocated = 0;
	return this;
}


void Array_delete(Array *this) {
	if (!this) {
		return;
	}
	Array_clear(this);
	free(this);
}


void *Array_at(const Array *this, size_t idx) {
	return (char *) this->ptr + idx * this->class->size;
}


void Array_clear(Array *this) {
	if (!this->ptr) {
		return;
	}
	if (this->size > 0 && this->class->delete) {
		Array_forEach(this, (void (*) (void *)) this->class->delete);
	}
	free(this->ptr);
	this->ptr = NULL;
}



void Array_resizeTo(Array *this, size_t target) {
	if (target <= this->allocated) {
		return;
	}
	this->ptr = xreallocarray(this->ptr, this->class->size, target);
	this->allocated = target * this->class->size;
}


void Array_resizeNear(Array *this, size_t target) {
	if (this->allocated == target) {
		return;
	}
	size_t new_size = this->allocated;
	if (this->allocated < target) {
		while (new_size < target) {
			new_size <<= 1;
		}
	} else {
		if (target < this->size) {
			return;
		}
		while (new_size > target && new_size>>1 >= this->size) {
			new_size >>= 1;
		}
	}
	this->ptr = xreallocarray(this->ptr, sizeof(void *), new_size);
	this->allocated = new_size;
}



void *Array_push(Array *this, void const *item) {
	if (this->allocated < this->size+1) {
		Array_resizeNear(this, this->size+1);
	}
	void *end_ptr = Array_at(this, this->size);
	memmove(end_ptr, item, this->class->size);
	this->size++;
	return end_ptr;
}


void *Array_pop(Array *this) {
	assert(this->size > 0);
	this->size--;
	return Array_at(this, this->size);
}

