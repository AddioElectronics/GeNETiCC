#ifndef GENETICC_LIST_DEFS_H_
#define GENETICC_LIST_DEFS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

#include "Addio/GeNETiCC/Collections/array/array_geneticc_defs.h"



typedef struct
{
	capacity_size_t capacity;	//The total size of the array (in bytes).
	type_size_t elem_size;		//The size of the value's type (in bytes).
	byte_offset_t tail;			//The byte offset to the first item in the array.
	byte_offset_t head;			//The byte offset to the element after the last item in the array. If head == tail, there are no elements in the array.
	//size_t size;					//The size of the populated elements as bytes.
	//length_t count;				//The amount of populated elements in the list.
	ARRAY_PTR array;			//Pointer to the start of the array
	//type_t type;				//The type of elements stored in the array.
}list_t;


#define LIST_PTR		list_t*



#endif /* GENETICC_LIST_DEFS_H_ */
