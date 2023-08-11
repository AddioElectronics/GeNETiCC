/*
	-Generic Enumerable-
	
	A collection of C# like macros/functions for working with enumerable s.
	
	*If you are constantly using the "unsafe" macros/functions, you will get a major performance benefit by switching to the "list_geneticc.h" implementations.
*/

#ifndef ENUMERABLE_GENETICC_H_
#define ENUMERABLE_GENETICC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

#include "Addio/GeNETiCC/geneticc.h"
#include "enumerable_geneticc_defs.h"
#include "internal/enumerable_geneticc_internal_macros.h"


#pragma region Macros


/*
*	Searches an enumerable for a value, and confirms its existence.
*
*	Enumerable_Contains(enumerable, value)
*	Enumerable_Contains(enumerable, value, length)
*	Enumerable_Contains(enumerable, value, length, start)
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	value		The value to search for.
*	/param	length		(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	start		(Optional)The index to start searching from.
*
*	/returns	bool	If the enumerable contains the value.
*/
#define Enumerable_Contains(...) ENUMERABLE_CONTAINS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Enumerates through an enumerable checking the elements to the predicate, and returns true or false if an element satisfied the condition.
*
*	Enumerable_Exists(enumerable, predicate)
*	Enumerable_Exists(enumerable, predicate, length)
*	Enumerable_Exists(enumerable, predicate, length, start)
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	predicate	A function which checks each value to its conditions.
*	/param	length		The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			True if an element in the enumerable satisfied the predicate's condition, false if the predicate was unsatisfied.
*/
#define Enumerable_Exists(...) ENUMERABLE_EXISTS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Search an enumerable for a specific value, and returns the first index.
*
*	Enumerable_IndexOf(enumerable, value)
*	Enumerable_IndexOf(enumerable, value, length)
*	Enumerable_IndexOf(enumerable, value, length, start)
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	value		The value to search for.
*	/param	length		(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	start		(Optional)The index to start searching from.
*
*	/returns	int		The index to the first item in the enumerable matching value, or -1 if it does not exist.
*/
#define Enumerable_IndexOf(...) ENUMERABLE_INDEXOF_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an enumerable from back to front for a value, and returns the index of the first match (latest index).
*
*	Enumerable_LastIndexOf(enumerable, value)
*	Enumerable_LastIndexOf(enumerable, value, length)
*	Enumerable_LastIndexOf(enumerable, value, length, start)
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	value		The value to search for.
*	/param	length		(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	start		(Optional)The earliest index in the search range.
*
*	/returns	uint		The index to the last item in the enumerable matching value, or -1 if it does not exist.
*/
#define Enumerable_LastIndexOf(...) ENUMERABLE_LASTINDEXOF_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Counts how many times "value" is seen in the enumerable.
*
*	Enumerable_ValueCount(enumerable, value)
*	Enumerable_ValueCount(enumerable, value, length)
*	Enumerable_ValueCount(enumerable, value, length, start)
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	value		The value to search for.
*	/param	length		(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	start		(Optional)The index to start searching from.
*
*	/returns	uint	The amount of times "value" is in the enumerable.
*/
#define Enumerable_ValueCount(...) ENUMERABLE_VALUECOUNT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an enumerable for the largest value, and returns its index.
*
*	Enumerable_Max(enumerable)
*	Enumerable_Max(enumerable, length)
*	Enumerable_Max(enumerable, length, start)
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	length		(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	start		(Optional)The index to start at.
*
*	/returns	uint	The index of the largest value in the enumerable.
*/
#define Enumerable_Max(...) ENUMERABLE_MAX_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an enumerable for the largest value, and returns the value.
*
*	Enumerable_Max_Val(enumerable)
*	Enumerable_Max_Val(enumerable, length)
*	Enumerable_Max_Val(enumerable, length, start)
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	length		(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	start		(Optional)The index to start at.
*
*	/returns	generic		The largest value in the enumerable.
*/
#define Enumerable_Max_Val(...) ENUMERABLE_MAX_VALUE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an enumerable for the smallest value, and returns its index.
*
*	Enumerable_Min(enumerable)
*	Enumerable_Min(enumerable, length)
*	Enumerable_Min(enumerable, length, start)
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	length		(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	start		(Optional)The index to start at.
*
*	/returns	uint	The index of the smallest value in the enumerable.
*/
#define Enumerable_Min(...) ENUMERABLE_MIN_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Searches an enumerable for the smallest value, and returns the value
*
*	Enumerable_Min_Val(enumerable)
*	Enumerable_Min_Val(enumerable, length)
*	Enumerable_Min_Val(enumerable, length, start)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	start			(Optional)The index to start at.
*
*	/returns	generic		The largest value in the enumerable.
*/
#define Enumerable_Min_Val(...) ENUMERABLE_MIN_VALUE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Gets the sum of all values in the enumerable.
*
*	Enumerable_Sum(enumerable)
*	Enumerable_Sum(enumerable, length)
*	Enumerable_Sum(enumerable, length, start)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	start			(Optional)The index to start at.
*
*	/returns	generic		The index of the smallest value in the enumerable.
*/
#define Enumerable_Sum(...) ENUMERABLE_SUM_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Gets the average of all values in the enumerable.
*
*	Enumerable_Average(enumerable)
*	Enumerable_Average(enumerable, length)
*	Enumerable_Average(enumerable, length, start)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	start			(Optional)The index to start at.
*
*	/returns	generic		The index of the smallest value in the enumerable.
*/
#define Enumerable_Average(...) ENUMERABLE_AVERAGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Retrieves the index to the first value in the enumerable that matches the predicate.
*
*	Enumerable_FindIndex(enumerable, predicate)
*	Enumerable_FindIndex(enumerable, predicate, length)
*	Enumerable_FindIndex(enumerable, predicate, length, start)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	predicate		A pointer to a function (ENUMERABLE_PREDICATE) which checks each value to its conditions.
*	/param	length			(Optional)The count of elements in the enumerable. (eg. int[10] = 10)
*	/param	start			(Optional)The index to start at.

*
*	/returns			Zero based index of the first value in the enumerable that matches the predicate, or -1 if the value does not exist.
*/
#define Enumerable_FindIndex(...) ENUMERABLE_FINDINDEX_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Retrieves a pointer to the first value in the enumerable that matches the predicate.
*
*	Enumerable_Find(enumerable, predicate)
*	Enumerable_Find(enumerable, predicate, length)
*	Enumerable_Find(enumerable, predicate, length, start)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	length			(Optional)The count of elements in the enumerable. (eg. int[10] = 10)
*	/param	elem_size		(Optional)The size of value's type (in bytes).
*	/param	predicate		A pointer to a function (ENUMERABLE_PREDICATE) which checks each value to its conditions.
*
*	/returns	uint8_t*	A pointer to the first value in the enumerable that matches the predicate
*/
#define Enumerable_Find(...) ENUMERABLE_FIND_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Retrieves the index to the first value in the enumerable that matches the predicate.
*
*	Enumerable_FindIndexArgs(enumerable, predicate, length, arg_count, ...)
*
*	/param		enumerable			Pointer to the enumerable.
*	/param		length			The count of elements in the enumerable. (eg. int[10] = 10)
*	/param		elem_size		The size of value's type (in bytes).
*	/param		predicate		A pointer to a function (ENUMERABLE_PREDICATE) which checks each value to its conditions.
*	/param		arg_count		How many arguments have been passed.
*	/param		...				The variadic arguments.
*
*	/returns					Zero based index of the first value in the enumerable that matches the predicate, or -1 if the value does not exist.
*/
#define Enumerable_FindIndexArgs(enumerable, predicate, length, arg_count, ...) internal_enumerable_select_args_memory(enumerable, length, sizeof(enumerable[0]), predicate, arg_count, ##__VA_ARGS__)

/*
*	Retrieves the index to the first value in the enumerable that matches the predicate.
*
*	Enumerable_FindIndexVargs(enumerable, predicate, arg_count, ap)
*	Enumerable_FindIndexVargs(enumerable, predicate, arg_count, ap, length)
*	Enumerable_FindIndexVargs(enumerable, predicate, arg_count, ap, length, start)
*
*	/param		enumerable			Pointer to the enumerable.
*	/param		predicate		A pointer to a function (ENUMERABLE_PREDICATE) which checks each value to its conditions.
*	/param		arg_count		How many arguments have been passed.
*	/param		ap				The variadic arguments. *Will call va_end before returning.
*	/param		length			(Optional)The count of elements in the enumerable. (eg. int[10] = 10)
*	/param		start			(Optional)The index to start at.
*
*	/returns					Zero based index of the first value in the enumerable that matches the predicate, or -1 if the value does not exist.
*/
#define Enumerable_FindIndexVargs(...) ENUMERABLE_FINDINDEX_VARGS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)



/*
*	Retrieves a pointer to the first value in the enumerable that matches the predicate.
*
*	Enumerable_FindArgs(enumerable, predicate, length, arg_count, ...)
*
*	/param		enumerable			Pointer to the enumerable.
*	/param		predicate		A pointer to a function (ENUMERABLE_PREDICATE) which checks each value to its conditions.
*	/param		length			The count of elements in the enumerable. (eg. int[10] = 10)
*	/param		arg_count		How many arguments have been passed.
*	/param		...				The variadic arguments.
*
*	/returns	uint8_t*		A pointer to the first value in the enumerable that matches the predicate, or null if there were no matches.
*/
#define Enumerable_FindArgs(enumerable, predicate, length, arg_count, ...) INTERNAL_ENUMERABLE_GET_POINTER(enumerable, internal_enumerable_select_args_memory(enumerable, length, sizeof(enumerable[0]), predicate, arg_count, ##__VA_ARGS__), sizeof(enumerable[0]))

/*
*	Retrieves a pointer to the first value in the enumerable that matches the predicate.
*
*	Enumerable_FindVargs(enumerable, predicate, arg_count, ap)
*	Enumerable_FindVargs(enumerable, predicate, arg_count, ap, length)
*	Enumerable_FindVargs(enumerable, predicate, arg_count, ap, length, start)
*
*	/param		enumerable			Pointer to the enumerable.
*	/param		predicate		A pointer to a function (ENUMERABLE_PREDICATE) which checks each value to its conditions.
*	/param		arg_count		How many arguments have been passed.
*	/param		ap				The variadic arguments. *Will call va_end before returning.
*	/param		length			(Optional)The count of elements in the enumerable. (eg. int[10] = 10)
*	/param		start			(Optional)The index to start at.
*
*	/returns	uint8_t*		A pointer to the first value in the enumerable that matches the predicate, or null if there were no matches.
*/
#define Enumerable_FindVargs(...) ENUMERABLE_FIND_VARGS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)



#pragma endregion Macros

#pragma region Unsafe Macros

/*
*	Retrieves an enumerable of pointers to the elements that match the conditions defined by the specified predicate.
*
*	Enumerable_FindAll(enumerable, predicate, out_count)
*	Enumerable_FindAll(enumerable, predicate, length, out_count)
*	Enumerable_FindAll(enumerable, predicate, length, start, out_count)
*
*	/param	enumerable			*			Pointer to the start of the enumerable.
*	/param	length			uint		(Optional)The count of elements in the enumerable. (eg. int[10] = 10)
*	/param	predicate		PREDICATE	A pointer to a function (ENUMERABLE_PREDICATE) which checks each value to its conditions.
*	/param	out_length		int*		(Optional)The amount of matches in the enumerable.
*
*	/returns	uint8t**	An enumerable of pointers to each value in the enumerable that matched the predicate's conditions
*/
#define Enumerable_FindAll(...) ENUMERABLE_FINDALL_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Inserts a range of values into an enumerable.
*	Works by allocating enough memory to fit the original enumerable, plus the new values.
*	It then copies the enumerable and the values into the new enumerable, and returns a pointer to where the new enumerable was allocated.
*	*WARNING : length must be passed for pointer enumerables!
*
*	Enumerable_InsertRange(enumerable, range, index)	
*	Enumerable_InsertRange(enumerable, range, index, count)	
*	Enumerable_InsertRange(enumerable, range, index, count, length)										
*	Enumerable_InsertRange(enumerable, range, index, count, length, free_old)					
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	range			Pointer to the start of the range of values.
*	/param	index			Index at which to insert the range at.
*	/param	count			(Optional)The amount of values to insert (eg. 4 integers = 4, not sizeof(int) * 4)
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	free_old		(Optional)If true, the enumerable passed to "enumerable" will be freed after the copy.
*	/param	start			(Optional)The index to start at. (must be larger than index)
*
*	/returns	uint8_t*	A pointer to the start of the new enumerable.
*/
#define Enumerable_InsertRange(...) ENUMERABLE_INSERTRANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Inserts a value into an enumerable.
*	Works by allocating enough memory to fit the original enumerable, plus the new value.
*	It then copies the enumerable and the value into the new enumerable, and returns a pointer to where the new enumerable was allocated.
*	*WARNING : length must be passed for pointer enumerables!
*
*	Enumerable_Insert(enumerable, value, index)
*	Enumerable_Insert(enumerable, value, index, length)
*	Enumerable_Insert(enumerable, value, index, length, free_old)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	value			The value to insert.
*	/param	index			The index where the new values should be inserted at.
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	free_old		(Optional)If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new enumerable.
*/
#define Enumerable_Insert(...) ENUMERABLE_INSERT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Adds a range of values into the end of an enumerable.
*	*Calculates the offset and then calls enumerable_insertRange_memory.
*	*WARNING : length must be passed for pointer enumerables!
*
*	Enumerable_AddRange(enumerable, value)
*	Enumerable_AddRange(enumerable, value, count)
*	Enumerable_AddRange(enumerable, value, count, length)
*	Enumerable_AddRange(enumerable, value, count, length, free_old)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	value			The value to add.
*	/param	count			(Optional)The amount of values to insert (eg. 4 integers = 4, not sizeof(int) * 4)
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	free_old		(Optional)If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new enumerable.
*/
#define Enumerable_AddRange(...) ENUMERABLE_ADDRANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Adds a value to the end of an enumerable.
*	*Calculates the offset and then calls enumerable_insertRange_memory with a count of 1.
*	*WARNING : length must be passed for pointer enumerables!
*
*	Enumerable_Add(enumerable, value)
*	Enumerable_Add(enumerable, value, length)
*	Enumerable_Add(enumerable, value, length, free_old)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	value			The value to add.
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	free_old		(Optional)If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new enumerable.
*/
#define Enumerable_Add(...) ENUMERABLE_ADD_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Adds a range of values on to the start of an enumerable.
*	*Calculates the offset and then calls enumerable_insertRange_memory.
*	*WARNING : length must be passed for pointer enumerables!
*
*	Enumerable_PrependRange(enumerable, range)
*	Enumerable_PrependRange(enumerable, range, count)
*	Enumerable_PrependRange(enumerable, range, count, length)
*	Enumerable_PrependRange(enumerable, range, count, length, free_old)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	value			The value to prepend.
*	/param	index			The index where the new values should be prepended at.
*	/param	count			(Optional)The amount of values to insert (eg. 4 integers = 4, not sizeof(int) * 4)
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	free_old		(Optional)If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new enumerable.
*/
#define Enumerable_PrependRange(...) ENUMERABLE_PREPENDRANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Adds a value to the start of an enumerable.
*	*Calculates the offset and then calls enumerable_insertRange_memory with a count of 1.
*	*WARNING : length must be passed for pointer enumerables!
*
*	Enumerable_Prepend(enumerable, value)
*	Enumerable_Prepend(enumerable, value, length)
*	Enumerable_Prepend(enumerable, value, length, free_old)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	value			The value to prepend.
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	free_old		(Optional)If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new enumerable.
*/
#define Enumerable_Prepend(...) ENUMERABLE_PREPEND_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)



/*
*	Removes a range of values from an enumerable.
*	Works by allocating memory to fit the original enumerable, minus the count.
*	It then copies the data surrounding offset + count from the old enumerable to the new, and returns a pointer to where the new enumerable was allocated.
*	*WARNING : length must be passed for pointer enumerables!
*
*	Enumerable_RemoveRange(enumerable,  index, count)
*	Enumerable_RemoveRange(enumerable,  index, count, length)
*	Enumerable_RemoveRange(enumerable,  index, count, length, free_old)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	index			The index of the element to start removing at.
*	/param	count			The amount of elements to remove
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	free_old		(Optional)If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new enumerable.
*/
#define Enumerable_RemoveRange(...) ENUMERABLE_REMOVERANGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Removes a value from an enumerable by its index.
*	*WARNING : length must be passed for pointer enumerables!
*
*	Enumerable_RemoveAt(enumerable,  index)
*	Enumerable_RemoveAt(enumerable,  index, length)
*	Enumerable_RemoveAt(enumerable,  index, length, free_old)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	index			The index of the element to remove.
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	free_old		(Optional)If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new enumerable.
*/
#define Enumerable_RemoveAt(...) ENUMERABLE_REMOVEAT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Removes the first item from an enumerable which equals "value."
*	*WARNING : length must be passed for pointer enumerables!
*
*	Enumerable_Remove(enumerable, value)
*	Enumerable_Remove(enumerable, value, out_remove)
*	Enumerable_Remove(enumerable, value, length, out_remove)
*	Enumerable_Remove(enumerable, value, length, free_old, out_remove)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	value			The value to remove.
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	free_old		(Optional)If true, the enumerable passed to "enumerable" will be freed after the copy.
*	/param	*out_removed	(Out, Optional)Returns true or false depending if a value was found and removed.
*
*	/returns	uint8_t*	A pointer to the start of the new enumerable.
*/
#define Enumerable_Remove(...) ENUMERABLE_REMOVE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Removes all items from an enumerable which equals "value."
*	*WARNING : length must be passed for pointer enumerables!
*
*	Enumerable_RemoveAll(enumerable, value)
*	Enumerable_RemoveAll(enumerable, value, out_count)
*	Enumerable_RemoveAll(enumerable, value, length, out_count)
*	Enumerable_RemoveAll(enumerable, value, length, free_old, out_count)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	value			The value to determine which elements to remove.
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	free_old		(Optional)If true, the enumerable passed to "enumerable" will be freed after the copy.
*	/param	*out_count		(Out, Optional)Returns amount of values removed from the enumerable.
*
*	/returns	uint8_t*	A pointer to the start of the new enumerable.
*/
#define Enumerable_RemoveAll(...) ENUMERABLE_REMOVEALL_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Reverse the enumerable in memory.
*	*WARNING : length must be passed for pointer enumerables!
*
*	Enumerable_Reverse(enumerable)
*	Enumerable_Reverse(enumerable, length)
*	Enumerable_Reverse(enumerable, length, free_old)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	free_old		(Optional)If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new enumerable.
*/
#define Enumerable_Reverse(...) ENUMERABLE_REVERSE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


//#define Enumerable_Sort(enumerable, comparison)


/*
*	Assigns the given value to the elements of the specified enumerable.
*	
*	Enumerable_Fill(enumerable, value)
*	Enumerable_Fill(enumerable, value, length)
*	Enumerable_Fill(enumerable, value, length, start)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	value			Assigns the given value(or pointer to value) to each element of the specified enumerable.
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	start			(Optional)The index to start at.
*/
#define Enumerable_Fill(...)		ENUMERABLE_FILL_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Converts an enumerable of one data type, to an enumerable of a different data type.
*
*	Enumerable_ConvertTo(enumerable, oldType, newType)
*	Enumerable_ConvertTo(enumerable, length, oldType, newType)
*	Enumerable_ConvertTo(enumerable, length, oldType, newType, free_old)
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	length			(Optional)The count of elements in the enumerable. *Required for pointer enumerables.
*	/param	oldType			The type of values the original enumerable stores.
*	/param	newType			The new type to convert to.
*	/param	free_old		(Optional)If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns	uint8_t*	A pointer to the start of the new enumerable.
*/
#define Enumerable_ConvertTo(...) ENUMERABLE_CONVERTTO_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

//#define Enumerable_ConvertAll(...) 

#pragma endregion Unsafe Macros


#endif /* ENUMERABLE_GENETICC_H_ */
