/*
	-Generic Array-
	
	A collection of C# like macros/functions for working with arrays.
	
	*If you are constantly using the "unsafe" macros/functions, you will get a major performance benefit by switching to the "geneticc_list.h" implementations.
*/

#ifndef GENETICC_ARRAY_H_
#define GENETICC_ARRAY_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

#include "../geneticc.h"
#include "geneticc_array_defs.h"
#include "internal/geneticc_array_internal_macros.h"


#pragma region Macros


/*
*	Searches an array for a value, and confirms its existence.
*
*	Array_Contains(array, value)
*	Array_Contains(array, value, length)
*	Array_Contains(array, value, length, start)
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to search for.
*	/param	length		(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	start		(Optional)The index to start searching from.
*
*	/returns	bool	If the array contains the value.
*/
#define Array_Contains(...) ARRAY_CONTAINS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Enumerates through an array checking the elements to the predicate, and returns true or false if an element satisfied the condition.
*
*	Array_Exists(array, predicate)
*	Array_Exists(array, predicate, length)
*	Array_Exists(array, predicate, length, start)
*
*	/param	array		Pointer to the start of the array.
*	/param	predicate	A function which checks each value to its conditions.
*	/param	length		The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			True if an element in the array satisfied the predicate's condition, false if the predicate was unsatisfied.
*/
#define Array_Exists(...) ARRAY_EXISTS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Search an array for a specific value, and returns the first index.
*
*	Array_IndexOf(array, value)
*	Array_IndexOf(array, value, length)
*	Array_IndexOf(array, value, length, start)
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to search for.
*	/param	length		(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	start		(Optional)The index to start searching from.
*
*	/returns	int		The index to the first item in the array matching value, or -1 if it does not exist.
*/
#define Array_IndexOf(...) ARRAY_INDEXOF_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an array from back to front for a value, and returns the index of the first match (latest index).
*
*	Array_LastIndexOf(array, value)
*	Array_LastIndexOf(array, value, length)
*	Array_LastIndexOf(array, value, length, start)
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to search for.
*	/param	length		(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	start		(Optional)The earliest index in the search range.
*
*	/returns	uint		The index to the last item in the array matching value, or -1 if it does not exist.
*/
#define Array_LastIndexOf(...) ARRAY_LASTINDEXOF_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Counts how many times "value" is seen in the array.
*
*	Array_ValueCount(array, value)
*	Array_ValueCount(array, value, length)
*	Array_ValueCount(array, value, length, start)
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to search for.
*	/param	length		(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	start		(Optional)The index to start searching from.
*
*	/returns	uint	The amount of times "value" is in the array.
*/
#define Array_ValueCount(...) ARRAY_VALUECOUNT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an array for the largest value, and returns its index.
*
*	Array_Max(array)
*	Array_Max(array, length)
*	Array_Max(array, length, start)
*
*	/param	array		Pointer to the start of the array.
*	/param	length		(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	start		(Optional)The index to start at.
*
*	/returns	uint	The index of the largest value in the array.
*/
#define Array_Max(...) ARRAY_MAX_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an array for the largest value, and returns the value.
*
*	Array_Max_Val(array)
*	Array_Max_Val(array, length)
*	Array_Max_Val(array, length, start)
*
*	/param	array		Pointer to the start of the array.
*	/param	length		(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	start		(Optional)The index to start at.
*
*	/returns	generic		The largest value in the array.
*/
#define Array_Max_Val(...) ARRAY_MAX_VALUE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an array for the smallest value, and returns its index.
*
*	Array_Min(array)
*	Array_Min(array, length)
*	Array_Min(array, length, start)
*
*	/param	array		Pointer to the start of the array.
*	/param	length		(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	start		(Optional)The index to start at.
*
*	/returns	uint	The index of the smallest value in the array.
*/
#define Array_Min(...) ARRAY_MIN_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an array for the smallest value, and returns the value
*
*	Array_Min_Val(array)
*	Array_Min_Val(array, length)
*	Array_Min_Val(array, length, start)
*
*	/param	array			Pointer to the start of the array.
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	start			(Optional)The index to start at.
*
*	/returns	generic		The largest value in the array.
*/
#define Array_Min_Val(...) ARRAY_MIN_VALUE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Gets the sum of all values in the array.
*
*	Array_Sum(array)
*	Array_Sum(array, length)
*	Array_Sum(array, length, start)
*
*	/param	array			Pointer to the start of the array.
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	start			(Optional)The index to start at.
*
*	/returns	generic		The index of the smallest value in the array.
*/
#define Array_Sum(...) ARRAY_SUM_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Gets the average of all values in the array.
*
*	Array_Average(array)
*	Array_Average(array, length)
*	Array_Average(array, length, start)
*
*	/param	array			Pointer to the start of the array.
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	start			(Optional)The index to start at.
*
*	/returns	generic		The index of the smallest value in the array.
*/
#define Array_Average(...) ARRAY_AVERAGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Retrieves the index to the first value in the array that matches the predicate.
*
*	Array_FindIndex(array, predicate)
*	Array_FindIndex(array, predicate, length)
*	Array_FindIndex(array, predicate, length, start)
*
*	/param	array			Pointer to the start of the array.
*	/param	predicate		A pointer to a function (ARRAY_PREDICATE) which checks each value to its conditions.
*	/param	length			(Optional)The count of elements in the array. (eg. int[10] = 10)
*	/param	start			(Optional)The index to start at.

*
*	/returns			Zero based index of the first value in the array that matches the predicate, or -1 if the value does not exist.
*/
#define Array_FindIndex(...) ARRAY_FINDINDEX_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Retrieves a pointer to the first value in the array that matches the predicate.
*
*	Array_Find(array, predicate)
*	Array_Find(array, predicate, length)
*	Array_Find(array, predicate, length, start)
*
*	/param	array			Pointer to the start of the array.
*	/param	length			(Optional)The count of elements in the array. (eg. int[10] = 10)
*	/param	elem_size		(Optional)The size of value's type (in bytes).
*	/param	predicate		A pointer to a function (ARRAY_PREDICATE) which checks each value to its conditions.
*
*	/returns	uint8_t*	A pointer to the first value in the array that matches the predicate
*/
#define Array_Find(...) ARRAY_FIND_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Retrieves the index to the first value in the array that matches the predicate.
*
*	Array_FindIndexArgs(array, predicate, length, arg_count, ...)
*
*	/param		array			Pointer to the array.
*	/param		length			The count of elements in the array. (eg. int[10] = 10)
*	/param		elem_size		The size of value's type (in bytes).
*	/param		predicate		A pointer to a function (ARRAY_PREDICATE) which checks each value to its conditions.
*	/param		arg_count		How many arguments have been passed.
*	/param		...				The variadic arguments.
*
*	/returns					Zero based index of the first value in the array that matches the predicate, or -1 if the value does not exist.
*/
#define Array_FindIndexArgs(array, predicate, length, arg_count, ...) internal_array_select_args_memory(array, length, sizeof(array[0]), predicate, arg_count, ##__VA_ARGS__)

/*
*	Retrieves the index to the first value in the array that matches the predicate.
*
*	Array_FindIndexVargs(array, predicate, arg_count, ap)
*	Array_FindIndexVargs(array, predicate, arg_count, ap, length)
*	Array_FindIndexVargs(array, predicate, arg_count, ap, length, start)
*
*	/param		array			Pointer to the array.
*	/param		predicate		A pointer to a function (ARRAY_PREDICATE) which checks each value to its conditions.
*	/param		arg_count		How many arguments have been passed.
*	/param		ap				The variadic arguments. *Will call va_end before returning.
*	/param		length			(Optional)The count of elements in the array. (eg. int[10] = 10)
*	/param		start			(Optional)The index to start at.
*
*	/returns					Zero based index of the first value in the array that matches the predicate, or -1 if the value does not exist.
*/
#define Array_FindIndexVargs(...) ARRAY_FINDINDEX_VARGS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)



/*
*	Retrieves a pointer to the first value in the array that matches the predicate.
*
*	Array_FindArgs(array, predicate, length, arg_count, ...)
*
*	/param		array			Pointer to the array.
*	/param		predicate		A pointer to a function (ARRAY_PREDICATE) which checks each value to its conditions.
*	/param		length			The count of elements in the array. (eg. int[10] = 10)
*	/param		arg_count		How many arguments have been passed.
*	/param		...				The variadic arguments.
*
*	/returns	uint8_t*		A pointer to the first value in the array that matches the predicate, or null if there were no matches.
*/
#define Array_FindArgs(array, predicate, length, arg_count, ...) INTERNAL_ARRAY_GET_POINTER(array, internal_array_select_args_memory(array, length, sizeof(array[0]), predicate, arg_count, ##__VA_ARGS__), sizeof(array[0]))

/*
*	Retrieves a pointer to the first value in the array that matches the predicate.
*
*	Array_FindVargs(array, predicate, arg_count, ap)
*	Array_FindVargs(array, predicate, arg_count, ap, length)
*	Array_FindVargs(array, predicate, arg_count, ap, length, start)
*
*	/param		array			Pointer to the array.
*	/param		predicate		A pointer to a function (ARRAY_PREDICATE) which checks each value to its conditions.
*	/param		arg_count		How many arguments have been passed.
*	/param		ap				The variadic arguments. *Will call va_end before returning.
*	/param		length			(Optional)The count of elements in the array. (eg. int[10] = 10)
*	/param		start			(Optional)The index to start at.
*
*	/returns	uint8_t*		A pointer to the first value in the array that matches the predicate, or null if there were no matches.
*/
#define Array_FindVargs(...) ARRAY_FIND_VARGS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


#pragma endregion Macros

#pragma region Unsafe Macros

/*
*	Retrieves an array of pointers to the elements that match the conditions defined by the specified predicate.
*
*	Array_FindAll(array, predicate, out_count)
*	Array_FindAll(array, predicate, length, out_count)
*	Array_FindAll(array, predicate, length, start, out_count)
*
*	/param	array			*			Pointer to the start of the array.
*	/param	length			uint		(Optional)The count of elements in the array. (eg. int[10] = 10)
*	/param	predicate		PREDICATE	A pointer to a function (ARRAY_PREDICATE) which checks each value to its conditions.
*	/param	out_length		int*		(Optional)The amount of matches in the array.
*
*	/returns	uint8t**	An array of pointers to each value in the array that matched the predicate's conditions
*/
#define Array_FindAll(...) ARRAY_FINDALL_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Inserts a range of values into an array.
*	Works by allocating enough memory to fit the original array, plus the new values.
*	It then copies the array and the values into the new array, and returns a pointer to where the new array was allocated.
*	*WARNING : length must be passed for pointer arrays!
*
*	Array_InsertRange(array, range, index)	
*	Array_InsertRange(array, range, index, count)	
*	Array_InsertRange(array, range, index, count, length)										
*	Array_InsertRange(array, range, index, count, length, free_old)					
*
*	/param	array			Pointer to the start of the array.
*	/param	range			Pointer to the start of the range of values.
*	/param	index			Index at which to insert the range at.
*	/param	count			(Optional)The amount of values to insert (eg. 4 integers = 4, not sizeof(int) * 4)
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	free_old		(Optional)If true, the array passed to "array" will be freed after the copy.
*	/param	start			(Optional)The index to start at. (must be larger than index)
*
*	/returns	uint8_t*	A pointer to the start of the new array.
*/
#define Array_InsertRange(...) ARRAY_INSERTRANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Inserts a value into an array.
*	Works by allocating enough memory to fit the original array, plus the new value.
*	It then copies the array and the value into the new array, and returns a pointer to where the new array was allocated.
*	*WARNING : length must be passed for pointer arrays!
*
*	Array_Insert(array, value, index)
*	Array_Insert(array, value, index, length)
*	Array_Insert(array, value, index, length, free_old)
*
*	/param	array			Pointer to the start of the array.
*	/param	value			The value to insert.
*	/param	index			The index where the new values should be inserted at.
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	free_old		(Optional)If true, the array passed to "array" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new array.
*/
#define Array_Insert(...) ARRAY_INSERT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Adds a range of values into the end of an array.
*	*Calculates the offset and then calls array_insertRange_memory.
*	*WARNING : length must be passed for pointer arrays!
*
*	Array_AddRange(array, value)
*	Array_AddRange(array, value, count)
*	Array_AddRange(array, value, count, length)
*	Array_AddRange(array, value, count, length, free_old)
*
*	/param	array			Pointer to the start of the array.
*	/param	value			The value to add.
*	/param	count			(Optional)The amount of values to insert (eg. 4 integers = 4, not sizeof(int) * 4)
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	free_old		(Optional)If true, the array passed to "array" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new array.
*/
#define Array_AddRange(...) ARRAY_ADDRANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Adds a value to the end of an array.
*	*Calculates the offset and then calls array_insertRange_memory with a count of 1.
*	*WARNING : length must be passed for pointer arrays!
*
*	Array_Add(array, value)
*	Array_Add(array, value, length)
*	Array_Add(array, value, length, free_old)
*
*	/param	array			Pointer to the start of the array.
*	/param	value			The value to add.
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	free_old		(Optional)If true, the array passed to "array" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new array.
*/
#define Array_Add(...) ARRAY_ADD_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Adds a range of values on to the start of an array.
*	*Calculates the offset and then calls array_insertRange_memory.
*	*WARNING : length must be passed for pointer arrays!
*
*	Array_PrependRange(array, range)
*	Array_PrependRange(array, range, count)
*	Array_PrependRange(array, range, count, length)
*	Array_PrependRange(array, range, count, length, free_old)
*
*	/param	array			Pointer to the start of the array.
*	/param	value			The value to prepend.
*	/param	index			The index where the new values should be prepended at.
*	/param	count			(Optional)The amount of values to insert (eg. 4 integers = 4, not sizeof(int) * 4)
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	free_old		(Optional)If true, the array passed to "array" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new array.
*/
#define Array_PrependRange(...) ARRAY_PREPENDRANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Adds a value to the start of an array.
*	*Calculates the offset and then calls array_insertRange_memory with a count of 1.
*	*WARNING : length must be passed for pointer arrays!
*
*	Array_Prepend(array, value)
*	Array_Prepend(array, value, length)
*	Array_Prepend(array, value, length, free_old)
*
*	/param	array			Pointer to the start of the array.
*	/param	value			The value to prepend.
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	free_old		(Optional)If true, the array passed to "array" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new array.
*/
#define Array_Prepend(...) ARRAY_PREPEND_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)



/*
*	Removes a range of values from an array.
*	Works by allocating memory to fit the original array, minus the count.
*	It then copies the data surrounding offset + count from the old array to the new, and returns a pointer to where the new array was allocated.
*	*WARNING : length must be passed for pointer arrays!
*
*	Array_RemoveRange(array,  index, count)
*	Array_RemoveRange(array,  index, count, length)
*	Array_RemoveRange(array,  index, count, length, free_old)
*
*	/param	array			Pointer to the start of the array.
*	/param	index			The index of the element to start removing at.
*	/param	count			The amount of elements to remove
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	free_old		(Optional)If true, the array passed to "array" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new array.
*/
#define Array_RemoveRange(...) ARRAY_REMOVERANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Removes a value from an array by its index.
*	*WARNING : length must be passed for pointer arrays!
*
*	Array_RemoveAt(array,  index)
*	Array_RemoveAt(array,  index, length)
*	Array_RemoveAt(array,  index, length, free_old)
*
*	/param	array			Pointer to the start of the array.
*	/param	index			The index of the element to remove.
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	free_old		(Optional)If true, the array passed to "array" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new array.
*/
#define Array_RemoveAt(...) ARRAY_REMOVEAT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Removes the first item from an array which equals "value."
*	*WARNING : length must be passed for pointer arrays!
*
*	Array_Remove(array, value)
*	Array_Remove(array, value, out_remove)
*	Array_Remove(array, value, length, out_remove)
*	Array_Remove(array, value, length, free_old, out_remove)
*
*	/param	array			Pointer to the start of the array.
*	/param	value			The value to remove.
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	free_old		(Optional)If true, the array passed to "array" will be freed after the copy.
*	/param	*out_removed	(Out, Optional)Returns true or false depending if a value was found and removed.
*
*	/returns	uint8_t*	A pointer to the start of the new array.
*/
#define Array_Remove(...) ARRAY_REMOVE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Removes all items from an array which equals "value."
*	*WARNING : length must be passed for pointer arrays!
*
*	Array_RemoveAll(array, value)
*	Array_RemoveAll(array, value, out_count)
*	Array_RemoveAll(array, value, length, out_count)
*	Array_RemoveAll(array, value, length, free_old, out_count)
*
*	/param	array			Pointer to the start of the array.
*	/param	value			The value to determine which elements to remove.
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	free_old		(Optional)If true, the array passed to "array" will be freed after the copy.
*	/param	*out_count		(Out, Optional)Returns amount of values removed from the array.
*
*	/returns	uint8_t*	A pointer to the start of the new array.
*/
#define Array_RemoveAll(...) ARRAY_REMOVEALL_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Reverse the array in memory.
*	*WARNING : length must be passed for pointer arrays!
*
*	Array_Reverse(array)
*	Array_Reverse(array, length)
*	Array_Reverse(array, length, free_old)
*
*	/param	array			Pointer to the start of the array.
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	free_old		(Optional)If true, the array passed to "array" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new array.
*/
#define Array_Reverse(...) ARRAY_REVERSE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


//#define Array_Sort(array, comparison)

/*
*	Converts an array of one data type, to an array of a different data type.
*
*	Array_ConvertTo(array, oldType, newType)
*	Array_ConvertTo(array, length, oldType, newType)
*	Array_ConvertTo(array, length, oldType, newType, free_old)
*
*	/param	array			Pointer to the start of the array.
*	/param	length			(Optional)The count of elements in the array. *Required for pointer arrays.
*	/param	oldType			The type of values the original array stores.
*	/param	newType			The new type to convert to.
*	/param	free_old		(Optional)If true, the array passed to "array" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new array.
*/
#define Array_ConvertTo(...) ARRAY_CONVERTTO_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

//#define Array_ConvertAll(...) 

#pragma endregion Unsafe Macros


#endif /* GENETICC_ARRAY_H_ */
