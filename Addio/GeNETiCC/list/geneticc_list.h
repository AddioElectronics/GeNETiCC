/*
	-Generic List-
	
	A C# like list implementation which keeps track of its size, and includes macros/functions for adding, inserting, removing...
	
	*Uses more memory than a standard array, but will drastically increase performance if you are using the "unsafe" functions, over the "geneticc_array.h" functions.
*/



#ifndef GENETICC_LIST_H_
#define GENETICC_LIST_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

#include "../geneticc.h"
#include "../array/geneticc_array.h"
#include "geneticc_list_config.h"
#include "internal/geneticc_list_internal_macros.h"

#pragma region Initialization Macros

/*
*	Allocates and Initializes a list.
*
*	new_List(elem_size)
*	new_List(elem_size, capacity)
*
*	/param	elem_size		The size of the value's type (in bytes).
*	/param	capacity		(Optional)The amount of elements that can be stored in the list.
*
*	/Returns list_t	The newly allocated list.
*/
#define new_List(...) LIST_NEW_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Initializes a list
*
*	List_New(out_list, elem_size)
*	List_New(out_list, elem_size, capacity)
*
*	/param	*out_list	(Out)The initialized list.
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	capacity	(Optional)The amount of elements that can be stored in the list.
*
*/
#define List_Init(...) LIST_INIT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Initializes a list from an already existing array.
*
*	Array_To_List(array)
*	Array_To_List(array, length)
*	Array_To_List(array, length, populated_length)
*	Array_To_List(array, length, populated_length, elem_size)
*
*	/param	array				The initial size of the list. (How many values can be stored)
*	/param	length				(Optional)The length of the array.	*Required for pointer arrays.
*	/param	populated_length	(Optional)The count of elements currently in the array.
*	/param	elem_size			The size of the value's type (in bytes).
*
*	/Returns LIST_PTR			The newly allocated list.
*/
#define Array_To_List(...) ARRAY_TO_LIST_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Allocates and Initializes a list from an already existing array.
*	Copies the array into a new block of memory. (Use Array_To_list to use the same array)
*
*	Array_CopyTo_List(array)
*	Array_CopyTo_List(array, length)
*	Array_CopyTo_List(array, length, populated_length)
*	Array_CopyTo_List(array, length, elem_size, populated_length)
*
*	/param	array				The initial size of the list. (How many values can be stored)
*	/param	length				(Optional)The length of the array.		*Required for pointer arrays.
*	/param	populated_length	(Optional)The count of elements currently in the array.
*	/param	elem_size			The size of the value's type (in bytes).
*
*	/Returns list_t		The newly allocated list.
*/
#define Array_CopyTo_List(...) ARRAY_COPYTO_LIST_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Initializes a list from an already existing array.
*
*	Array_Init_List(array, out_list)
*	Array_Init_List(array, length, out_list)
*	Array_Init_List(array, length, populated_length, out_list)
*	Array_Init_List(array, length, populated_length, elem_size, out_list)
*
*	/param	array				The initial size of the list. (How many values can be stored)
*	/param	length				(Optional)The length the array was created with.	*Required for pointer arrays.
*	/param	populated_length	(Optional)The count of elements currently in the array.
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	*out_list			(Out)The initialized list.
*
*/
#define Array_Init_List(...) ARRAY_INIT_LIST_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Initializes a list, and copies an already existing array to the list.
*
*	Array_CopyInit_List(array, out_list)
*	Array_CopyInit_List(array, length, out_list)
*	Array_CopyInit_List(array, length, populated_length, out_list)
*	Array_CopyInit_List(array, length, populated_length, elem_size, out_list)
*
*	/param	array				The initial size of the list. (How many values can be stored)
*	/param	length				(Optional)The length the array was created with.	*Required for pointer arrays.
*	/param	populated_length	(Optional)The count of elements currently in the array.
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	*out_list			(Out)The initialized list.
*
*/
#define Array_CopyInit_List(...) ARRAY_COPY_INIT_LIST_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Frees a list and its internal array from memory.
*
*	List_Delete(list)
*
*	/param		list			Pointer to the list.
*/
#define List_Delete(list)		internal_list_delete(list)


#pragma endregion Initialization Macros

#pragma region Get/Set Macros

/*
*	Gets a pointer to the tail of the array.
*
*	List_Array(list)
*
*	/param		list			Pointer to the list.
*
*	/Returns	LIST_PTR		Pointer to the tail of the internal array.
*/
#define List_Array(list)	internal_list_get_aligned_array(list)
//#define List(list)			internal_list_get_aligned_array(list)
//#define Larry(list)			internal_list_get_aligned_array(list)

/*
*	Returns a pointer to the value at the index.
*	Checks for index out of range, and adjusts the index relative to the tail.
*
*	List_Get(list)
*
*	/param		list		Pointer to the list.
*	/param		index		Index of the value to retrieve.
*
*	/returns	ELEMENT_PTR		Pointer to the element at the index, or null if the index was out of range.
*/
#define List_Get(list, index)	internal_list_get(list, index)

/*
*	Gets a value which resides at the specified index in the list.
*	*Standard value types only.
*
*	List_Get_Std(list, index, type)
*
*	/param		list			Pointer to the list.
*	/param		index			Index of the value.
*	/param		type			A value used to retrieve the type.
*
*	/returns					The value residing at the index.
*/
#define List_Get_Std(list, index, type)		GENETICC_DEREFERENCE(internal_list_get(list, index), type)

/*
*	Sets the element at the index to the value.
*
*	List_Set(list, value, index)
*
*	/param		list		Pointer to the list.
*	/param		value		Value to set.
*	/param		index		Index being set to value.
*
*	/returns	bool		True if the value was set, or null if the index was out of range.
*/
#define List_Set(list, value, index)	INTERNAL_LIST_SET(list, value, index)


/*
*	Either gets a value from the index, or sets the value at the index.
*
*	List_Val(list, index)				/returns	ELEMENT_PTR		Pointer to the element at the index, or null if the index was out of range.
*	List_Val(list, value, index)		/returns	bool			True if the value was set, or null if the index was out of range.
*
*	/param		list			Pointer to the list.
*	/param		value			Value or pointer to the value to set.
*	/param		index			Index being set to value.
*/
#define List_Val(...)			LIST_VAL_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#pragma endregion Get/Set Macros

#pragma region Size and Capacity Macros



/*
*	Either returns the current capacity, or sets a new capacity for the list.
*
*	capacity_count_t	List_Capacity(list)				=	Gets the capacity	/Returns	Length of how many elements can fit in the internal array.
*	bool				List_Capacity(list, capacity)	=	Sets the capacity	/Returns	Returns true if the capacity was or was not changed, or false if the capacity would be unable to store the current array.
*
*	/param		list		Pointer to the list.
*	/param		capacity	(Optional)The new capacity (Element count).
*/
#define List_Capacity(...)	LIST_CAPACITY_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Gets the capacity represented as bytes.
*
*	List_Get_CapacitySize(list)
*
*	/param		list		Pointer to the list.
*/
#define List_Get_CapacitySize(list)	list->capacity


/*
*	Sets a new capacity for the list.
*	
*	List_Set_Capacity(list)
*
*	/param		list		Pointer to the list.
*	/param		capacity	The new capacity (Element count).
*
*	/Returns	bool		Returns true if the capacity was or was not changed, or false if the capacity would be unable to store the current array.
*/
#define List_Set_Capacity(list, capacity)	internal_list_set_capacity(list, capacity)

/*
*	Gets the capacity of of the list, represented as element count.
*
*	List_Get_Capacity(list)
*
*	/param		list					Pointer to the list.
*
*	/Returns	capacity_count_t		Length of how many elements can fit in the internal array.
*/
#define List_Get_Capacity(list)	internal_list_get_capacity(list)


/*
*	Gets the total size of elements in the list.
*
*	List_Size(list)
*
*	/param	list			Pointer to the list.
*
*	/Returns	uint		Size of the elements in the list.
*/
#define List_Size(list)	internal_list_get_count(list)

/*
*	Gets the total count of elements in the list.
*
*	List_Count(list)
*
*	/param		list		Pointer to the list.
*
*	/Returns	uint		Count of the elements in the list.
*/
#define List_Count(list)	internal_list_get_count(list)


/*
*	Moves the elements in the array so the tail is at index 0.
*
*	List_Realign(list)
*
*	/param		list		Pointer to the list.
*/
#define List_Realign(list)	internal_list_realign_array(list)

/*
*	Realigns the internal array, sets the capacity to the size of the array,
*	and then reallocates the array.
*
*	List_TrimExcess(list)
*
*	/param		list		Pointer to the list.
*/
#define List_TrimExcess(list)	internal_list_trimExcess(list)



/*
*	Moves the head, changing the size of the internal array.
*	*Positive value will add values, negative will remove values.
*	*Nothing will happen if the head will be less than the tail, or greater than the capacity.
*
*	List_AdjustHead(list, n)
*	List_AdjustHead(list, n, erase)
*
*	/param	list			Pointer to the list.
*	/param	n				How many elements to add/remove.
*	/param	erase			(Optional, Default=false)Should the memory be set to 0?
*
*	/Returns	uint		The new size of the list.
*/
#define List_AdjustHead(...)	LIST_ADJUSTHEAD_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Moves the tail, changing the size of the internal array.
*	*Positive value will remove values, negative will add values.
*	*Nothing will happen if the tail will be less than zero, or greater than the head.
*
*	List_AdjustTail(list, n)
*	List_AdjustTail(list, n, erase)
*
*	/param	list			Pointer to the list.
*	/param	n				How many elements to add/remove.
*	/param	erase			(Optional, Default=false)Should the memory be set to 0?
*
*	/Returns	uint		The new size of the list.
*/
#define List_AdjustTail(...)	LIST_VAL_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


// List_AdjustHead/Tail does the same thing in a safer way.
///*
//*	List_TrimEnd(list, n)
//*
//*	/param	list			Pointer to the list
//*	/param	n				The amount of values to trim from the end.
//*/
//#define List_TrimEnd(list, n)	list->head - (n * INTERNAL_LIST_GET_ELEM_SIZE(list))
//
///*
//*	List_TrimStart(list, n)
//*
//*	/param	list			Pointer to the list
//*	/param	n				The amount of values to trim from the start.
//*/
//#define List_TrimStart(list, n)	list->tail + (n * INTERNAL_LIST_GET_ELEM_SIZE(list))

#pragma endregion Size and Capacity Macros

#pragma region Macros


/*
*	Searches n list for a value, and confirms its existence.
*
*	List_Contains(list, value )
*	List_Contains(list, value, index)
*	List_Contains(list, value, index, length)
*
*	/param	list		Pointer to the list
*	/param	value		The value to search for.
*	/param	start		(Optional)The index to start searching from.
*	/param	length		(Optional)The amount of values to search.
*
*	/returns	bool	If the list contains the value.
*/
#define List_Contains(...) LIST_CONTAINS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Search an list for a specific value, and returns the first index.
*
*	List_IndexOf(list, value)
*	List_IndexOf(list, value,  start)
*	List_IndexOf(list, value,  start, length)
*
*	/param	list		Pointer to the list
*	/param	value		The value to search for.
*	/param	start		(Optional)The index to start searching from.
*	/param	length		(Optional)The amount of values to search.
*
*	/returns	int		The index to the first item in the list matching value, or -1 if it does not exist.
*/
#define List_IndexOf(...) LIST_INDEXOF_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an list from back to front for a value, and returns the index of the first match (latest index).
*
*	List_LastIndexOf(list, value)
*	List_LastIndexOf(list, value, index)
*	List_LastIndexOf(list, value, index, length)
*
*	/param	list		Pointer to the list
*	/param	value		The value to search for.
*	/param	start		(Optional)The earliest index in the search range.
*	/param	length		(Optional)The amount of values to search.
*
*	/returns	int		The index to the last item in the list matching value, or -1 if it does not exist.
*/
#define List_LastIndexOf(...) LIST_LASTINDEXOF_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Counts how many times "value" is seen in the list.
*
*	List_ValueCount(list, value)
*	List_ValueCount(list, value, index)
*	List_ValueCount(list, value, index, length)
*
*	/param	list		Pointer to the list
*	/param	value		The value to search for.
*	/param	start		(Optional)The index to start searching from.
*	/param	length		(Optional)The amount of values to search.
*
*	/returns	uint	The amount of times "value" is in the list.
*/
#define List_ValueCount(...) LIST_VALUECOUNT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an list for the largest value, and returns its index.
*
*	List_Max(list)
*	List_Max(list, index)
*	List_Max(list, index, length)
*
*	/param	list		Pointer to the list
*	/param	start		(Optional)The index to start at.
*	/param	length		(Optional)The amount of values to search.
*
*	/returns	uint	The index of the largest value in the list.
*/
#define List_Max(...) LIST_MAX_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an list for the largest value, and returns the value.
*
*	List_Max_Val(list)
*	List_Max_Val(list, index)
*	List_Max_Val(list, index, length)
*
*	/param	list		Pointer to the list
*	/param	start		(Optional)The index to start at.
*	/param	length		(Optional)The amount of values to search.
*
*	/returns	generic		The largest value in the list.
*/
#define List_Max_Val(...) LIST_MAX_VALUE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an list for the smallest value, and returns its index.
*
*	List_Min(list)
*	List_Min(list, index)
*	List_Min(list, index, length)
*
*	/param	list		Pointer to the list
*	/param	start		(Optional)The index to start at.
*	/param	length		(Optional)The amount of values to search.
*
*	/returns	uint	The index of the smallest value in the list.
*/
#define List_Min(...) LIST_MIN_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an list for the smallest value, and returns the value
*
*	List_Min_Val(list)
*	List_Min_Val(list, index)
*	List_Min_Val(list, index, length)
*
*	/param	list			Pointer to the list
*	/param	start			(Optional)The index to start at.
*	/param	length		(Optional)The amount of values to search.
*
*	/returns	generic		The smallest value in the list.
*/
#define List_Min_Val(...) LIST_MIN_VALUE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Gets the sum of all values in the list.
*
*	List_Sum(list)
*	List_Sum(list, index)
*	List_Sum(list, index, length)
*
*	/param	list			Pointer to the list
*	/param	index			(Optional)The index to start at.
*	/param	length		(Optional)The amount of values to search.
*
*	/returns	generic		The sum of all values in the list.
*/
#define List_Sum(...) LIST_SUM_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Gets the average of all values in the list.
*
*	List_Average(list)
*	List_Average(list, index)
*	List_Average(list, index, length)
*
*	/param	list			Pointer to the list
*	/param	index			(Optional)The index to start at.
*	/param	length			(Optional)The amount of values to search.
*
*	/returns	generic		The index of the smallest value in the list.
*/
#define List_Average(...) LIST_AVERAGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Enumerates each element in the list, and performs an action on them.
*
*	List_ForEach(list, action)
*	List_ForEach(list, action,index)
*	List_ForEach(list, action,index, length)
*
*	/param		list	LIST_PTR	Pointer to the list.
*	/param		action	ACTION		Function pointer of type ACTION.
*	/param		index	uint		(Optional, default=0)Zero based index, at which to start at.
*	/param		length	uint		(Optional, default=0(ALL))How many elements after the index to apply the action to.
*/
#define List_ForEach(...)		LIST_FOREACH_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Determines whether all elements of a sequence satisfy a condition.
*
*	List_All(list, predicate)
*	List_All(list, predicate, index)
*	List_All(list, predicate, index, length)
*
*	/param		list		LIST_PTR	Pointer to the list.
*	/param		predicate	PREDICATE	Function pointer of type PREDICATE.
*	/param		index		uint		(Optional, default=0)Zero based index, at which to start at.
*	/param		length		uint		(Optional, default=0(ALL))How many elements after the index to apply the action to.
*/
#define List_All(...)		LIST_ALL_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Determines whether any element of a sequence exists or satisfies a condition.
*
*	List_Any(list, action)
*	List_Any(list, action,index)
*	List_Any(list, action,index, length)
*
*	/param		list	LIST_PTR	Pointer to the list.
*	/param		action	ACTION		Function pointer of type ACTION.
*	/param		index	uint		(Optional, default=0)Zero based index, at which to start at.
*	/param		length	uint		(Optional, default=0(ALL))How many elements after the index to apply the action to.
*/
#define List_Any(...)		LIST_ANY_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Retrieves the index to the first value in the list that matches the predicate.
*
*	List_Select(list, predicate)
*	List_Select(list, predicate, index)
*	List_Select(list, predicate, index, length)
*
*	/param		list			Pointer to the list.
*	/param		predicate		A pointer to a function (PREDICATE) which checks each value to its conditions. If arg_count and ap are used, predicate is type PREDICATE_ARGS.
*	/param		index			(Optional)The index to start at.
*	/param		length			(Optional)The amount of values to search.
*
*	/returns	int				Zero based index of the first value in the array that matches the predicate, or -1 if the value does not exist.
*/
#define List_Select(...)	LIST_SELECT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Retrieves a pointer to the first value in the list that matches the predicate.
*
*	List_SelectArgs(list, predicate, arg_count, ...)
*
*	/param		list			Pointer to the list.
*	/param		predicate		A pointer to a function (PREDICATE) which checks each value to its conditions.
*	/param		arg_count		How many arguments have been passed.
*	/param		...				The variadic arguments.
*
*	/returns	uint8_t*	A pointer to the first value in the array that matches the predicate
*/
#define List_SelectArgs(list, predicate, arg_count, ...) internal_list_select_args_memory(list, predicate, arg_count, ##__VA_ARGS__)

/*
*	Retrieves a pointer to the first value in the list that matches the predicate.
*
*	List_SelectVargs(list, predicate, arg_count, ap)
*	List_SelectVargs(list, predicate, arg_count, ap, index)
*	List_SelectVargs(list, predicate, arg_count, ap, index, length)
*
*	/param		list			Pointer to the list.
*	/param		predicate		A pointer to a function (PREDICATE) which checks each value to its conditions.
*	/param		arg_count		How many arguments have been passed.
*	/param		ap				The variadic arguments. *Will call va_end before returning.
*	/param		index			(Optional)The index to start at.
*	/param		length			(Optional)The amount of values to search.
*
*	/returns	uint8_t*	A pointer to the first value in the array that matches the predicate
*/
#define List_SelectVargs(...)	LIST_SELECT_VARGS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Retrieves a pointer to the first value in the list that matches the predicate.
*
*	List_SelectPointer(list, predicate)
*	List_SelectPointer(list, predicate, index)
*	List_SelectPointer(list, predicate, index, length)
*
*	/param		list			Pointer to the list.
*	/param		predicate		A pointer to a function (PREDICATE) which checks each value to its conditions. If arg_count and ap are used, predicate is type PREDICATE_ARGS.
*	/param		index			(Optional)The index to start at.
*	/param		length			(Optional)The amount of values to search.
*
*	/returns	uint8_t*	A pointer to the first value in the array that matches the predicate
*/
#define List_SelectPointer(...)	LIST_SELECTPOINTER_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Retrieves a pointer to the first value in the list that matches the predicate.
*
*	List_SelectPointerArgs(list, predicate, arg_count, ...)
*
*	/param		list			Pointer to the list.
*	/param		predicate		A pointer to a function (PREDICATE) which checks each value to its conditions.
*	/param		arg_count		How many arguments have been passed.
*	/param		...				The variadic arguments.
*
*	/returns	uint8_t*	A pointer to the first value in the array that matches the predicate
*/
#define List_SelectPointerArgs(list, predicate, arg_count, ...) internal_list_get(list, internal_list_select_args_memory(list, predicate, arg_count, ##__VA_ARGS__))

/*
*	Retrieves a pointer to the first value in the list that matches the predicate.
*
*	List_SelectPointerVargs(list, predicate, arg_count, ap)
*
*	/param		list			Pointer to the list.
*	/param		predicate		A pointer to a function (PREDICATE) which checks each value to its conditions.
*	/param		arg_count		How many arguments have been passed.
*	/param		ap				The variadic arguments. *Will call va_end before returning.
*
*	/returns	uint8_t*	A pointer to the first value in the array that matches the predicate
*/
#define List_SelectPointerVargs(list, predicate, arg_count, ap) internal_list_get(list, internal_list_select_vargs_memory(list, predicate, arg_count, ap))

#pragma endregion Macros

#pragma region Unsafe Macros


/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, its index will be added to a list and returned.
*
*	List_SelectMany(list, predicate)
*	List_SelectMany(list, predicate, index)
*	List_SelectMany(list, predicate, index, length)
*
*	/param		list		LIST_PTR	Pointer to the list.
*	/param		predicate	PREDICATE	A pointer to a function (PREDICATE) which checks each value to its conditions.
*	/param		index		uint		Zero based index at which to start enumeration.
*	/param		length		uint		The number of elements to enumerate.
*
*	/returns	LIST_PTR				A list of zero based indexes to all values in the array that matched the predicate, or NULL if there were no matches.
*/
#define List_SelectMany(...) LIST_SELECTMANY_INDEXES_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, will have a pointer to them added to a list that will be returned.
*
*	List_SelectManyPointers(list, predicate)
*	List_SelectManyPointers(list, predicate, index)
*	List_SelectManyPointers(list, predicate, index, length)
*
*	/param		list		LIST_PTR	Pointer to the list.
*	/param		predicate	PREDICATE	A pointer to a function (PREDICATE) which checks each value to its conditions.
*	/param		index		uint		Zero based index at which to start enumeration.
*	/param		length		uint		The number of elements to enumerate.
*
*	/returns	LIST_PTR				A list of pointers to each value in the list that matched the predicate's conditions, or NULL if there were no matches.
*/
#define List_SelectManyPointers(...) LIST_SELECTMANY_POINTERS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, will be added to a list and returned.
*
*	List_SelectManyValues(list, predicate)
*	List_SelectManyValues(list, predicate, index)
*	List_SelectManyValues(list, predicate, index, length)
*
*	/param		list		LIST_PTR		Pointer to the list.
*	/param		predicate	PREDICATE_ARGS	A pointer to a function (PREDICATE) which checks each value to its conditions.
*	/param		index		uint			Zero based index at which to start enumeration.
*	/param		length		uint			The number of elements to enumerate.
*
*	/returns	LIST_PTR				A list of values from the original list that matched the conditions defined by predicate, or NULL if there was no matches.
*/
#define List_SelectManyValues(...) LIST_SELECTMANY_VALUES_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, its index will be added to a list and returned.
*
*	List_SelectManyArgs(list, predicate, arg_count, ...)
*
*	/param		list		LIST_PTR		Pointer to the list.
*	/param		predicate	PREDICATE_ARGS	A pointer to a function (PREDICATE) which checks each value to its conditions.
*	/param		arg_count	uint			How many arguments have been passed.
*	/param		...							The variadic arguments.
*
*	/returns	LIST_PTR				A list of zero based indexes to all values in the array that matched the predicate, or NULL if there were no matches.
*/
#define List_SelectManyArgs(list, predicate, arg_count, ...) internal_list_selectMany_indexes_args(list, predicate, arg_count, ##__VA_ARGS__)

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, will have a pointer to them added to a list that will be returned.
*
*	List_SelectManyPointersArgs(list, predicate, arg_count, ...)
*
*	/param		list		LIST_PTR		Pointer to the list.
*	/param		predicate	PREDICATE_ARGS	A pointer to a function (PREDICATE) which checks each value to its conditions.
*	/param		arg_count	uint			How many arguments have been passed.
*	/param		...							The variadic arguments.
*
*	/returns	LIST_PTR				A list containing pointers to each value in the list that matched the predicate's conditions, or NULL if there were no matches.
*/
#define List_SelectManyPointersArgs(list, predicate, arg_count, ...) internal_list_selectMany_pointers_args(list, predicate, arg_count, ##__VA_ARGS__)

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, will be added to a list and returned.
*
*	List_SelectManyValuesArgs(list, predicate, arg_count, ...)
*
*	/param		list		LIST_PTR		Pointer to the list.
*	/param		predicate	PREDICATE_ARGS	A pointer to a function (PREDICATE) which checks each value to its conditions.
*	/param		arg_count	uint			How many arguments have been passed.
*	/param		...							The variadic arguments.
*
*	/returns	LIST_PTR				A list of values from the original list that matched the conditions defined by predicate, or NULL if there was no matches.
*/
#define List_SelectManyValuesArgs(list, predicate, arg_count, ...) internal_list_selectMany_values_args(list, predicate, arg_count, ##__VA_ARGS__)

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, its index will be added to a list and returned.
*
*	List_SelectManyVargs(list, predicate)
*
*	/param		list		LIST_PTR		Pointer to the list.
*	/param		predicate	PREDICATE_ARGS	A pointer to a function (PREDICATE) which checks each value to its conditions.
*	/param		arg_count	uint			How many arguments have been passed.
*	/param		ap			va_list			The variadic arguments.
*
*	/returns				LIST_PTR	A list of zero based indexes to all values in the array that matched the predicate, or NULL if there were no matches.
*/
#define List_SelectManyVargs(list, predicate, arg_count, ap) internal_list_selectMany_indexes_args(list, predicate, arg_count, ap)

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, will have a pointer to them added to a list that will be returned.
*
*	List_SelectManyPointersVargs(list, predicate)
*
*	/param		list		LIST_PTR		Pointer to the list.
*	/param		predicate	PREDICATE_ARGS	A pointer to a function (PREDICATE) which checks each value to its conditions.
*	/param		arg_count	uint			How many arguments have been passed.
*	/param		ap			va_list			The variadic arguments.
*
*	/returns				LIST_PTR	A list containing pointers to each value in the list that matched the predicate's conditions, or NULL if there were no matches.
*/
#define List_SelectManyPointersVargs(list, predicate, arg_count, ap) internal_list_selectMany_pointers_args(list, predicate, arg_count, ap)

/*
*	Enumerates through each value in the list, and checks it against the predicate's condition.
*	Every value that satisfied the condition, will be added to a list and returned.
*
*	List_SelectManyValuesVargs(list, predicate)
*
*	/param		list		LIST_PTR		Pointer to the list.
*	/param		predicate	PREDICATE_ARGS	A pointer to a function (PREDICATE) which checks each value to its conditions.
*	/param		arg_count	uint			How many arguments have been passed.
*	/param		ap			va_list			The variadic arguments.
*
*	/returns				LIST_PTR	A list of values from the original list that matched the conditions defined by predicate, or NULL if there was no matches.
*/
#define List_SelectManyValuesVargs(list, predicate, arg_count, ap) internal_list_selectMany_values_args(list, predicate, arg_count, ap)


/*
*	Gets a range of elements from a list, and returns them inside a new list.
*
*	List_GetRange(list, index, length)
*
*	/param	list			Pointer to the new list
*	/param	index			Zero based index at which to start enumeration.
*	/param	length			The number of elements to get.
*
*	/returns LIST_PTR		Pointer to the new list containing the range.
*/
#define List_GetRange(list, index, length) internal_list_getRange_memory(list, index * INTERNAL_LIST_GET_ELEM_SIZE(list), length * INTERNAL_LIST_GET_ELEM_SIZE(list))


/*
*	Inserts an array of values into a list, at the specified index.
*	If range has elements that are a different size than the list, it will discard or pad bytes while inserting.
*
*	List_InsertRange(list, range, index)
*	List_InsertRange(list, range, index, length)
*	List_InsertRange(list, range, index, length, elem_size)
*
*	/param	list			Pointer to the list
*	/param	range			The range of values to insert.
*	/param	index			The index where the new values should be inserted at.
*	/param	length			(Optional)The length of range.														*Required if range is not a static array.
*	/param	elem_size		(Optional)The size of the elements in the range.									*Required when passing a struct.
*
*	/returns	bool		True if the range was inserted, or false if the capacity was maxed out.
*/
#define List_InsertRange(...) LIST_INSERTRANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Inserts a list, into another list.
*
*	List_InsertList(list, insert_list, index)
*
*	/param	list			Pointer to the list
*	/param	insert_list		Pointer to the list to insert.
*	/param	index			The index where the new values should be inserted at.
*
*	/returns	bool		True if the range was inserted, or false if the capacity was maxed out.
*/
#define List_InsertList(list, insert_list, index) internal_list_insertRange_memory(list, List_Array(insert_list), index * INTERNAL_LIST_GET_ELEM_SIZE(list), List_Size(insert_list), INTERNAL_LIST_GET_ELEM_SIZE(insert_list))



/*
*	Inserts a value into the list, at the specified index.
*
*	List_Insert(list, value, index)
*	List_Insert(list, value, index, elem_size)
*
*	/param	list			Pointer to the list
*	/param	value			The value to insert.
*	/param	index			The index where the new values should be inserted at.
*	/param	elem_size		(Optional)The size of the elements in the range.									*Required when passing a struct or pointer.
*
*	/returns	bool		True if the element was inserted, or false if the capacity was maxed out.
*/
#define List_Insert(...)	LIST_INSERT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Adds an array of values on to the end of the list.
*	If range has elements that are a different size than the list, it will discard or pad bytes while inserting.
*
*	List_AddRange(list, range)								
*	List_AddRange(list, range, length)				
*	List_AddRange(list, range, length, elem_size)
*
*	/param	list			Pointer to the list
*	/param	range			The range of values to insert.
*	/param	index			The index where the new values should be inserted at.
*	/param	length			(Optional)Count of how many elements are in range.									*length must be passed when range is not static
*	/param	elem_size		(Optional)The size of the elements in "range."										*Required when passing a struct. 
*
*	/returns	bool		True if the range was added, or false if the capacity was maxed out.
*/
#define List_AddRange(...) LIST_ADDRANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Adds a list of values on to the end of a list.
*
*	List_AddList(list, add_list)
*
*	/param	list			Pointer to the list
*	/param	add_list		Pointer to the list to add.
*
*	/returns	bool		True if the list was added, or false if the capacity was maxed out.
*/
#define List_AddList(list, add_list) internal_list_addRange_memory(list, List_Array(add_list), List_Size(add_list), INTERNAL_LIST_GET_ELEM_SIZE(add_list))

/*
*	Adds a value to the end of a list.
*
*	List_Add(list, value)	
*	List_Add(list, value, elem_size)
*
*	/param	list			Pointer to the list
*	/param	value			The value or pointer to the value to add.
*	/param	elem_size		(Optional)The size of the elements in the range.									*Required when passing a struct or pointer.
*
*	/returns	bool		True if the element was added, or false if the capacity was maxed out.
*/
#define List_Add(...)		LIST_ADD_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Adds a range of values to the start of a list.
*
*	List_PrependRange(list, range)
*	List_PrependRange(list, range, length)	
*	List_PrependRange(list, range, length, elem_size)
*
*	/param	list			Pointer to the list
*	/param	range			The range of values to insert.
*	/param	length	(Optional)Count of how many elements are in range.											*length must be passed when range is not static
*	/param	elem_size		(Optional)The size of the elements in the range.									*Required when passing a struct.
*
*	/returns	bool		True if the range was prepended, or false if the capacity was maxed out.
*/
#define List_PrependRange(...) LIST_PREPENDRANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Adds a list of values to the start of a list.
*
*	List_PrependList(list, add_list)
*
*	/param	list			Pointer to the list
*	/param	prepend_list	Pointer to the list to prepend.
*
*	/returns	bool		True if the element was prepended, or false if the capacity was maxed out.
*/
#define List_PrependList(list, prepend_list) internal_list_prependRange_memory(list, List_Array(prepend_list), List_Size(prepend_list), INTERNAL_LIST_GET_ELEM_SIZE(prepend_list))

/*
*	Prepends a value to the start of a list.
*
*	List_Prepend(list, value)
*	List_Prepend(list, value, elem_size)
*
*	/param	list			Pointer to the list
*	/param	value			The value or pointer to the value to prepend.
*	/param	elem_size		(Optional)The size of the elements in the range.									*Required when passing a struct or pointer.
*
*	/returns	bool		True if the range was prepended, or false if the capacity was maxed out.
*/
#define List_Prepend(...)	LIST_PREPEND_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)



/*
*	Removes a range of values from a list.
*
*	List_RemoveRange(list,  index, length)
*
*	/param	list			Pointer to the list
*	/param	index			The index of the element to start removing at.
*	/param	length			The amount of elements to remove.
*/
#define List_RemoveRange(list, index, length) INTERNAL_LIST_REMOVERANGE(list, index, length)


/*
*	Removes an element from a list, at a specified index.
*
*	List_RemoveAt(list,  index)
*
*	/param	list			Pointer to the list
*	/param	index			The index of the element to remove.
*/
#define List_RemoveAt(list,  index) INTERNAL_LIST_REMOVEAT(list, index)


/*
*	Removes the first element from the list that matches value.
*
*	List_Remove(list, value)
*
*	/param	list			Pointer to the list
*	/param	value			The value to remove.
*
*	/returns	bool		True or false depending on if an item was found and removed.
*/
#define List_Remove(list, value) INTERNAL_LIST_REMOVE(list, value)


/*
*	List_RemoveAll(list, value)
*
*	/param	list			Pointer to the list
*	/param	value			The value to remove.
*
*	/returns	uint32_t	Returns amount of values removed from the list.
*/
#define List_RemoveAll(list, value) INTERNAL_LIST_REMOVEALL(list, value)


/*
*	Removes the first element in the list, that satisfies the predicate's condition.
*
*	List_RemoveMatch(list, predicate)
*
*	/param	list			Pointer to the list
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*
*	/returns	bool		True or false depending on if an item was found and removed.
*/
#define List_RemoveMatch(list, predicate)	internal_list_removeMatch_memory(list, predicate)


/*
*	Removes all elements in the list that satisfies the predicate's condition.
*
*	List_RemoveAllMatching(list, predicate)
*
*	/param	list			Pointer to the list
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*
*	/returns	uint32_t	Returns amount of values removed from the list.
*/
#define List_RemoveAllMatching(list, predicate) internal_list_removeAllMatching_memory(list, predicate)

/*
*	Removes the first element in the list, that satisfies the predicate's condition.
*
*	List_RemoveMatch_Vargs(list, predicate, arg_count, ap)
*
*	/param	list				Pointer to the list_dma
*	/param	predicate			A pointer to a function which checks each value to its conditions.
*	/param	arg_count			How many arguments have been passed.
*	/param	ap					The variadic arguments. *Will call va_end before returning.
*
*	/returns	bool			True or false depending on if an item was found and removed.
*/
#define List_RemoveMatch_Vargs(list, predicate, arg_count, ap) internal_list_removeMatch_vargs_memory(list, predicate, arg_count, ap)

/*
*	Removes all elements in the list that satisfies the predicate's condition.
*
*	List_RemoveAllMatching_Vargs(list, predicate, arg_count, ap)
*
*	/param	list				Pointer to the list_dma
*	/param	predicate			A pointer to a function which checks each value to its conditions.
*	/param	arg_count			How many arguments have been passed.
*	/param	ap					The variadic arguments. *Will call va_end before returning.
*
*	/returns	uint32_t		Returns amount of values removed from the list.
*/
#define List_RemoveAllMatching_Vargs(list, predicate, arg_count, ap) internal_list_removeAllMatching_vargs_memory(list, predicate, arg_count, ap)

/*
*	Removes the first element in the list, that satisfies the predicate's condition.
*
*	List_RemoveMatch_Args(list, predicate, *out_removed, arg_count, ...)
*
*	/param	list				Pointer to the list_dma
*	/param	predicate			A pointer to a function which checks each value to its conditions.
*	/param	arg_count			How many arguments have been passed.
*	/param	...					The variadic arguments.
*
*	/returns	bool			True or false depending on if an item was found and removed.
*/
#define List_RemoveMatch_Args(list, predicate, arg_count, ...) internal_list_removeMatch_args_memory(list, predicate, arg_count, ##__VA_ARGS__)


/*
*	Removes all elements in the list that satisfies the predicate's condition.
*
*	List_RemoveAllMatching_Args(list, predicate, *out_remove_count, arg_count, ...)
*
*	/param	list				Pointer to the list_dma
*	/param	predicate			A pointer to a function which checks each value to its conditions.
*	/param	arg_count			How many arguments have been passed.
*	/param	...					The variadic arguments.
*
*	/returns	uint32_t		Returns amount of values removed from the list.
*/
#define List_RemoveAllMatching_Args(list, predicate, arg_count, ...) internal_list_removeAllMatching_args_memory(list, predicate, arg_count, ##__VA_ARGS__)

/*
*	Reverse the internal array in memory.
*
*	List_Reverse(list)
*
*	/param	list			Pointer to the list
*/
#define List_Reverse(list) internal_list_reverse_memory(list)


/*
*	Casts all values in a list, and returns a new list containing all the cast values.
*
*	List_ConvertTo(list, oldType, newType)
*
*	/param	list			Pointer to the list.							*List can contain standard value types only.
*	/param	oldType			The type of values the original list stores.
*	/param	newType			The new type to convert to.
*
*	/returns LIST_PTR		A new list containing the converted values.
*/
#define List_ConvertTo(list, oldType, newType) internal_list_convertTo(list, oldType, newType)



#pragma endregion Unsafe Macros


#pragma region DMA Functions

/*
*	Initializes a list, and copies an already existing array to the list via DMA.
*
*	Array_DMA_CopyTo_List(array)
*	Array_DMA_CopyTo_List(array, length)
*	Array_DMA_CopyTo_List(array, length, populated_length)
*	Array_DMA_CopyTo_List(array, length, populated_length, elem_size)
*
*	/param	array				The initial size of the list. (How many values can be stored)
*	/param	length				(Optional)Either the length of the array, or the new capacity(only if "populated_length is used).	*Required for pointer arrays.
*	/param	populated_length	(Optional)The count of elements currently in the array.
*	/param	elem_size			(Optional)The size of a single element from the array.												*Required if passing a pointer whos type does not represent the internal values. (ARRAY_PTR, void*)
*
*	/Returns list_t		The newly allocated list.
*/
#define Array_DMA_CopyTo_List(...) ARRAY_DMA_COPYTO_LIST_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Initializes a list, and copies an already existing array to the list via DMA.
*
*	Array_DMA_Copy_Init_List(array, out_list)
*	Array_DMA_Copy_Init_List(array, length, out_list)
*	Array_DMA_Copy_Init_List(array, length, populated_length, out_list)
*	Array_DMA_Copy_Init_List(array, length, populated_length, elem_size, out_list)
*
*	/param	array				The initial size of the list. (How many values can be stored)
*	/param	length				(Optional)The length the array was created with.	*Required for pointer arrays.
*	/param	populated_length	(Optional)The count of elements currently in the array.
*	/param	elem_size			(Optional)The size of a single element from the array.												*Required if passing a pointer whos type does not represent the internal values. (ARRAY_PTR, void*)
*	/param	*out_list			(Out)The initialized list.
*
*/
#define Array_DMA_Copy_Init_List(...) ARRAY_DMA_COPY_INIT_LIST_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)



/*
*	Allocates a new list, and copies a range of values from the old list to the new via DMA.
*
*	List_DMA_GetRange(list, index, length)
*
*	/param	list			Pointer to the new list
*	/param	index			Zero based index at which to start enumeration.
*	/param	length			The number of elements in the range.
*
*	/returns	LIST_PTR	Pointer to the new list, or null if the DMA transfer was unable to register.
*/
#define List_DMA_GetRange(list, index, length) internal_list_dma_getRange_memory(list, index * INTERNAL_LIST_GET_ELEM_SIZE(list), length * INTERNAL_LIST_GET_ELEM_SIZE(list))


/*
*	Inserts an array of values into a list, at the specified index, using DMA.
*
*	List_DMA_InsertRange(list, range, index)				*range_length must be passed when range is not static
*	List_DMA_InsertRange(list, range, index, length)
*
*	/param	list			Pointer to the list
*	/param	range			The range of values to insert.
*	/param	index			The index at which the range will be inserted.
*	/param	length			(Optional)The amount of values to insert (eg. 4 integers = 4, not sizeof(int) * 4) *Required if range is not a static array.
*
*	/returns	bool		True if the DMA transfer was registered. False if there was no capacity in the list, or if the maximum transfer count has been reached.
*/
#define List_DMA_InsertRange(...) LIST_DMA_INSERTRANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Inserts a list of values into a another list, at the specified index, using DMA.
*
*	List_DMA_InsertList_DMA(list, insert_list, index)
*
*	/param	list				Pointer to the list
*	/param	insert_list			Pointer to the list to insert.
*	/param	index				The index at which the range will be inserted.
*
*	/returns	bool		True if the range was inserted, or false if the capacity was maxed out.
*/
#define List_DMA_InsertList(list, insert_list, index) internal_list_dma_insertRange_memory(list, List_Array(insert_list), index * INTERNAL_LIST_GET_ELEM_SIZE(list), List_Size(insert_list))

/*
*	Inserts a value into the list, at the specified index, using DMA.
*
*	List_DMA_Insert(list, value, index)
*
*	/param	list			Pointer to the list
*	/param	value			The value to insert.
*	/param	index			The index at which the range will be inserted.
*
*	/returns	bool		True if the range was inserted, or false if the capacity was maxed out.
*/
#define List_DMA_Insert(list, value, index) INTERNAL_LIST_DMA_INSERT(list, value, index * INTERNAL_LIST_GET_ELEM_SIZE(list_dma))

/*
*	Adds an array of values on to the end of the list, using DMA.
*
*	List_DMA_AddRange(list, range)
*	List_DMA_AddRange(list, range, length)				*range_length must be passed when range is not static
*
*	/param	list				Pointer to the list
*	/param	range				The range of values to insert.
*	/param	range_length		Count of how many elements are in range.
*
*	/returns	bool		True if the range was inserted, or false if the capacity was maxed out.
*/
#define List_DMA_AddRange(...) LIST_DMA_ADDRANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Adds a list of values on to the end of a list, using DMA.
*	
*	List_DMA_AddList_DMA(list, add_list)
*
*	/param	list			Pointer to the list
*	/param	add_list		Pointer to the list to add.
*
*	/returns	bool		True if the range was inserted, or false if the capacity was maxed out.
*/
#define List_DMA_AddList(list, add_list) internal_list_dma_addRange_memory(list, List_Array(add_list), List_Size(add_list))

/*
*	Adds a value to the end of a list, using DMA.
*
*	List_DMA_Add(list, value)
*
*	/param	list			Pointer to the list
*	/param	value			The value or pointer to the value to add.
*
*	/returns	bool		True if the range was added, or false if the capacity was maxed out.
*/
#define List_DMA_Add(list, value) INTERNAL_LIST_DMA_ADD(list, value)

/*
*	Adds a range of values to the start of a list, via DMA.
*
*	List_DMA_PrependRange(list, range)
*	List_DMA_PrependRange(list, range, length)				*range_length must be passed when range is not static
*
*	/param	list			Pointer to the list
*	/param	range			The range of values to insert.
*	/param	length			(Optional)Count of how many elements are in range.
*
*	/returns	bool		True if the range was prepended, or false if the capacity was maxed out.
*/
#define List_DMA_PrependRange(...) LIST_DMA_PREPENDRANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	List_DMA_PrependList_DMA(list, add_list_dma)
*
*	/param	list			Pointer to the list
*	/param	prepend_list	Pointer to the list_dma to prepend.
*
*	/returns	bool		True if the range was inserted, or false if the capacity was maxed out.
*/
#define List_DMA_PrependList(list, prepend_list) internal_list_dma_prependRange_memory(list, List_Array(prepend_list), List_Size(prepend_list))

/*
*	Adds a list of values to the start of a list, using DMA.
*
*	List_DMA_Prepend(list, value)
*
*	/param	list			Pointer to the list
*	/param	value			The value or pointer to the value to prepend.
*
*	/returns	bool		True if the range was prepended, or false if the capacity was maxed out.
*/
#define List_DMA_Prepend(list, value) INTERNAL_LIST_DMA_PREPEND(list, value)



/*
*	Removes a range of values from a list, using DMA.
*
*	List_DMA_RemoveRange(list,  index, length)
*
*	/param	list			Pointer to the list_dma
*	/param	index			The index of the element to start removing at.
*	/param	length			The amount of elements to remove.
*
*	/returns	uint8_t*	A pointer to the start of the new list_dma.
*/
#define List_DMA_RemoveRange(list, index, length) internal_list_dma_removeRange_memory(list, index * INTERNAL_LIST_GET_ELEM_SIZE(list), length * INTERNAL_LIST_GET_ELEM_SIZE(list))


/*
*	Removes an element from a list, at a specified index, using DMA.
*
*	List_DMA_RemoveAt(list,  index)
*
*	/param	list			Pointer to the list_dma
*	/param	index			The index of the element to remove.
*/
#define List_DMA_RemoveAt(list,  index) internal_list_dma_removeAt_memory(list, index * INTERNAL_LIST_GET_ELEM_SIZE(list))

/*
*	Removes the first element from the list that matches value, using DMA.
*
*	List_DMA_Remove(list, value)
*	List_DMA_Remove(list, value, *out_removed)
*
*	/param	list			Pointer to the list_dma
*	/param	value			The value to remove.
*	/param	*out_removed	(out, optional)True or false depending off if a value was found and removed.
*
*	/returns	bool		True if the DMA transfer was registered, or if there were no values to remove, out_removed will also be false. When False and out_removed true, the maximum transfer count has been reached.
*/
#define List_DMA_Remove(...) LIST_DMA_REMOVE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Removes all elements from a list, that match the value, using DMA.
*
*	List_DMA_RemoveAll(list, value)
*	List_DMA_RemoveAll(list, value, *out_remove_count)
*
*	/param	list				Pointer to the list_dma
*	/param	value				The value to remove.
*	/param	*out_remove_count	(out, optional)A count of how many elements were removed.
*
*	/returns	bool			True if the DMA transfer was registered, or if there were no values to remove, out_remove_count will also be 0. When False and out_remove_count > 0, the maximum transfer count has been reached.
*/
#define List_DMA_RemoveAll(...) LIST_DMA_REMOVEALL_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Removes the first element in the list, that satisfies the predicate's condition, using DMA.
*
*	List_DMA_RemoveMatch(list, predicate)
*	List_DMA_RemoveMatch(list, predicate, *out_removed)
*
*	/param	list			Pointer to the list_dma
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*	/param	*out_removed	(out, optional)True or false depending off if a value was found and removed.
*
*	/returns	bool		True if the DMA transfer was registered, or if there were no values to remove, out_removed will also be false. When False and out_removed true, the maximum transfer count has been reached.
*/
#define List_DMA_RemoveMatch(...) LIST_DMA_REMOVEMATCH_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Removes all elements in the list that satisfies the predicate's condition, using DMA.
*
*	List_DMA_RemoveAllMatching(list, predicate)
*	List_DMA_RemoveAllMatching(list, predicate, *out_remove_count)
*
*	/param	list				Pointer to the list_dma
*	/param	predicate			A pointer to a function which checks each value to its conditions.
*	/param	*out_remove_count	(out, optional)A count of how many elements were removed.
*
*	/returns	bool			True if the DMA transfer was registered, or if there were no values to remove, out_remove_count will also be 0. When False and out_remove_count > 0, the maximum transfer count has been reached.
*/
#define List_DMA_RemoveAllMatching(...) LIST_DMA_REMOVEALLMATCHING_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Removes the first element in the list, that satisfies the predicate's condition, using DMA.
*
*	List_DMA_RemoveMatch_Vargs(list, predicate, arg_count, ap)
*	List_DMA_RemoveMatch_Vargs(list, predicate, arg_count, ap, *out_removed)
*
*	/param	list				Pointer to the list_dma
*	/param	predicate			A pointer to a function which checks each value to its conditions.
*	/param	arg_count			How many arguments have been passed.
*	/param	ap					The variadic arguments. *Will call va_end before returning.
*	/param	*out_remove_count	(out, optional)A count of how many elements were removed.
*
*	/returns	bool		True if the DMA transfer was registered, or if there were no values to remove, out_removed will also be false. When False and out_removed true, the maximum transfer count has been reached.
*/
#define List_DMA_RemoveMatch_Vargs(...) LIST_DMA_REMOVEMATCH_VARGS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Removes all elements in the list that satisfies the predicate's condition, using DMA.
*
*	List_DMA_RemoveAllMatching_Vargs(list, predicate, arg_count, ap)
*	List_DMA_RemoveAllMatching_Vargs(list, predicate, arg_count, ap, *out_remove_count)
*
*	/param	list				Pointer to the list_dma
*	/param	predicate			A pointer to a function which checks each value to its conditions.
*	/param	arg_count			How many arguments have been passed.
*	/param	ap					The variadic arguments. *Will call va_end before returning.
*	/param	*out_remove_count	(out, optional)A count of how many elements were removed.
*
*	/returns	bool			True if the DMA transfer was registered, or if there were no values to remove, out_remove_count will also be 0. When False and out_remove_count > 0, the maximum transfer count has been reached.
*/
#define List_DMA_RemoveAllMatching_Vargs(...) LIST_DMA_REMOVEALLMATCHING_VARGS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Removes the first element in the list, that satisfies the predicate's condition, using DMA.
*
*	List_DMA_RemoveMatch_Args(list, predicate, *out_removed, arg_count, ...)
*
*	/param	list				Pointer to the list_dma
*	/param	predicate			A pointer to a function which checks each value to its conditions.
*	/param	*out_removed		True or false depending off if a value was found and removed.
*	/param	arg_count			How many arguments have been passed.
*	/param	...					The variadic arguments.
*
*	/returns	bool		True if the DMA transfer was registered, or if there were no values to remove, out_removed will also be false. When False and out_removed true, the maximum transfer count has been reached.
*/
#define List_DMA_RemoveMatch_Args(list, predicate, out_removed, arg_count, ...) internal_list_dma_removeMatch_args_memory(list, predicate, out_removed, arg_count, ##__VA_ARGS__)


/*
*	Removes all elements in the list that satisfies the predicate's condition, using DMA.
*
*	List_DMA_RemoveAllMatching_Args(list, predicate, *out_remove_count, arg_count, ...)
*
*	/param	list				Pointer to the list_dma
*	/param	predicate			A pointer to a function which checks each value to its conditions.
*	/param	*out_remove_count	(out, optional)A count of how many elements were removed.
*	/param	arg_count			How many arguments have been passed.
*	/param	...					The variadic arguments.
*
*	/returns	bool			True if the DMA transfer was registered, or if there were no values to remove, out_remove_count will also be 0. When False and out_remove_count > 0, the maximum transfer count has been reached.
*/
#define List_DMA_RemoveAllMatching_Args(list, predicate, out_remove_count, arg_count, ...) internal_list_dma_removeAllMatching_args_memory(list, predicate, out_removed, out_remove_count, ##__VA_ARGS__)

#pragma endregion DMA Functions


#endif /* GENETICC_LIST_H_ */