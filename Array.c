#include "Array.h"
#include "utils.h"


#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>



Array *Array_new(ArrayClass *class) {
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



void Array_clear(Array *this) {
	if (this->ptr) {
		if (this->size > 0) {
			for (size_t i = 0; i < this->size; i++) {
				this->class->delete(this->ptr[i]);
			}
		}
		free(this->ptr);
		this->ptr = NULL;
	}
}



void Array_max_resize(Array *this, size_t target_size) {
	if (target_size	<= this->allocated) {
		return;
	}
	size_t new_size = this->allocated || 1;
	while (new_size < target_size) {
		new_size <<= 1;
	}
	this->ptr = xreallocarray(this->ptr, sizeof(void *), new_size);
	this->allocated = new_size;
}


void Array_min_resize(Array *this) {
	if (this->allocated == this->size) {
		return;
	}
	size_t new_size = this->size;
	while (new_size>>1 > this->size) {
		new_size >>= 1;
	}
	this->ptr = xreallocarray(this->ptr, sizeof(void *), new_size);
	this->allocated = new_size;
}



void **Array_push(Array *this, void *item) {
	if (this->allocated < this->size+1) {
		Array_max_resize(this, this->size+1);
	}
	this->ptr[this->size] = item;
	this->size++;
	return &this->ptr[this->size-1];
}


void *Array_pop(Array *this) {
	assert(this->size > 0);
	void *item = this->ptr[this->size-1];
	free(&this->ptr[this->size-1]);
	this->size--;
	return item;
}

