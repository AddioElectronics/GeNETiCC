#include "geneticc_list_internal_functions.h"

#include <utils_assert.h>
#include <stddef.h>

#include "../../geneticc_memory.h"
#include "../../array/internal/geneticc_array_internal_functions.h"

#pragma region INTERNAL Functions

/*
*	Allocates and Initializes a list
*
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	capacity	The size of the internal array (in bytes).
*
*	/Returns			Pointer to the newly allocated list.
*/
LIST_PTR internal_list_new( element_size_t elem_size, capacity_size_t capacity)
{
	LIST_PTR list = 0;
	list = malloc(sizeof(list_t));
	
	internal_list_init(list, elem_size, capacity);
	
	return list;
}

/*
*	Initializes a list
*
*	/param	out_list	(out)The list
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	capacity	The size of the internal array (in bytes).
*
*/
void internal_list_init(LIST_PTR out_list,  element_size_t elem_size, capacity_size_t capacity)
{
	list_t list;
	if(elem_size == 0)
	list.elem_size = 1;
	else
	list.elem_size = elem_size;
	
	list.capacity = 0;
	list.tail = 0;
	list.head = 0;
	list.array = 0;
	
	if(list.capacity == 0 || list.capacity != capacity)
		internal_list_ensure_capacity(&list, capacity);
		
	*out_list = list;
}

/*
*	Initializes a list from an already existing array.
*
*	/param	array				A pointer to the start of the array.
*	/param	size				The total capacity of the list(in bytes).
*	/param	elem_size			The size of the value's type (in bytes).
*	/param	populated_size		The size of the array (in bytes)	*If 0, this will use size.
*
*	/Returns					Pointer to the newly allocated list.
*/
LIST_PTR internal_array_to_list(ARRAY_PTR array, size_t size,  element_size_t elem_size, size_t populated_size)
{
	LIST_PTR list = malloc(sizeof(list_t));
	list->array = array;
	list->capacity = size;
	list->elem_size = elem_size;	
	list->tail = 0;
	
	if(populated_size != 0 && populated_size < size)
	list->head = populated_size;
	else
	list->head = size;
	
	list->array = realloc(list->array, size);
	
	return list;

}

/*
*	Initializes a list by copying an already existing array.
*	*Copies array to another location. Use Array_Init_List to use the original array. And remember to free the original array.
*
*	/param	array				A pointer to the start of the array.
*	/param	size				The total capacity of the list(in bytes).
*	/param	elem_size			The size of the value's type (in bytes).
*	/param	populated_size		The size of the array (in bytes)	*If 0, this will use size.
*
*	/Returns					Pointer to the newly allocated list.
*/
LIST_PTR internal_array_copyTo_list(ARRAY_PTR array, size_t size,  element_size_t elem_size, size_t populated_size)
{
	LIST_PTR list = malloc(sizeof(list_t));
	
	if(populated_size == 0)
		populated_size = size;
	
	uint8_t* newArray = malloc(size);
	memcpy(newArray, array, populated_size);
		
	internal_array_init_list(newArray, size, elem_size, populated_size, list);
	
	return list;
}


/*
*	Initializes a list from an already existing array.
*
*	/param	array				A pointer to the start of the array.
*	/param	size				The total size of the array(in bytes).
*	/param	elem_size			The size of the value's type (in bytes).
*	/param	capacity			The size of the internal array (in bytes).
*	/param	populated_size		The size of the used elements in the array (in bytes).
*	/param	out_list			(out)The list
*
*/
void internal_array_init_list(ARRAY_PTR array, size_t size,  element_size_t elem_size, size_t populated_size, LIST_PTR out_list)
{
	out_list->array = array;
	out_list->capacity = size;
	out_list->elem_size = elem_size;
	
	if(populated_size != 0 && populated_size < size)
	out_list->head = populated_size;
	else
	out_list->head = size;
}

/*
*	Initializes a list, and copies an already existing array to the list.
*
*	/param	array				A pointer to the start of the array.
*	/param	size				The total size of the array(in bytes).
*	/param	elem_size			The size of the value's type (in bytes).
*	/param	capacity			The size of the internal array (in bytes).
*	/param	populated_size		The size of the used elements in the array (in bytes).
*	/param	out_list			(out)The list
*
*/
void internal_array_copy_init_list(ARRAY_PTR array, size_t size,  element_size_t elem_size, size_t populated_size, LIST_PTR out_list)
{
	internal_list_init(out_list, elem_size, size);
	
	if(populated_size != 0 && populated_size < size)
	out_list->head = populated_size;
	else
	out_list->head = size;
	
	geneticc_memcpy(out_list->array, array, out_list->head);
}

/*
*	Allocates a new array and copies the elements from the list.
*
*	/param	list			Pointer to the list.
*
*	/returns				Pointer to the newly allocated array.
*/
ARRAY_PTR internal_list_toArray(LIST_PTR list)
{
	size_t size = internal_list_get_size(list);
	ARRAY_PTR array = malloc(size);
	
	geneticc_memcpy(array, internal_list_get_aligned_array(list), size);
	
	return array;
}


/*
*	Frees the list and its internal array from memory.
*
*	List_Delete(list)
*
*	/param		list			Pointer to the list.
*/
void GENOPTI_ATT_FORCE_INLINE internal_list_delete(LIST_PTR list)
{
	free(list->array);
	free(list);
}



/*
*	Initializes a list from an already existing array.
*
*	/param	list			Pointer to the list.
*	/param	min				The minimum capacity the list can be.
*
*	/returns				True if the capacity was equal to or greater than min. False if the capacity has reached its maximum.
*/
bool internal_list_ensure_capacity(LIST_PTR list, capacity_size_t min)
{
	if(min > ARRAY_MAX_SIZE)
		return false;
	
	if(list->capacity < min)
	{
		size_t newCapacity = list->capacity == 0 ? LIST_DEFAULT_CAPACITY * list->elem_size : LIST_INCREASE_CAPACITY(list->capacity, list->elem_size);
		
		if(newCapacity > ARRAY_MAX_SIZE) newCapacity = ARRAY_MAX_SIZE;
		if(newCapacity < min) newCapacity = min;
		
		if(list->capacity == 0)
		{
			list->array = malloc(newCapacity);
		}
		else
		{
			list->array = realloc(list->array, newCapacity);
		}
		list->capacity = newCapacity;		
	}
	return true;
}

/*
*	Sets the list's capacity, as long as it is not smaller than the internal array.
*
*	/param	list			The list
*	/param	newCapacity		The amount of elements the new capacity will be.
*
*	/returns				True if the capacity was changed, or if it did not need to be changed. False if the internal array is larger than newCapacity.
*/
bool internal_list_set_capacity(LIST_PTR list, capacity_count_t newCapacity)
{
	size_t new_size =  LIST_COUNT_TO_SIZE(newCapacity, list->elem_size);
	size_t current_size = internal_list_get_size(list);
	
	if(list->capacity == new_size)
		return true;
	
	if(list->head <= new_size)
	{
		goto SetCapacity;
	}
	else if(list->head > new_size && new_size >= current_size)
	{
		internal_list_realign_array(list);
		goto SetCapacity;
	}
	
	return false;
	
	SetCapacity:
	{
		list->capacity = new_size;
		list->array = realloc(list->array, list->capacity);
		return true;
	}	
}

/*
*	Get the list's capacity as a count, not size.
*
*	/param	list			Pointer to the list.
*
*	/returns				The amount of elements that can fit in the list.
*/
capacity_count_t __attribute__((__always_inline__)) internal_list_get_capacity(LIST_PTR list)
{
	return list->capacity * list->elem_size;
}

/*
*	Gets the total size of elements in the list.
*
*	/param	list			Pointer to the list.
*
*	/Returns	uint		Size of the elements in the list.
*/
size_t __attribute__((__always_inline__))  internal_list_get_size(LIST_PTR list)
{
	return  (list->head - list->tail);
}

/*
*	Gets the total count of elements in the list.
*
*	/param	list			Pointer to the list.
*
*	/Returns	uint		Count of the elements in the list.
*/
length_t __attribute__((__always_inline__))  internal_list_get_count(LIST_PTR list)
{
	return  (list->head - list->tail) *  list->elem_size;
}

/*
*	Moves the head, changing the size of the internal array.
*	*Positive value will add values, negative will remove values.
*	*Nothing will happen if the head will be less than the tail, or greater than the capacity.
*
*	/param	list			Pointer to the list.
*	/param	n				How many elements to add/remove.
*	/param	erase			Should the memory be set to 0?
*
*	/Returns	uint		The new size of the list.
*/
length_t internal_list_adjustHead(LIST_PTR list, int n, bool erase )
{
	size_t n_size = n * list->elem_size;
	
	ASSERT(n_size + list->head <= list->capacity || n_size + list->head >= list->tail);
	//if(n_size + list->head > list->capacity || n_size + list->head < list->tail)
		//return internal_list_get_count(list);		
	
	if(erase )
	{
		if(n > 0)
		memset(list->array + list->head, 0, n_size);
		else if(n < 0)
		memset(list->array + list->head + n_size, 0, n_size * -1);
	}
	list->head += n;
	
	return internal_list_get_count(list);	
}

/*
*	Moves the tail, changing the size of the internal array.
*	*Positive value will remove values, negative will add values.
*	*Nothing will happen if the tail will be less than zero, or greater than the head.
*
*	/param	list			Pointer to the list.
*	/param	n				How many elements to add/remove.
*	/param	erase			Should the memory be set to 0?
*
*	/Returns	uint		The new size of the list.
*/
length_t internal_list_adjustTail(LIST_PTR list, int n, bool erase )
{
	size_t n_size = n * list->elem_size;
	
	ASSERT(n_size + list->tail <= list->head || n_size + list->tail >= 0);
	//if(n_size + list->tail > list->head || n_size + list->tail < 0)
		//return internal_list_get_count(list);
	
	if(erase )
	{
		if(n > 0)
		memset(list->array + list->tail, 0, n_size);
		else if(n < 0)
		memset(list->array + list->tail + n_size, 0, n_size * -1);
	}
	
	list->tail += n;
	
	return internal_list_get_count(list);
}


/*
*	Aligns the internal array to have its head at index 0.
*
*	/param	list			Pointer to the list.
*/
void internal_list_realign_array(LIST_PTR list)
{
	//Array is already aligned.
	if(list->tail == 0) return;
	
	geneticc_memcpy(list->array, (list->array + list->tail), internal_list_get_size(list));
	
	list->head -= list->tail;
	list->tail = 0;
}

/*
*	Realigns the internal array, sets the capacity to the size of the array,
*	and then reallocates the array.
*
*	/param		list		Pointer to the list.
*/
void GENOPTI_ATT_FORCE_INLINE  internal_list_trimExcess(LIST_PTR list)
{
	internal_list_realign_array(list);
	internal_list_set_capacity(list, list->head);
}


/*
*	List Get Array
*
*	Returns a pointer to the start of first element in the array.
*	Does not realign or move any memory around.
*
*	/param		list	Pointer to the list. 
*
*	/returns			Pointer to the first element in the array.
*/
ARRAY_PTR GENOPTI_ATT_FORCE_INLINE internal_list_get_aligned_array(LIST_PTR list)
{
	return list->array + list->tail;
}

/*
*	Moves the array so the tail is at index 0.
*	Checks for index out of range, and adjusts the index relative to the tail.
*
*	/param		list		Pointer to the list.
*	/param		value		Value to set.
*	/param		index		Index being set to value.
*
*	/returns	bool		True if the value was set, or null if the index was out of range.
*/
bool internal_list_set_memory(LIST_PTR list, const ELEMENT_PTR value, index_t index)
{
	length_t length = internal_list_get_count(list);
	
	if(index > length)
	{
		//ASSERT(false);	//Index out of range
		return false;
	}
	
	memcpy(list->array + list->tail + (index * list->elem_size), value, list->elem_size);
	return true;
}

/*
*	Moves the array so the tail is at index 0.
*	Checks for index out of range, and adjusts the index relative to the tail.
*
*	/param		list		Pointer to the list.
*	/param		value		Value to set.
*	/param		index		Index being set to value.
*
*	/returns	bool		True if the value was set, or null if the index was out of range.
*/
bool internal_list_set_generic(LIST_PTR list, generic_union_t value, index_t index)
{
	uint64_t v = value.u64;	//Store value so we can get pointer from.
	
	return internal_list_set_memory(list, &v, index);
}

/*
*	Returns a pointer to the value at the index.
*	Checks for index out of range, and adjusts the index relative to the tail.
*
*	/param		list		Pointer to the list.
*	/param		index		Index of the value to retrieve.
*
*	/returns	ELEMENT_PTR		Pointer to the element at the index, or null if the index was out of range.
*/
ELEMENT_PTR internal_list_get(LIST_PTR list, index_t index)
{
	length_t length = internal_list_get_count(list);
	
	//No items in the list.
	if(length == 0)
	{
		return NULL;
	}
	
	if(index > length)
	{
		//ASSERT(false);	//Index out of range
		return NULL;
	}
	
	return &((internal_list_get_aligned_array(list))[index * list->elem_size]);
}

/*
*	Enumerates through all elements in the list, checking them to the predicate's conditions,
*	and returning if the predicate was satisfied.
*
*	/param	list		Pointer to the list.
*	/param	predicate	A function which checks each value to its conditions.
*
*	/returns			True if an element in the list satisfied the predicate's condition, false if the predicate was unsatisfied.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_exists(LIST_PTR list, PREDICATE predicate)
{
	return internal_array_exists(list->array + list->tail, list->head - list->tail, list->elem_size, predicate);
}

/*
*	Enumerates each element in the list, and performs an action on them.
*
*	/param		list	Pointer to the list.
*	/param		action	Function pointer of type ACTION.
*	/param		offset	Zero based offset, at which to start at.
*	/param		size	Size of all the elements which will be included.
*/
void internal_list_forEach(LIST_PTR list, ACTION action, byte_offset_t offset, size_t size)
{
	if(size == 0)
	size = internal_list_get_size(list) - offset;
	
	ARRAY_PTR array = List_Array(list) + offset;
	
	for(int i = 0; i < size; i += list->elem_size)
	{
		(*action)(array + i);
	}
}

/*
*	Determines whether all elements of a sequence satisfy a condition.
*
*	/param		list	Pointer to the list.
*	/param		action	Function pointer of type PREDICATE.
*	/param		offset	Zero based offset, at which to start at.
*	/param		size	Size of all the elements which will be included.
*
*	/returns	bool	true if every element of the source sequence passes the test in the specified predicate, or if the sequence is empty; otherwise, false.
*/
bool internal_list_all(LIST_PTR list, PREDICATE predicate, byte_offset_t offset, size_t size)
{
	if(size == 0)
	size = internal_list_get_size(list) - offset;
	
	ARRAY_PTR array = List_Array(list) + offset;
	
	for(int i = 0; i < size; i += list->elem_size)
	{
		if(!(*predicate)(&array[i]))
		return false;
	}
	
	return true;
}

/*
*	Determines whether any element of a sequence exists or satisfies a condition.
*
*	/param		list	Pointer to the list.
*	/param		action	Function pointer of type PREDICATE.
*	/param		offset	Zero based offset, at which to start at.
*	/param		size	Size of all the elements which will be included.
*
*	/returns	bool	true if the source sequence contains any elements; otherwise, false
*/
bool internal_list_any(LIST_PTR list, PREDICATE predicate, byte_offset_t offset, size_t size)
{
	if(size == 0)
	size = internal_list_get_size(list) - offset;
	
	ARRAY_PTR array = List_Array(list) + offset;
	
	for(int i = 0; i < size; i += list->elem_size)
	{
		if((*predicate)(&array[i]))
		return true;
	}
	
	return false;
}

/*
*	Retrieves a pointer to the first value in the array that matches the predicate.
*
*	/param	list		Pointer to the list.
*	/param	predicate	A function which checks each value to its conditions.
*
*	/returns			Zero based index of the first value in the array that matches the predicate, or -1 if the value does not exist.
*/
int GENOPTI_ATT_FORCE_INLINE internal_list_find_memory(LIST_PTR list, PREDICATE predicate)
{
	return internal_array_select_memory(list->array + list->tail, list->head - list->tail, list->elem_size, predicate) - (list->tail == 0 ? 0 : list->tail / list->elem_size);	
}

/*
*	Retrieves a pointer to the first value in the array that matches the predicate.
*
*	/param	list		Pointer to the list.
*	/param	predicate	A function which checks each value to its conditions, while accepting arguments in the form of a va_list.
*	/param	arg_count	How many arguments have been passed.
*	/param	...			The variadic arguments.
*
*	/returns			Zero based index of the first value in the array that matches the predicate, or -1 if the value does not exist.
*/
int GENOPTI_ATT_FORCE_INLINE internal_list_find_args_memory(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, ...)
{
	va_list ap;
	va_start(ap, arg_count);	
	return internal_list_find_vargs_memory(list, predicate, arg_count, ap);
}

/*
*	Retrieves a pointer to the first value in the array that matches the predicate.
*
*	/param	list		Pointer to the list.
*	/param	predicate	A function which checks each value to its conditions, while accepting arguments in the form of a va_list.
*	/param	start		The byte offset at which to start selecting at.
*	/param	count		The size of the total values to check.
*	/param	arg_count	How many arguments have been passed.
*	/param	ap			The variadic arguments. *Will call va_end before returning.
*
*	/returns			Zero based index of the first value in the array that matches the predicate, or -1 if the value does not exist.
*/
int internal_list_find_vargs_memory(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, va_list ap)
{
	#warning reminder to add start and length to all the list select functions. The macros currently allow them, but it would be much faster to use them in the function.
	size_t size = internal_list_get_size(list);

	for(int i = list->tail; i < list->head; i += list->elem_size)
	{
		if((*predicate)(list->array + i, arg_count, ap ))
		{
			va_end(ap);
			return i = 0 ? 0 : (i - list->tail) / list->elem_size;
		}
	}

	va_end(ap);
	
	return -1;
}

#pragma endregion INTERNAL Functions

#pragma region Unsafe Generic Methods

#warning reminder to move and rename these
#define LIST_COPY_FRONT_BACK_SIZE_THRESHOLD 32
#define LIST_COPY_SIZE_THRESHOLD 32

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, its index will be added to a list and returned.
*
*	/param	list		Pointer to the start of the list.
*	/param	predicate	A function which checks each value to its conditions.
*	/param	offset		Zero based offset, at which to start at.
*	/param	size		Size of all the elements which will be included.
*
*	/returns			A list of zero based indexes to all values in the array that matched the predicate, or NULL if there were no matches.
*/
LIST_PTR internal_list_findAll_indexes(LIST_PTR list, PREDICATE predicate, byte_offset_t offset, size_t size)
{
	LIST_PTR newList = new_List(sizeof(index_t));
	
	ARRAY_PTR array = List_Array(list);
	
	if(size == 0)
	size = internal_list_get_size(list) - offset;

	for(byte_offset_t i = 0; i < size; i += list->elem_size)
	{
		if((*predicate)(array + i))
		{
			internal_list_add_generic(newList, (generic_union_t)(i == 0 ? 0 : i / list->elem_size), sizeof(index_t));
		}
	}

	if(internal_list_get_size(newList) == 0)
	{
		internal_list_delete(newList);
		return NULL;
	}
	
	return newList;
}

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, will have a pointer to them added to a list that will be returned.
*
*	/param	list		Pointer to the start of the list.
*	/param	predicate	A function which checks each value to its conditions.
*	/param	offset		Zero based offset, at which to start at.
*	/param	size		Size of all the elements which will be included.
*
*	/returns			A list containing pointers to each value in the list that matched the predicate's conditions, or NULL if there was no matches.
*/
LIST_PTR internal_list_findAll_pointers(LIST_PTR list, PREDICATE predicate, byte_offset_t offset, size_t size)
{
	LIST_PTR newList = new_List(sizeof(ELEMENT_PTR));
	
	//ARRAY_PTR array = List_Array(list);
	
	if(size == 0)
	size = internal_list_get_size(list) - offset;

	for(byte_offset_t i = offset; i < size + offset; i += list->elem_size)
	{
		if((*predicate)(list->array + i))
		{
			internal_list_add_generic(newList, (generic_union_t)&list->array[i], list->elem_size);
		}
	}

	if(internal_list_get_size(newList) == 0)
	{
		internal_list_delete(newList);
		return NULL;
	}
	
	return newList;
}

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, will be added to a list and returned.
*
*	/param	list		Pointer to the start of the list.
*	/param	predicate	A function which checks each value to its conditions.
*	/param	offset		Zero based offset, at which to start at.
*	/param	size		Size of all the elements which will be included.
*
*	/returns			A list of values from the original list that matched the conditions defined by predicate, or NULL if there was no matches.
*/
LIST_PTR internal_list_findAll_values(LIST_PTR list, PREDICATE predicate, byte_offset_t offset, size_t size)
{
	LIST_PTR newList = new_List(list->elem_size);
	
	//ARRAY_PTR array = List_Array(list);
	
	if(size == 0)
	size = internal_list_get_size(list) - offset;

	for(byte_offset_t i = offset; i < size + offset; i += list->elem_size)
	{
		if((*predicate)(list->array + i))
		{
			internal_list_add_memory(newList, &list->array[i], list->elem_size);
		}
	}

	if(internal_list_get_size(newList) == 0)
	{
		internal_list_delete(newList);
		return NULL;
	}
	
	return newList;
}




/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, its index will be added to a list and returned.
*
*	/param	list		Pointer to the start of the list.
*	/param	predicate	A function which checks each value to its conditions.
*	/param	offset		Zero based offset, at which to start at.
*	/param	size		Size of all the elements which will be included.
*	/param	arg_count	How many arguments have been passed.
*	/param	...			The variadic arguments.
*
*	/returns			A list of zero based indexes to all values in the array that matched the predicate, or NULL if there were no matches.
*/
LIST_PTR internal_list_findAll_indexes_args(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, ...)
{
	va_list ap;
	va_start(ap, arg_count);
	return internal_list_findAll_indexes_vargs(list, predicate, arg_count, ap);
}

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, will have a pointer to them added to a list that will be returned.
*
*	/param	list		Pointer to the start of the list.
*	/param	predicate	A function which checks each value to its conditions.
*	/param	offset		Zero based offset, at which to start at.
*	/param	size		Size of all the elements which will be included.
*	/param	arg_count	How many arguments have been passed.
*	/param	...			The variadic arguments.
*
*	/returns			A list containing pointers to each value in the list that matched the predicate's conditions, or NULL if there was no matches.
*/
LIST_PTR internal_list_findAll_pointers_args(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, ...)
{
	va_list ap;
	va_start(ap, arg_count);
	return internal_list_findAll_pointers_vargs(list, predicate, arg_count, ap);
}

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, will be added to a list and returned.
*
*	/param	list		Pointer to the start of the list.
*	/param	predicate	A function which checks each value to its conditions.
*	/param	offset		Zero based offset, at which to start at.
*	/param	size		Size of all the elements which will be included.
*	/param	arg_count	How many arguments have been passed.
*	/param	...			The variadic arguments.
*
*	/returns			A list of values from the original list that matched the conditions defined by predicate, or null if there was no matches.
*/
LIST_PTR internal_list_findAll_values_args(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, ...)
{
	va_list ap;
	va_start(ap, arg_count);
	return internal_list_findAll_values_vargs(list, predicate, arg_count, ap);
}


/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, its index will be added to a list and returned.
*
*	/param	list		Pointer to the start of the list.
*	/param	predicate	A function which checks each value to its conditions.
*	/param	offset		Zero based offset, at which to start at.
*	/param	size		Size of all the elements which will be included.
*	/param	arg_count	How many arguments have been passed.
*	/param	ap			The variadic arguments. *Will call va_end before returning.
*
*	/returns			A list of zero based indexes to all values in the array that matched the predicate, or NULL if there were no matches.
*/
LIST_PTR internal_list_findAll_indexes_vargs(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, va_list ap)
{
	LIST_PTR newList = new_List(list->elem_size);
	
	size_t size = internal_list_get_size(list);

	for(int i = list->tail + list->elem_size; i < list->head; i += list->elem_size)
	{
		if((*predicate)(list->array + i, arg_count, ap))
		{
			internal_list_add_generic(newList, (generic_union_t)((i - list->tail) == 0 ? 0 : (i - list->tail) /list->elem_size), list->elem_size);
		}
	}
	
	va_end(ap);
	
	if(internal_list_get_size(newList) == 0)
	{
		internal_list_delete(newList);
		return NULL;
	}
	
	return newList;
}

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, will have a pointer to them added to a list that will be returned.
*
*	/param	list		Pointer to the start of the list.
*	/param	predicate	A function which checks each value to its conditions.
*	/param	offset		Zero based offset, at which to start at.
*	/param	size		Size of all the elements which will be included.
*	/param	arg_count	How many arguments have been passed.
*	/param	ap			The variadic arguments. *Will call va_end before returning.
*
*	/returns			A list containing pointers to each value in the list that matched the predicate's conditions
*/
LIST_PTR internal_list_findAll_pointers_vargs(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, va_list ap)
{
	LIST_PTR newList = new_List(list->elem_size);
	
	size_t size = internal_list_get_size(list);

	for(int i = list->tail; i < list->head; i += list->elem_size)
	{
		if((*predicate)(list->array + i, arg_count, ap))
		{
			internal_list_add_generic(newList, (generic_union_t)&list->array[i], list->elem_size);
		}
	}
	
	va_end(ap);
	
	if(internal_list_get_size(newList) == 0)
	{
		internal_list_delete(newList);
		return NULL;
	}
	
	return newList;
}

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, will be added to a list and returned.
*
*	/param	list		Pointer to the start of the list.
*	/param	predicate	A function which checks each value to its conditions.
*	/param	offset		Zero based offset, at which to start at.
*	/param	size		Size of all the elements which will be included.
*	/param	arg_count	How many arguments have been passed.
*	/param	ap			The variadic arguments. *Will call va_end before returning.
*
*	/returns			A list of values from the original list that matched the conditions defined by predicate, or null if there was no matches.
*/
LIST_PTR internal_list_findAll_values_vargs(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, va_list ap)
{
	LIST_PTR newList = new_List(list->elem_size);
	
	size_t size = internal_list_get_size(list);

	for(int i = list->tail; i < list->head; i += list->elem_size)
	{
		if((*predicate)(list->array + i, arg_count, ap))
		{
			internal_list_add_memory(newList, &list->array[i], list->elem_size);
		}
	}
	
	va_end(ap);
	
	if(internal_list_get_size(newList) == 0)
	{
		internal_list_delete(newList);
		return NULL;
	}
	
	return newList;
}



/*
*	Creates a new list containing the values within the range.
*
*	/param	list			Pointer to the new list
*	/param	offset			Byte offset at which the range starts.
*	/param	range_size		The size of the the range.
*
*	/returns LIST_PTR		Pointer to the new list containing the range.
*/
LIST_PTR internal_list_getRange_memory(LIST_PTR list, byte_offset_t offset, size_t range_size)
{
	LIST_PTR rangeList = internal_list_new(list->elem_size, range_size);
	
	ARRAY_PTR array = List_Array(list) + offset;
	
	size_t size = internal_list_get_size(list);
	
	ASSERT(offset + range_size >= size);	//Out of range
	
	geneticc_memcpy(rangeList->array, array, range_size);
	//internal_list_addRange_memory(rangeList, List_Array(list) + index, count, list->elem_size);
	
	return rangeList;
}

/*
*	Inserts a range of values in to a list.
*
*	/param		list		Pointer to the list.
*	/param		range		Pointer to where the range of values to insert starts.
*	/param		offset		The byte offset where the new values should be inserted at(Not taking list->tail into account).
*	/param		range_size	The size of the of elements being inserted
*	/param		elem_size	The size of the elements being inserted.
*
*	/returns	bool		True or false depending if the list had enough capacity for the range of values.
*/
bool internal_list_insertRange_memory(LIST_PTR list, const ARRAY_PTR range, byte_offset_t offset, size_t range_size, element_size_t elem_size )
{
	//If range_size is 0, you may have passed a non static array without the length.
	ASSERT(range_size);
	
	//uint32_t byte_count = count * list->elem_size;
	length_t count = range_size / elem_size;
	//size_t insert_size = count * list->elem_size;
	
	
	size_t current_size = internal_list_get_size(list);
	
	if(!internal_list_ensure_capacity(list, current_size + range_size ))
		return false;
	
	//#if GENETICC_OPTIMIZATION_LEVEL != GENETICC_OPTIMIZATION_LEVEL_SPEED_1
	
	bool front_has_capacity = range_size <= list->tail;
	
	//If we are prepending the range, check if there is enough room before the tail. 
	if(offset == 0 && front_has_capacity)
	{
		//Enough room to fit the range before the tail
		if(list->elem_size != elem_size)
		{
			geneticc_set_values_memory((list->array + list->tail) - range_size, list->elem_size, range, elem_size, count);
		}
		else
		{
			geneticc_memcpy((list->array + list->tail) - range_size, range, range_size);
		}
		list->tail -= range_size;
		return;
	}
	
	bool back_has_capacity = range_size <= list->capacity - list->head;
	
	//If we are adding the range, check if there is enough room between the head and end of the internal array.
	if(offset == current_size && back_has_capacity)
	{
		//Enough room to fit the range after the head?
		if(list->elem_size != elem_size)
		{
			geneticc_set_values_memory((list->array + list->head), list->elem_size, range, elem_size, count);
		}
		else
		{
			geneticc_memcpy((list->array + list->head), range, range_size);
		}
		
		list->head += range_size;
		return;
	}
	
	//#endif
	

	

	size_t front_size = offset;
	size_t back_size = current_size - front_size;
	byte_offset_t back_offset = list->tail + front_size; //Where back is located in source | or list->head - back_size
	size_t insert_end_offset = back_offset + range_size;
	bool front_smaller = front_size < back_size;
	int size_threshold = front_size - back_size;
	bool threshold_met = size_threshold > LIST_COPY_FRONT_BACK_SIZE_THRESHOLD || size_threshold < -LIST_COPY_FRONT_BACK_SIZE_THRESHOLD;	
	bool alignArray = (LIST_MAX_TAIL_BEFORE_REALIGN * list->elem_size) < list->tail;

//#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
//goto MoveBoth;
//#endif

	if(alignArray || (!front_has_capacity && !back_has_capacity))
	{
		MoveBoth:
		//Move front to start of array.
		if(front_size > 0)
		{
			geneticc_memcpy(list->array, (list->array + list->tail), front_size);
			insert_end_offset -= list->tail;
			list->tail = 0;
		}
		
		//Move back
		if(back_size > 0)
		{
			geneticc_memcpyovrlp(list->array + insert_end_offset, (list->array + back_offset), back_size);
			list->head = insert_end_offset + back_size;
		}
		goto CopyRange;
	}
	//#if GENETICC_OPTIMIZATION_LEVEL != GENETICC_OPTIMIZATION_LEVEL_SIZE
	else
	{
		
		if(front_has_capacity && back_has_capacity){
			if(front_smaller)
				goto MoveFront;				
			else
				goto MoveBack;
		}else if(front_has_capacity){
			//if(front_smaller || (!front_smaller && !threshold_met))
				goto MoveFront;				
			//else
				//goto MoveBack;
		}else if(back_has_capacity){
			//if(front_smaller && threshold_met)
				//goto MoveFront;
			//else
				goto MoveBack;
		}
	}
	
	MoveFront:
	{
		geneticc_memcpy((list->array + list->tail) - range_size, (list->array + list->tail), front_size);
		list->tail -= range_size;	
		goto CopyRange;
	}
	
	MoveBack:
	{
		geneticc_memcpyovrlp((list->array + back_offset) + range_size, (list->array + back_offset), back_size);
		list->head += range_size;
	}
	//#endif
	

	byte_offset_t insert_offset;	//Same as back_offset
	
	CopyRange:
	insert_offset = list->tail + offset;
		

	if(list->elem_size != elem_size)
	{
		//Range elements are different size than list elements.
		//As long as you list is something like int, and range is something like char,
		//everything is fine.
		//If list is int, and range is long, then data could be lost, especially the sign,
		//this is when it is up to the coder to convert the array before inserting.
		//Can use the oh so ugry internal_array_convertTo_memory() to do so.
		
		geneticc_set_values_memory((list->array + insert_offset), list->elem_size, range, elem_size, count);
	}
	else
	{
		//Place our range into the new array.
		geneticc_memcpy( (list->array  + insert_offset), range, range_size);
	}

	return true;
}


/*
*	Inserts a value in to a list.
*
*	/param		list		Pointer to the list.
*	/param		value		Pointer to the value to insert starts.
*	/param		offset		The byte offset where the new values should be inserted at.
*	/param		elem_size	The size of the elements being inserted.
*
*	/returns	bool		True or false depending if the list had enough capacity for the of value.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_insert_memory(LIST_PTR list, const ELEMENT_PTR value, byte_offset_t offset, element_size_t elem_size)
{
	return internal_list_insertRange_memory(list, value, offset, elem_size, elem_size);
}

/*
*	Inserts a value in to a list.
*
*	/param		list		Pointer to the list.
*	/param		value		Pointer to the value to insert starts.
*	/param		offset		The byte offset where the new values should be inserted at.
*	/param		elem_size	The size of the elements being inserted.
*
*	/returns	bool		True or false depending if the list had enough capacity for the of value.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_insert_generic(LIST_PTR list, generic_union_t value, byte_offset_t offset,  element_size_t elem_size)
{
	uint64_t v = value.u64;	//Store value so we can get pointer from.
	
	return internal_list_insertRange_memory(list, &v, offset, elem_size, elem_size);
}

/*
*	Adds a range of values on to the end of an list.
*
*	/param		list		Pointer to the list.
*	/param		range		Pointer to where the range of values to add starts.
*	/param		range_size	The size of the of elements being added
*	/param		elem_size	The size of the elements being added.
*
*	/returns	bool		True or false depending if the list had enough capacity for the range of values.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_addRange_memory(LIST_PTR list, const ARRAY_PTR range, size_t range_size,  element_size_t elem_size )
{
	return internal_list_insertRange_memory(list, range, internal_list_get_size(list), range_size, elem_size);
}

/*
*	Adds a values on to the end of an list.
*
*	/param		list		Pointer to the list.
*	/param		value		Pointer to where the value to add starts.
*	/param		elem_size	The size of the element being added.
*
*	/returns	bool		True or false depending if the list had enough capacity for the range of values.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_add_memory(LIST_PTR list, const ELEMENT_PTR value,  element_size_t elem_size )
{
	return internal_list_insertRange_memory(list, value,  internal_list_get_size(list), elem_size, elem_size);
}

/*
*	Adds a value to the end of an list.
*
*	/param		list		Pointer to the list.
*	/param		value		The value being added.
*	/param		elem_size	The size of the element being added.
*
*	/returns	bool		True or false depending if the list had enough capacity for the range of values.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_add_generic(LIST_PTR list, generic_union_t value,  element_size_t elem_size)
{
	uint64_t v = value.u64;	//Store value so we can get pointer from.
	
	return internal_list_insertRange_memory(list, &v, internal_list_get_size(list), elem_size, elem_size);
}

/*
*	Prepends a range of values on to the start of an list.
*
*	/param		list		Pointer to the list.
*	/param		range		Pointer to where the range of values to Prepends starts.
*	/param		range_size	The size of the of elements being added
*	/param		elem_size	The size of the elements being added.
*
*	/returns	bool		True or false depending if the list had enough capacity for the range of values.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_prependRange_memory(LIST_PTR list, const ARRAY_PTR range, size_t range_size,  element_size_t elem_size )
{
	return internal_list_insertRange_memory(list, range, 0, range_size, elem_size);
}

/*
*	Prepends a value to the end of an list.
*
*	/param		list		Pointer to the list.
*	/param		value		The value being added.
*	/param		elem_size	The size of the element being added.
*
*	/returns	bool		True or false depending if the list had enough capacity for the range of values.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_prepend_memory(LIST_PTR list, const ELEMENT_PTR value,  element_size_t elem_size )
{
	return internal_list_insertRange_memory(list, value, 0, elem_size, elem_size);
}

/*
*	Prepends a value to the end of an list.
*
*	/param		list		Pointer to the list.
*	/param		value		The value being added.
*	/param		elem_size	The size of the element being added.
*
*	/returns	bool		True or false depending if the list had enough capacity for the range of values.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_prepend_generic(LIST_PTR list, generic_union_t value,  element_size_t elem_size)
{
	uint64_t v = value.u64;	//Store value so we can get pointer from.

	return internal_list_insertRange_memory(list, &v, 0, elem_size, elem_size);
}

/*
*	Removes a range of values from an list.
*
*	/param	list		Pointer to the list.
*	/param	offset		The byte offset to start removing at.
*	/param	byte_count	The size of the elements being removed.
*/
void internal_list_removeRange_memory(LIST_PTR list, byte_offset_t offset, size_t byte_count )
{
	ASSERT(byte_count && byte_count % list->elem_size == 0);
	
	//size_t byte_count = length * list->elem_size;
	byte_offset_t aligned_offset = list->tail + offset;
	
	if(offset == 0)
	{
		list->tail += byte_count;
	}
	else if(aligned_offset == list->head)
	{
		list->head -= byte_count;
	}
	else
	{
		size_t max_tail = (LIST_MAX_TAIL_BEFORE_REALIGN * list->elem_size);
		bool alignArray = max_tail < list->tail;
		
		#if GENETICC_OPTIMIZATION_LEVEL != GENETICC_OPTIMIZATION_LEVEL_SIZE
		
		size_t current_size = internal_list_get_size(list);
		size_t front_size = offset;
		size_t back_size = current_size - front_size;
		
		bool front_smaller = front_size < back_size;
		int size_threshold = front_size - back_size;
		bool threshold_met = size_threshold > LIST_COPY_FRONT_BACK_SIZE_THRESHOLD || size_threshold < -LIST_COPY_FRONT_BACK_SIZE_THRESHOLD;
		
		
		
		
		if(alignArray)
		{
			geneticc_memcpy(list->array, list->array + list->tail + front_size, front_size);
			geneticc_memcpy(list->array + front_size, list->array + aligned_offset + byte_count, back_size);
			list->head -= list->tail - byte_count;
			list->tail = 0;
		}
		else
		{
			if(front_smaller && threshold_met && list->tail + byte_count <= max_tail)
			{
				geneticc_memcpyovrlp(list->array + list->tail + byte_count, list->array + list->tail, front_size);
				list->tail += byte_count;
			}
			else
			{
				geneticc_memcpy(list->array + aligned_offset, list->array + aligned_offset + byte_count, back_size);
				list->head -= byte_count;
			}
			
		}
		
		#else
		size_t back_size =  internal_list_get_size(list) - offset;
		geneticc_memcpy(list->array + aligned_offset, list->array + aligned_offset + byte_count, back_size);
		list->head -= byte_count;		
		#endif
	
	}
}

/*
*	Removes a values from an list by its byte offset.
*
*	/param	list		Pointer to the list.
*	/param	offset		The byte offset to start removing at.
*/
void GENOPTI_ATT_FORCE_INLINE internal_list_removeAt_memory(LIST_PTR list, byte_offset_t offset )
{
	internal_list_removeRange_memory(list, offset, list->elem_size);
}


/*
*	Removes the first item from an list which equals "value."
*
*	/param	list			Pointer to the list.
*	/param	value			A pointer to the value to remove.
*
*	/returns				True or false depending on if an item was found and removed.
*/
bool internal_list_remove_memory(LIST_PTR list, const ELEMENT_PTR value )
{
	//#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	//Element index
	ARRAY_PTR array = internal_list_get_aligned_array(list);
	size_t size = internal_list_get_size(list);
	int index = internal_array_indexOf_memory( array, value, size, list->elem_size);
	
	//Value not in list, nothing to remove.
	if(index == -1)
	{
		return false;
	}

	internal_list_removeAt_memory(list, index * list->elem_size);
	
	return true;
	//#endif
}


/*
*	Removes the first item from an list which equals "value."
*
*	/param	list			Pointer to the list.
*	/param	value			A pointer to the value to remove.
*
*	/returns				True or false depending on if an item was found and removed.
*/
bool internal_list_remove_generic(LIST_PTR list, generic_union_t value)
{
	uint64_t v = value.u64;	//Store value so we can get pointer from.
		
	return internal_list_remove_memory(list, &v) != 0;
}


/*
*	Removes all items from an list which equals "value."
*
*	/param	list			Pointer to the start of the list.
*	/param	value			A pointer to the value which is used to determine which elements to remove.
*
*	/returns				A count of how many elements were removed.
*/
uint32_t internal_list_removeAll_memory(LIST_PTR list, ELEMENT_PTR value)
{
	#warning reminder to time different optimization levels in this function.
	uint32_t remove_count = 0;
	
	#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	
	int index = 0;
	do
	{
		index = List_Indexof(list, value, index);
		
		if(index != -1)
		{
			remove_count ++;
			internal_list_removeAt_memory(list, index * list->elem_size);
		}
	} while (index != -1);
	
	
	#elif GENETICC_OPTIMIZATION_LEVEL < GENETICC_OPTIMIZATION_LEVEL_SPEED_2
	
	size_t size = internal_list_get_size(list);
	
	
	
	for(int i = list->tail; i < list->head; i += list->elem_size)	
	{
		uint8_t b;
		RedoIndex:
		b = 0;
		
		for(b; b < list->elem_size; b++)
		{
			if(list->array[i + b] != value[b])
			{
				break;				
			}
		}
		
		if(b == list->elem_size)
		{
			remove_count++;
			
			#if GENETICC_OPTIMIZATION_LEVEL != GENETICC_OPTIMIZATION_LEVEL_SIZE			
				if(i < size / 2)
				{
					geneticc_memcpyovrlp(/*Dest*/ list->array + list->tail + list->elem_size, /*Src*/ list->array + list->tail,  /*n*/ i - list->tail);
					list->tail += list->elem_size;
				}
				else
				{
					memcpy(list->array + i, list->array + i + list->elem_size, size - (i - list->tail));
					list->head -= list->elem_size;
				
					//Redo the current index, as the next value is now in its place.
					goto RedoIndex;
				}
			#else
				memcpy(list->array + i, list->array + i + list->elem_size, size - (i - list->tail));
				list->head -= list->elem_size;
				
				//Redo the current index, as the next value is now in its place.
				goto RedoIndex;
			#endif
		}
	}
	
	#else
	
	LIST_PTR remove_list = new_List(list->elem_size, size);
	
	switch(list->elem_size)
	{
		case 1:
			for(int i = list->tail; i < list->head; i += list->elem_size)
			{
				if( ((uint8_t*)list->array)[i] != *((uint8_t*)value) )
				{
					internal_list_add_memory(remove_list, (uint8_t*)list->array)[i], list->elem_size);
					remove_count++;
				}
			}
		break;
		case 2:
			for(int i = list->tail; i < list->head; i += list->elem_size)
			{
				if( ((uint16_t*)list->array)[i] != *((uint16_t*)value) )
				{
					internal_list_add_memory(remove_list, (uint16_t*)list->array)[i], list->elem_size);
					remove_count++;
				}
			}
		break;
		case 4:
			for(int i = list->tail; i < list->head; i += list->elem_size)
			{
				if( ((uint32_t*)list->array)[i] != *((uint32_t*)value) )
				{
					internal_list_add_memory(remove_list, (uint32_t*)list->array)[i], list->elem_size);
					remove_count++;
				}
			}
		break;
		case 8:
			for(int i = list->tail; i < list->head; i += list->elem_size)
			{
				if( ((uint64_t*)list->array)[i] != *((uint64_t*)value) )
				{
					internal_list_add_memory(remove_list, (uint64_t*)list->array)[i], list->elem_size);
					remove_count++;
				}
			}
		break;
		default:
		
			uint8_t* non_matching = malloc(list->elem_size);
			
			for(int i = list->tail; i < list->head; i += list->elem_size)
			{
				RedoIndex:
				uint8_t b = 0;
				
				for(b; b < list->elem_size; b++)
				{
					if(list[i + b] == value[b])
					{
						non_matching[b] = value[b];
					}
					else
					{
						break;
					}
				}
				
				if(b == list->elem_size)
				{
					internal_list_add_memory(remove_list, non_matching, list->elem_size);
					remove_count++;
				}
			}
		break;
	}
	
	internal_list_trimExcess(remove_list);
	
	list->head = remove_list->head;
	list->tail = remove_list->tail;
	list->capacity = remove_list->capacity;
	free(list->array);
	list->array = remove_list->array;
	free(remove_list);
	
	#endif

	return remove_count;
}

/*
*	Removes all items from an list which equals "value."
*
*	/param	list			Pointer to the start of the list.
*	/param	value			The value which is used to determine which elements to remove.
*
*	/returns				Returns amount of values removed from the list.
*/
uint32_t internal_list_removeAll_generic(LIST_PTR list, generic_union_t value )
{
	uint64_t v = value.u64;	//Store value so we can get pointer from.
	
	return internal_list_removeAll_memory(list, &v);
}



/*
*	Removes the first item from an list which equals "value."
*
*	/param	list			Pointer to the list.
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*
*	/returns				True or false depending on if an item was found and removed.
*/
bool internal_list_removeMatch_memory(LIST_PTR list, PREDICATE predicate )
{
	//#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	//Element index
	ARRAY_PTR array = internal_list_get_aligned_array(list);
	size_t size = internal_list_get_size(list);
	int index = internal_array_select_memory( array, size, list->elem_size, predicate);
	
	//Value not in list, nothing to remove.
	if(index == -1)
	{
		return false;
	}

	internal_list_removeAt_memory(list, index * list->elem_size);
	
	return true;
	//#endif
}

/*
*	Removes all items from an list which equals "value."
*
*	/param	list			Pointer to the start of the list.
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*
*	/returns				A count of how many elements were removed.
*/
uint32_t internal_list_removeAllMatching_memory(LIST_PTR list, PREDICATE predicate)
{
	uint32_t remove_count = 0;
	
	#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	
	int index = 0;
	do
	{
		index = List_FindIndex(list, predicate);
		
		if(index != -1)
		{
			remove_count ++;
			internal_list_removeAt_memory(list, index * list->elem_size);
		}
	} while (index != -1);
	
	
	#else
	
	size_t size = internal_list_get_size(list);
	
	for(int i = list->tail; i < list->head; i += list->elem_size)
	{
		RedoIndex:
		if((*predicate)(list->array + i))
		{
			remove_count++;
			
			#if GENETICC_OPTIMIZATION_LEVEL != GENETICC_OPTIMIZATION_LEVEL_SIZE
			if(i < size / 2)
			{
				geneticc_memcpyovrlp(/*Dest*/ list->array + list->tail + list->elem_size, /*Src*/ list->array + list->tail,  /*n*/ i - list->tail);
				list->tail += list->elem_size;
			}
			else
			{
				memcpy(list->array + i, list->array + i + list->elem_size, size - (i - list->tail));
				list->head -= list->elem_size;
				
				//Redo the current index, as the next value is now in its place.
				goto RedoIndex;
			}
			#else
			memcpy(list->array + i, list->array + i + list->elem_size, size - (i - list->tail));
			list->head -= list->elem_size;
			
			//Redo the current index, as the next value is now in its place.
			goto RedoIndex;
			#endif
		}
	}
	#endif

	return remove_count;

}

/*
*	Removes the first item from an list which equals "value."
*
*	/param	list			Pointer to the list.
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*	/param	arg_count		How many arguments have been passed.
*	/param	ap				The variadic arguments. *Will call va_end before returning.
*
*	/returns				True or false depending on if an item was found and removed.
*/
bool internal_list_removeMatch_vargs_memory(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, va_list ap )
{
	//#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	//Element index
	ARRAY_PTR array = internal_list_get_aligned_array(list);
	size_t size = internal_list_get_size(list);
	int index = internal_array_select_vargs_memory( array, size, list->elem_size, predicate, arg_count, ap);
	
	//Value not in list, nothing to remove.
	if(index == -1)
	{
		return false;
	}

	internal_list_removeAt_memory(list, index * list->elem_size);
	
	return true;
	//#endif
}

/*
*	Removes all items from an list which equals "value."
*
*	/param	list			Pointer to the start of the list.
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*	/param	arg_count		How many arguments have been passed.
*	/param	ap				The variadic arguments. *Will call va_end before returning.
*
*	/returns				A count of how many elements were removed.
*/
uint32_t internal_list_removeAllMatching_vargs_memory(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, va_list ap)
{
	uint32_t remove_count = 0;
	
	#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	
	int index = 0;
	do 
	{
		index = List_FindIndexVargs(list, predicate, arg_count, ap, index);
		
		if(index != -1)
		{
			remove_count ++;
			internal_list_removeAt_memory(list, index * list->elem_size);
		}
	} while (index != -1);
	
	
	#else
	
	
	size_t size = internal_list_get_size(list);
	
	for(int i = list->tail; i < list->head; i += list->elem_size)
	{
		RedoIndex:
		if((*predicate)(list->array + i, arg_count, ap))
		{
			remove_count++;
			
			#if GENETICC_OPTIMIZATION_LEVEL > GENETICC_OPTIMIZATION_LEVEL_SPEED_1
			if(i < size / 2)
			{
				geneticc_memcpyovrlp(/*Dest*/ list->array + list->tail + list->elem_size, /*Src*/ list->array + list->tail,  /*n*/ i - list->tail);
				list->tail += list->elem_size;
			}
			else
			{
				memcpy(list->array + i, list->array + i + list->elem_size, size - (i - list->tail));
				list->head -= list->elem_size;
				
				//Redo the current index, as the next value is now in its place.
				goto RedoIndex;
			}
			#else
			memcpy(list->array + i, list->array + i + list->elem_size, size - (i - list->tail));
			list->head -= list->elem_size;
			
			//Redo the current index, as the next value is now in its place.
			goto RedoIndex;
			#endif
		}
	}
	
	#endif
	

	return remove_count;
}

/*
*	Removes the first item from an list which equals "value."
*
*	/param	list			Pointer to the list.
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*	/param	arg_count		How many arguments have been passed.
*	/param	...				The variadic arguments.
*
*	/returns				True or false depending on if an item was found and removed.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_removeMatch_args_memory(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, ... )
{
	va_list ap;
	va_start(ap, arg_count);
	return internal_list_removeMatch_vargs_memory(list, predicate, arg_count, ap);
}

/*
*	Removes all items from an list which equals "value."
*
*	/param	list			Pointer to the start of the list.
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*	/param	arg_count		How many arguments have been passed.
*	/param	...				The variadic arguments.
*
*	/returns				A count of how many elements were removed.
*/
uint32_t GENOPTI_ATT_FORCE_INLINE internal_list_removeAllMatching_args_memory(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, ...)
{
	va_list ap;
	va_start(ap, arg_count);
	return internal_list_removeAllMatching_vargs_memory(list, predicate, arg_count, ap);
}



/*
*	Reverse the internal array in memory.
*
*	/param	list			Pointer to the start of the list.
*/
void internal_list_reverse_memory(LIST_PTR list, int start)
{
	//internal_list_trimExcess(list);
	//size_t size = internal_list_get_size(list);
	
	list->array = internal_array_reverse_memory(list->array, list->capacity, list->elem_size, true);
}


//void internal_list_sort_memory(LIST_PTR list, COMPARISON comparison)
//{
	//list->array = internal_array_sort_memory();
//}



#pragma endregion Unsafe Generic Methods


#pragma region Unsafe Conversion Methods

#warning tired
#warning untested
#warning reminder to time each function, and add if GENETICC_OPTIMIZATION_LEVEL



/*
*	Array ConvertTo(Cast)
*
*	*Very slow!
*
*	/param		name	desc
*
*	/returns	type	desc
*/
LIST_PTR internal_list_convertTo_cast(LIST_PTR list, type_sized_t oldType, type_sized_t newType)
{
	ARRAY_PTR array = internal_array_convertTo_cast(list->array, list->capacity, oldType, newType, true);
	
	element_size_t new_elem_size = geneticc_get_sized_type_abs(newType);
	
	LIST_PTR new_list = Array_To_List(array, List_Count(list), 0, new_elem_size);
	
	return new_list;
}

#warning untested
LIST_PTR internal_list_convertTo_memory(LIST_PTR list, type_sized_t oldType, type_sized_t newType)
{
	ARRAY_PTR array = internal_array_convertTo_memory(list->array, list->capacity, oldType, newType, true);
	
	element_size_t new_elem_size = geneticc_get_sized_type_abs(newType);
	
	LIST_PTR new_list = Array_To_List(array, List_Count(list), 0, new_elem_size);
	
	return new_list;
}

#pragma endregion Unsafe Conversion Methods

#pragma region DMA Functions
#if GENETICC_USE_DMA


/*
*	Initializes a list by copying an already existing array using DMA.
*	*Returned list will not be usable right away. Use DMA user callbacks to get when list is filled.
*
*	/param	array				A pointer to the start of the array.
*	/param	size				The total capacity of the list(in bytes).
*	/param	elem_size			The size of the value's type (in bytes).
*	/param	populated_size		The size of the array (in bytes)	*If 0, this will use size.
*
*	/Returns					Pointer to the newly allocated list, or null if the transfer was unable to be registered.
*/
LIST_PTR internal_array_dma_copyTo_list(ARRAY_PTR array, size_t size,  element_size_t elem_size, size_t populated_size)
{
	LIST_PTR list = malloc(sizeof(list_t));
	
	if(populated_size == 0)
	populated_size = size;
	
	uint8_t* newArray = malloc(size);

	internal_array_init_list(newArray, size, elem_size, populated_size, list);
	
	geneticc_dma_transfer_t* transfer = geneticc_dma_create_transfer(list, array, populated_size, false);
	geneticc_dma_create_subtransfer(newArray, array, populated_size, transfer, NULL);
	
	bool registered = geneticc_dma_register_transfer(transfer);
	
	if(registered)
	return list;
	else
	return NULL;
}


/*
*	Initializes a list, and copies an already existing array to the list.
*
*	/param	array				A pointer to the start of the array.
*	/param	size				The total size of the array(in bytes).
*	/param	elem_size			The size of the value's type (in bytes).
*	/param	capacity			The size of the internal array (in bytes).
*	/param	populated_size		The size of the used elements in the array (in bytes).
*	/param	out_list			(out)The list
*
*	/returns	bool			True if the DMA transfer was registered. False if there was no capacity in the list, or if the maximum transfer count has been reached.
*/
bool internal_array_dma_copy_init_list(ARRAY_PTR array, size_t size,  element_size_t elem_size, size_t populated_size, LIST_PTR out_list)
{
	internal_list_init(out_list, elem_size, size);
	
	if(populated_size != 0 && populated_size < size)
	out_list->head = populated_size;
	else
	out_list->head = size;
	
	geneticc_dma_transfer_t* transfer = geneticc_dma_create_transfer(out_list, array, out_list->head, false);
	geneticc_dma_create_subtransfer(out_list->array, array, out_list->head, transfer, NULL);
	
	return geneticc_dma_register_transfer(transfer);
}

/*
*	Creates a new list containing the values within the range.
*
*	/param	list			Pointer to the new list
*	/param	offset			Byte offset at which the range starts.
*	/param	range_size		The size of the the range.
*	/param
*
*	/returns	bool		Pointer to the new list, or null if the DMA transfer was unable to register.
*/
LIST_PTR internal_list_dma_getRange_memory(LIST_PTR list, byte_offset_t offset, size_t range_size)
{
	LIST_PTR rangeList = internal_list_new(list->elem_size, range_size);
	
	ARRAY_PTR array = List_Array(list);
	
	size_t size = internal_list_get_size(list);
	
	ASSERT(offset + range_size >= size);	//Out of range	

	geneticc_dma_transfer_t* transfer = geneticc_dma_create_transfer(rangeList->array, array, range_size, false);
	geneticc_dma_subtransfer_t* subtransfer = geneticc_dma_create_subtransfer(rangeList->array, array + offset, range_size, transfer, NULL);
		
	bool registered = geneticc_dma_register_transfer(transfer);
	
	if(registered)
	return list;
	else
	return NULL;
}

/*
*	Inserts a range of values in to a list.
*
*	/param		list		Pointer to the list.
*	/param		range		Pointer to where the range of values to insert starts.
*	/param		offset		The byte offset where the new values should be inserted at(Not taking list->tail into account).
*	/param		range_size	The size of the of elements being inserted
*
*	/returns	bool		True if the DMA transfer was registered. False if there was no capacity in the list, or if the maximum transfer count has been reached.
*/
bool internal_list_dma_insertRange_memory(LIST_PTR list, const ARRAY_PTR range, byte_offset_t offset, size_t range_size)
{

	//If range_size is 0, you may have passed a non static array without the length.
	ASSERT(range_size);
	
	//uint32_t byte_count = count * list->elem_size;
	length_t count = range_size / list->elem_size;
	//size_t insert_size = count * list->elem_size;
	
	size_t current_size = internal_list_get_size(list);
	
	if(!internal_list_ensure_capacity(list, current_size + range_size ) || !geneticc_dma_accepting_transfers())
	return false;
	
	geneticc_dma_transfer_t* transfer = geneticc_dma_create_transfer(list, range, current_size + range_size, false);
	
	//#if GENETICC_OPTIMIZATION_LEVEL != GENETICC_OPTIMIZATION_LEVEL_SPEED_1		
	
	bool front_has_capacity = range_size <= list->tail;
	
	//If we are prepending the range, check if there is enough room before the tail.
	if(offset == 0 && front_has_capacity)
	{
		//Enough room to fit the range before the tail
		geneticc_dma_create_subtransfer((list->array + list->tail) - range_size, range, range_size, transfer, NULL);		
		list->tail -= range_size;
		goto RegisterTransfer;
	}
	
	bool back_has_capacity = range_size <= list->capacity - list->head;
	
	//If we are adding the range, check if there is enough room between the head and end of the internal array.
	if(offset == current_size && back_has_capacity)
	{
		//Enough room to fit the range after the head
		geneticc_dma_create_subtransfer((list->array + list->head), range, range_size, transfer, NULL);
		list->head += range_size;
		goto RegisterTransfer;
	}
	
	//#endif
	

	

	size_t front_size = offset;
	size_t back_size = current_size - front_size;
	byte_offset_t back_offset = list->tail + front_size; //Where back is located in source | or list->head - back_size
	size_t insert_end_offset = back_offset + range_size;


	#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	
	goto MoveBoth;
	
	#else
	
	bool front_smaller = front_size < back_size;
	int size_threshold = front_size - back_size;
	bool threshold_met = size_threshold > LIST_COPY_FRONT_BACK_SIZE_THRESHOLD || size_threshold < -LIST_COPY_FRONT_BACK_SIZE_THRESHOLD;
	bool alignArray = (LIST_MAX_TAIL_BEFORE_REALIGN * list->elem_size) < list->tail;	
		
	if(alignArray || (!front_has_capacity && !back_has_capacity))
	{
	#endif
		MoveBoth:
		//Move front to start of array.
		if(front_size > 0)
		{
			geneticc_dma_create_subtransfer(list->array, (list->array + list->tail), front_size, transfer, NULL);
			insert_end_offset -= list->tail;
			list->tail = 0;
		}
		
		//Move back
		if(back_size > 0)
		{
			geneticc_dma_create_subtransfer(list->array + insert_end_offset, (list->array + back_offset), back_size, transfer, NULL);
			list->head = insert_end_offset + back_size;
		}
		goto CopyRange;
	#if GENETICC_OPTIMIZATION_LEVEL != GENETICC_OPTIMIZATION_LEVEL_SIZE
	}
	else
	{
		
		if(front_has_capacity && back_has_capacity){
			if(front_smaller)
			goto MoveFront;
			else
			goto MoveBack;
			}else if(front_has_capacity){
			//if(front_smaller || (!front_smaller && !threshold_met))
			goto MoveFront;
			//else
			//goto MoveBack;
			}else if(back_has_capacity){
			//if(front_smaller && threshold_met)
			//goto MoveFront;
			//else
			goto MoveBack;
		}
	}
	
	MoveFront:
	{
		geneticc_dma_create_subtransfer((list->array + list->tail) - range_size, (list->array + list->tail), front_size, transfer, NULL);
		list->tail -= range_size;
		goto CopyRange;
	}
	
	MoveBack:
	{
		geneticc_dma_create_subtransfer((list->array + back_offset) + range_size, (list->array + back_offset), back_size, transfer, NULL);
		list->head += range_size;
	}
	#endif
	

	byte_offset_t insert_offset;	//Same as back_offset
	
	CopyRange:
	insert_offset = list->tail + offset;
	
	geneticc_dma_create_subtransfer((list->array  + insert_offset), range, range_size, transfer, NULL);
	
	RegisterTransfer:
	return geneticc_dma_register_transfer(transfer);
}


/*
*	Inserts a value in to a list.
*
*	/param		list		Pointer to the list.
*	/param		value		Pointer to the value to insert starts.
*	/param		offset		The byte offset where the new values should be inserted at.
*	/param		elem_size	The size of the elements being inserted.
*
*	/returns	bool		True if the DMA transfer was registered. False if there was no capacity in the list, or if the maximum transfer count has been reached.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_dma_insert_memory(LIST_PTR list, const ELEMENT_PTR value, byte_offset_t offset)
{
	return internal_list_dma_insertRange_memory(list, value, offset, list->elem_size);
}

/*
*	Inserts a value in to a list.
*
*	/param		list		Pointer to the list.
*	/param		value		Pointer to the value to insert starts.
*	/param		offset		The byte offset where the new values should be inserted at.
*	/param		elem_size	The size of the elements being inserted.
*
*	/returns	bool		True if the DMA transfer was registered. False if there was no capacity in the list, or if the maximum transfer count has been reached.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_dma_insert_generic(LIST_PTR list, generic_union_t value, byte_offset_t offset)
{
	uint64_t v = value.u64;	//Store value so we can get pointer from.
	
	return internal_list_dma_insertRange_memory(list, &v, offset, list->elem_size);
}

/*
*	Adds a range of values on to the end of an list.
*
*	/param		list		Pointer to the list.
*	/param		range		Pointer to where the range of values to add starts.
*	/param		range_size	The size of the of elements being added
*	/param		elem_size	The size of the elements being added.
*
*	/returns	bool		True if the DMA transfer was registered. False if there was no capacity in the list, or if the maximum transfer count has been reached.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_dma_addRange_memory(LIST_PTR list, const ARRAY_PTR range, size_t range_size )
{
	return internal_list_dma_insertRange_memory(list, range, internal_list_get_size(list), range_size);
}

/*
*	Adds a values on to the end of an list.
*
*	/param		list		Pointer to the list.
*	/param		value		Pointer to where the value to add starts.
*	/param		elem_size	The size of the element being added.
*
*	/returns	bool		True if the DMA transfer was registered. False if there was no capacity in the list, or if the maximum transfer count has been reached.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_dma_add_memory(LIST_PTR list, const ELEMENT_PTR value )
{
	return internal_list_dma_insertRange_memory(list, value,  internal_list_get_size(list), list->elem_size);
}

/*
*	Adds a value to the end of an list.
*
*	/param		list		Pointer to the list.
*	/param		value		The value being added.
*	/param		elem_size	The size of the element being added.
*
*	/returns	bool		True if the DMA transfer was registered. False if there was no capacity in the list, or if the maximum transfer count has been reached.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_dma_add_generic(LIST_PTR list, generic_union_t value)
{
	uint64_t v = value.u64;	//Store value so we can get pointer from.
	
	return internal_list_dma_insertRange_memory(list, &v, internal_list_get_size(list), list->elem_size);
}

/*
*	Prepends a range of values on to the start of an list.
*
*	/param		list		Pointer to the list.
*	/param		range		Pointer to where the range of values to Prepends starts.
*	/param		range_size	The size of the of elements being added
*
*	/returns	bool		True if the DMA transfer was registered. False if there was no capacity in the list, or if the maximum transfer count has been reached.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_dma_prependRange_memory(LIST_PTR list, const ARRAY_PTR range, size_t range_size )
{
	return internal_list_dma_insertRange_memory(list, range, 0, range_size);
}

/*
*	Prepends a value to the end of an list.
*
*	/param		list		Pointer to the list.
*	/param		value		The value being added.
*	/param		elem_size	The size of the element being added.
*
*	/returns	bool		True if the DMA transfer was registered. False if there was no capacity in the list, or if the maximum transfer count has been reached.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_dma_prepend_memory(LIST_PTR list, const ELEMENT_PTR value )
{
	return internal_list_dma_insertRange_memory(list, value, 0, list->elem_size);
}

/*
*	Prepends a value to the end of an list.
*
*	/param		list		Pointer to the list.
*	/param		value		The value being added.
*	/param		elem_size	The size of the element being added.
*
*	/returns	bool		True if the DMA transfer was registered. False if there was no capacity in the list, or if the maximum transfer count has been reached.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_dma_prepend_generic(LIST_PTR list, generic_union_t value)
{
	uint64_t v = value.u64;	//Store value so we can get pointer from.

	return internal_list_dma_insertRange_memory(list, &v, 0, list->elem_size);
}

/*
*	Removes a range of values from an list.
*
*	/param	list		Pointer to the list.
*	/param	offset		The byte offset to start removing at.
*	/param	byte_count	The size of the elements being removed.
*
*	/returns	bool	True if the DMA transfer was registered. False if the maximum transfer count has been reached.
*/
bool internal_list_dma_removeRange_memory(LIST_PTR list, byte_offset_t offset, size_t byte_count )
{
	if(!geneticc_dma_accepting_transfers())
	return false;
	
	
	ASSERT(byte_count && byte_count % list->elem_size == 0);
	
	//size_t byte_count = length * list->elem_size;
	byte_offset_t aligned_offset = list->tail + offset;
	size_t current_size = internal_list_get_size(list);
	
	geneticc_dma_transfer_t* transfer = geneticc_dma_create_transfer(list, list, current_size, false);
	
	if(offset == 0)
	{
		//Removing elements from the front of the array.
		list->tail += byte_count;
	}
	else if(aligned_offset == list->head)
	{
		//Removing elements from the back of the array.
		list->head -= byte_count;
	}
	else
	{
		//Removing elements from the middle of the array.
		
		size_t max_tail = (LIST_MAX_TAIL_BEFORE_REALIGN * list->elem_size);
		bool alignArray = max_tail < list->tail;
		
		#if GENETICC_OPTIMIZATION_LEVEL != GENETICC_OPTIMIZATION_LEVEL_SIZE
		
		size_t front_size = offset;
		size_t back_size = current_size - front_size;
		
		bool front_smaller = front_size < back_size;
		int size_threshold = front_size - back_size;
		bool threshold_met = size_threshold > LIST_COPY_FRONT_BACK_SIZE_THRESHOLD || size_threshold < -LIST_COPY_FRONT_BACK_SIZE_THRESHOLD;
		
		
		
		
		if(alignArray)
		{
			//Adjust the internal array so the tail is at index 0.
			geneticc_dma_create_subtransfer(list->array, list->array + list->tail + front_size, front_size, transfer, NULL);
			geneticc_dma_create_subtransfer(list->array + front_size, list->array + aligned_offset + byte_count, back_size, transfer, NULL);
			list->head -= list->tail - byte_count;
			list->tail = 0;
		}
		else
		{
			
			//Check if the front or back is smaller, so we can move the minimum amount of data to achieve the removing of elements.
			if(front_smaller && threshold_met && list->tail + byte_count <= max_tail)
			{
				//Only adjust the front of the array.
				geneticc_dma_create_subtransfer(list->array + list->tail + byte_count, list->array + list->tail, front_size, transfer, NULL);
				list->tail += byte_count;
			}
			else
			{
				//Only adjust the back of the array.
				geneticc_dma_create_subtransfer(list->array + aligned_offset, list->array + aligned_offset + byte_count, back_size, transfer, NULL);
				list->head -= byte_count;
			}
			
		}
		
		#else
		//Always move the back of the array.
		size_t back_size =  internal_list_get_size(list) - offset;
		geneticc_dma_create_subtransfer(list->array + aligned_offset, list->array + aligned_offset + byte_count, back_size, transfer, NULL);
		list->head -= byte_count;
		#endif
		
		return geneticc_dma_register_transfer(transfer);
	}
}

/*
*	Removes a values from an list by its byte offset.
*
*	/param	list		Pointer to the list.
*	/param	offset		The byte offset to start removing at.
*
*	/returns	bool	True if the DMA transfer was registered. False if the maximum transfer count has been reached.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_dma_removeAt_memory(LIST_PTR list, byte_offset_t offset )
{
	return internal_list_dma_removeRange_memory(list, offset, list->elem_size);
}


/*
*	Removes the first item from an list which equals "value."
*
*	/param	list			Pointer to the list.
*	/param	value			A pointer to the value to remove.
*	/param	*out_removed	True or false depending off if a value was found and removed.
*
*	/returns	bool		True if the DMA transfer was registered, or if there were no values to remove, out_removed will also be false. When False and out_removed true, the maximum transfer count has been reached.
*/
bool internal_list_dma_remove_memory(LIST_PTR list, const ELEMENT_PTR value, bool* out_removed )
{
	//#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	//Element index
	ARRAY_PTR array = internal_list_get_aligned_array(list);
	size_t size = internal_list_get_size(list);
	int index = internal_array_indexOf_memory(array, value, size, list->elem_size);
	
	//Value not in list, nothing to remove.
	if(index == -1)
	{
		if(out_removed != NULL)
		*out_removed = false;
		
		return true;
	}
	
	if(out_removed != NULL)
	*out_removed = true;

	return internal_list_dma_removeAt_memory(list, index * list->elem_size);
	//#endif
}


/*
*	Removes the first item from an list which equals "value."
*
*	/param	list			Pointer to the list.
*	/param	value			A pointer to the value to remove.
*	/param	*out_removed	True or false depending off if a value was found and removed.
*
*	/returns	bool		True if the DMA transfer was registered, or if there were no values to remove, out_removed will also be false. When False and out_removed true, the maximum transfer count has been reached.
*/
bool internal_list_dma_remove_generic(LIST_PTR list, generic_union_t value, bool* out_removed)
{
	uint64_t v = value.u64;	//Store value so we can get pointer from.
	
	return internal_list_dma_remove_memory(list, &v, out_removed);
}


/*
*	Removes all items from an list which equals "value."
*
*	/param	list				Pointer to the start of the list.
*	/param	value				A pointer to the value which is used to determine which elements to remove.
*	/param	*out_remove_count	(out)A count of how many elements were removed.
*		
*	/returns	bool		True if the DMA transfer was registered, or if there were no values to remove, out_remove_count will also be 0. When False and out_remove_count > 0, the maximum transfer count has been reached.
*/
bool internal_list_dma_removeAll_memory(LIST_PTR list, const ELEMENT_PTR value, uint32_t* out_remove_count)
{
	size_t size = internal_list_get_size(list);
	geneticc_dma_transfer_t* transfer = geneticc_dma_create_transfer(list, list, size, false);
	
	uint32_t remove_count = 0;	
	byte_offset_t i_offset = 0;
	
	
	
	//#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	//
	//int index = 0;
	//do
	//{
		//index = List_Indexof(list, value, index);
		//
		//if(index != -1)
		//{
			//remove_count ++;
//
			//geneticc_dma_create_subtransfer(/*Dest*/list->array + (index * list->elem_size) + list->tail - i_offset, /*Src*/ list->array + (index * list->elem_size) + list->tail - i_offset + list->elem_size,  size - ((index * list->elem_size) - i_offset - list->tail)  /*n*/, transfer, NULL);
			//i_offset += list->elem_size;
		//}
	//} while (index != -1);
	//
	//#else
	
	for(int i = list->tail; i < list->head; i += list->elem_size)
	{
		uint8_t b;
		RedoIndex:
		b = 0;
		
		for(b; b < list->elem_size; b++)
		{
			if(list->array[i + b] != value[b])
			{
				break;
			}
		}
		
		
		
		if(b == list->elem_size)
		{
			remove_count++;
			
			#if GENETICC_OPTIMIZATION_LEVEL != GENETICC_OPTIMIZATION_LEVEL_SIZE
			if(i < size / 2)
			{
				geneticc_dma_create_subtransfer(/*Dest*/ list->array + list->tail + list->elem_size, /*Src*/ list->array + list->tail,  /*n*/i - i_offset - list->tail, transfer, NULL);
				//geneticc_memcpyovrlp(/*Dest*/ list->array + list->tail + list->elem_size, /*Src*/ list->array + list->tail,  /*n*/ i - list->tail);
				list->tail += list->elem_size;
				
			}
			else
			{
				geneticc_dma_create_subtransfer(list->array + i - i_offset, list->array + i - i_offset + list->elem_size, size - (i - i_offset - list->tail), transfer, NULL);
				//memcpy(list->array + i, list->array + i + list->elem_size, size - (i - list->tail));
				list->head -= list->elem_size;
				
				#warning !!! need to test
				//Redo the current index, as the next value is now in its place.
				i_offset += list->elem_size;
				//goto RedoIndex;
			}
			#else
			geneticc_dma_create_subtransfer(list->array + i - i_offset, list->array + i - i_offset + list->elem_size, size - (i - i_offset - list->tail), transfer, NULL);
			//memcpy(list->array + i, list->array + i + list->elem_size, size - (i - list->tail));
			list->head -= list->elem_size;
			
			#warning !!! need to test
			//Redo the current index, as the next value is now in its place.
			i_offset += list->elem_size;
			//goto RedoIndex;
			#endif
		}
		

	}
	
	//#endif
	
	if(remove_count == 0)
	*out_remove_count = remove_count;
	
	return geneticc_dma_register_transfer(transfer);
	
}

/*
*	Removes all items from an list which equals "value."
*
*	/param	list			Pointer to the start of the list.
*	/param	value			The value which is used to determine which elements to remove.
*	/param	*out_remove_count	(out)A count of how many elements were removed.
*
*	/returns	bool			True if the DMA transfer was registered. False if the maximum transfer count has been reached.
*/
bool internal_list_dma_removeAll_generic(LIST_PTR list, generic_union_t value, uint32_t* out_remove_count )
{
	uint64_t v = value.u64;	//Store value so we can get pointer from.
	
	return internal_list_dma_removeAll_memory(list, &v, out_remove_count);
}


/*
*	Removes the first item from an list which equals "value."
*
*	/param	list			Pointer to the list.
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*	/param	*out_removed	True or false depending off if a value was found and removed.
*
*	/returns	bool		True if the DMA transfer was registered, or if there were no values to remove, out_removed will also be false. When False and out_removed true, the maximum transfer count has been reached.
*/
bool internal_list_dma_removeMatch_memory(LIST_PTR list, PREDICATE predicate, bool* out_removed )
{
	//#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	//Element index
	ARRAY_PTR array = internal_list_get_aligned_array(list);
	size_t size = internal_list_get_size(list);
	int index = internal_array_select_memory(array, size, list->elem_size, predicate);
	
	//Value not in list, nothing to remove.
	if(index == -1)
	{
		if(out_removed != NULL)
		*out_removed = false;
		
		return true;
	}
	
	if(out_removed != NULL)
	*out_removed = true;

	return internal_list_dma_removeAt_memory(list, index * list->elem_size);
	//#endif
}


/*
*	Removes all items from an list which equals "value."
*
*	/param	list				Pointer to the start of the list.
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*	/param	*out_remove_count	(out)A count of how many elements were removed.
*
*	/returns	bool		True if the DMA transfer was registered, or if there were no values to remove, out_remove_count will also be 0. When False and out_remove_count > 0, the maximum transfer count has been reached.
*/
bool internal_list_dma_removeAllMatching_memory(LIST_PTR list, PREDICATE predicate, uint32_t* out_remove_count)
{
	size_t size = internal_list_get_size(list);
	
	geneticc_dma_transfer_t* transfer = geneticc_dma_create_transfer(list, list, size, false);
	
	uint32_t remove_count = 0;
	byte_offset_t i_offset = 0;
	
	for(int i = list->tail; i < list->head; i += list->elem_size)
	{
		RedoIndex:

		if((*predicate)(list->array + i))
		{
			remove_count++;
			
			#if GENETICC_OPTIMIZATION_LEVEL != GENETICC_OPTIMIZATION_LEVEL_SIZE
			if(i < size / 2)
			{
				geneticc_dma_create_subtransfer(/*Dest*/ list->array + list->tail + list->elem_size, /*Src*/ list->array + list->tail,  /*n*/i - i_offset - list->tail, transfer, NULL);
				//geneticc_memcpyovrlp(/*Dest*/ list->array + list->tail + list->elem_size, /*Src*/ list->array + list->tail,  /*n*/ i - list->tail);
				list->tail += list->elem_size;
				
			}
			else
			{
				geneticc_dma_create_subtransfer(list->array + i - i_offset, list->array + i - i_offset + list->elem_size, size - (i - i_offset - list->tail), transfer, NULL);
				//memcpy(list->array + i, list->array + i + list->elem_size, size - (i - list->tail));
				list->head -= list->elem_size;
				
				#warning !!! need to test
				//Redo the current index, as the next value is now in its place.
				i_offset += list->elem_size;
				//goto RedoIndex;
			}
			#else
			geneticc_dma_create_subtransfer(list->array + i - i_offset, list->array + i - i_offset + list->elem_size, size - (i - i_offset - list->tail), transfer, NULL);
			//memcpy(list->array + i, list->array + i + list->elem_size, size - (i - list->tail));
			list->head -= list->elem_size;
			
			#warning !!! need to test
			//Redo the current index, as the next value is now in its place.
			i_offset += list->elem_size;
			//goto RedoIndex;
			#endif
		}
		

	}
	
	if(remove_count == 0)
	*out_remove_count = remove_count;
	
	return geneticc_dma_register_transfer(transfer);
	
}


/*
*	Removes the first item from an list which equals "value."
*
*	/param	list			Pointer to the list.
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*	/param	*out_removed	True or false depending off if a value was found and removed.
*	/param	arg_count		How many arguments have been passed.
*	/param	ap				The variadic arguments. *Will call va_end before returning.
*
*	/returns	bool		True if the DMA transfer was registered, or if there were no values to remove, out_removed will also be false. When False and out_removed true, the maximum transfer count has been reached.
*/
bool internal_list_dma_removeMatch_vargs_memory(LIST_PTR list, PREDICATE_ARGS predicate, bool* out_removed, int arg_count, va_list ap )
{
	ARRAY_PTR array = internal_list_get_aligned_array(list);
	size_t size = internal_list_get_size(list);
	int index = internal_array_select_vargs_memory(array, size, list->elem_size, predicate, arg_count, ap);
	va_end(ap);
	
	//Value not in list, nothing to remove.
	if(index == -1)
	{
		if(out_removed != NULL)
		*out_removed = false;
		
		return true;
	}
	
	if(out_removed != NULL)
	*out_removed = true;

	return internal_list_dma_removeAt_memory(list, index * list->elem_size);
}


/*
*	Removes all items from an list which equals "value."
*
*	/param	list				Pointer to the start of the list.
*	/param	predicate			A pointer to a function which checks each value to its conditions.
*	/param	*out_remove_count	(out)A count of how many elements were removed.
*	/param	arg_count			How many arguments have been passed.
*	/param	ap					The variadic arguments. *Will call va_end before returning.
*
*	/returns	bool		True if the DMA transfer was registered, or if there were no values to remove, out_remove_count will also be 0. When False and out_remove_count > 0, the maximum transfer count has been reached.
*/
bool internal_list_dma_removeAllMatching_vargs_memory(LIST_PTR list, PREDICATE_ARGS predicate, uint32_t* out_remove_count, int arg_count, va_list ap)
{
	size_t size = internal_list_get_size(list);
	
	geneticc_dma_transfer_t* transfer = geneticc_dma_create_transfer(list, list, size, false);
	
	uint32_t remove_count = 0;
	byte_offset_t i_offset = 0;
	
	for(int i = list->tail; i < list->head; i += list->elem_size)
	{
		RedoIndex:

		if((*predicate)(list->array + i, arg_count, ap))
		{
			remove_count++;
			
			#if GENETICC_OPTIMIZATION_LEVEL != GENETICC_OPTIMIZATION_LEVEL_SIZE
			if(i < size / 2)
			{
				geneticc_dma_create_subtransfer(/*Dest*/ list->array + list->tail + list->elem_size, /*Src*/ list->array + list->tail,  /*n*/i - i_offset - list->tail, transfer, NULL);
				//geneticc_memcpyovrlp(/*Dest*/ list->array + list->tail + list->elem_size, /*Src*/ list->array + list->tail,  /*n*/ i - list->tail);
				list->tail += list->elem_size;
				
			}
			else
			{
				geneticc_dma_create_subtransfer(list->array + i - i_offset, list->array + i - i_offset + list->elem_size, size - (i - i_offset - list->tail), transfer, NULL);
				//memcpy(list->array + i, list->array + i + list->elem_size, size - (i - list->tail));
				list->head -= list->elem_size;
				
				#warning !!! need to test
				//Redo the current index, as the next value is now in its place.
				i_offset += list->elem_size;
				//goto RedoIndex;
			}
			#else
			geneticc_dma_create_subtransfer(list->array + i - i_offset, list->array + i - i_offset + list->elem_size, size - (i - i_offset - list->tail), transfer, NULL);
			//memcpy(list->array + i, list->array + i + list->elem_size, size - (i - list->tail));
			list->head -= list->elem_size;
			
			#warning !!! need to test
			//Redo the current index, as the next value is now in its place.
			i_offset += list->elem_size;
			//goto RedoIndex;
			#endif
		}
		

	}
	
	if(remove_count == 0)
	*out_remove_count = remove_count;
	
	va_end(ap);
	
	return geneticc_dma_register_transfer(transfer);
	
}

/*
*	Removes the first item from an list which equals "value."
*
*	/param	list			Pointer to the list.
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*	/param	*out_removed	True or false depending off if a value was found and removed.
*	/param	arg_count		How many arguments have been passed.
*	/param	...				The variadic arguments.
*
*	/returns	bool		True if the DMA transfer was registered, or if there were no values to remove, out_removed will also be false. When False and out_removed true, the maximum transfer count has been reached.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_dma_removeMatch_args_memory(LIST_PTR list, PREDICATE_ARGS predicate, bool* out_removed, int arg_count, ... )
{
	va_list ap;
	va_start(ap, arg_count);
	return internal_list_dma_removeMatch_vargs_memory(list, predicate, out_removed, arg_count, ap);
}


/*
*	Removes all items from an list which equals "value."
*
*	/param	list				Pointer to the start of the list.
*	/param	predicate			A pointer to a function which checks each value to its conditions.
*	/param	*out_remove_count	(out)A count of how many elements were removed.
*	/param	arg_count			How many arguments have been passed.
*	/param	...					The variadic arguments.
*
*	/returns	bool		True if the DMA transfer was registered, or if there were no values to remove, out_remove_count will also be 0. When False and out_remove_count > 0, the maximum transfer count has been reached.
*/
bool GENOPTI_ATT_FORCE_INLINE internal_list_dma_removeAllMatching_args_memory(LIST_PTR list, PREDICATE_ARGS predicate, uint32_t* out_remove_count, int arg_count, ...)
{
	va_list ap;
	va_start(ap, arg_count);
	return internal_list_dma_removeAllMatching_vargs_memory(list, predicate, out_remove_count, arg_count, ap);
}

#endif
#pragma endregion DMA Functions