#ifndef GENETICC_ARRAY_INTERNAL_MACROS_H_
#define GENETICC_ARRAY_INTERNAL_MACROS_H_

#include "array_geneticc_internal_functions.h"
//#include "Addio/GeNETiCC/config/array_geneticc_config.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

#include "Addio/GeNETiCC/geneticc.h"


#pragma region INTERNAL Macros - INTERNAL USE


/*
*	Macro used to select functions by type.
*	*More info in array_geneticc.c
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to search for.
*	/param	length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			If the array contains the value.
*/
#warning reminder to add optimization levels for all macros.
#define REPEATER_MACRO_GENETICC_ARRAY_CONTAINS_MEMORY(array, value, length, elem_size) internal_array_contains_memory(array, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size)

#if GENETICC_OPTIMIZATION_LEVEL <= GENETICC_OPTIMIZATION_LEVEL_SPEED_1 || GENETICC_ALWAYS_INCLUDE_TYPE_SPECIFIC == true
#define INTERNAL_ARRAY_CONTAINS(array, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ARRAY_CONTAINS_MEMORY(array, value, length, elem_size))										\
default:				internal_array_contains_generic(array, (generic_union_t)value, (length * elem_size), elem_size))
#else
#define INTERNAL_ARRAY_CONTAINS(array, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT(REPEATER_MACRO_GENETICC_ARRAY_CONTAINS_MEMORY(array, value, length, elem_size))							\
float :					internal_call_array_contains_special(array, (generic_union_t)value, length, TYPE_FLOAT),								\
double :				internal_call_array_contains_special(array, (generic_union_t)value, length, TYPE_DOUBLE),								\
default:				internal_array_contains_generic(array, (generic_union_t)value, (length * elem_size), elem_size))
#endif

#define ARRAY_CONTAINS_4(array, value, length, start)	INTERNAL_ARRAY_CONTAINS((array + start), value, length, sizeof(array[0]))
#define ARRAY_CONTAINS_3(array, value, length)			INTERNAL_ARRAY_CONTAINS(array, value, length, sizeof(array[0]))
#define ARRAY_CONTAINS_2(array, value)					INTERNAL_ARRAY_CONTAINS(array, value, (sizeof(array) / sizeof(array[0]) ), sizeof(array[0]))
#define ARRAY_CONTAINS_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,					\
ARRAY_CONTAINS_4,							\
ARRAY_CONTAINS_3,							\
ARRAY_CONTAINS_2, )

/*
*	/param	array		Pointer to the start of the array.
*	/param	length		The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	predicate	A function which checks each value to its conditions.
*
*	/returns			True if an element in the array satisfied the predicate's condition, false if the predicate was unsatisfied.
*/
#warning reminder to add optimization levels for all macros.
#define INTERNAL_ARRAY_EXISTS(array, size, elem_size, predicate) \
internal_array_exists(array, size, elem_size, predicate)

#define ARRAY_EXISTS_4(array, predicate, length, start)		INTERNAL_ARRAY_EXISTS((array + start), length, sizeof(array[0]), predicate)
#define ARRAY_EXISTS_3(array, predicate, length)			INTERNAL_ARRAY_EXISTS(array, length, sizeof(array[0]), predicate)
#define ARRAY_EXISTS_2(array, predicate)					INTERNAL_ARRAY_EXISTS(array, (sizeof(array) / sizeof(array[0]) ), sizeof(array[0]), predicate)
#define ARRAY_EXISTS_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,					\
ARRAY_EXISTS_4,							\
ARRAY_EXISTS_3,							\
ARRAY_EXISTS_2, )

/*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to search for.
*	/param	length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the value in the array, or -1 if it does not exist.
*/
#define REPEATER_MACRO_GENETICC_ARRAY_INDEXOF_MEMORY(array, value, length, elem_size) internal_array_indexOf_memory(array, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size)

#if GENETICC_OPTIMIZATION_LEVEL <= GENETICC_OPTIMIZATION_LEVEL_SPEED_1 || GENETICC_ALWAYS_INCLUDE_TYPE_SPECIFIC == true
#define INTERNAL_ARRAY_INDEXOF(array, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ARRAY_INDEXOF_MEMORY(array, value, length, elem_size))										\
default:				internal_array_indexOf_generic(array, (generic_union_t)value, (length * elem_size), elem_size))
#else
#define INTERNAL_ARRAY_INDEXOF(array, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT(REPEATER_MACRO_GENETICC_ARRAY_INDEXOF_MEMORY(array, value, length, elem_size))								\
float :					internal_call_array_indexOf_special(array, (generic_union_t)value, length, TYPE_FLOAT),									\
double :				internal_call_array_indexOf_special(array, (generic_union_t)value, length, TYPE_DOUBLE),								\
default:				internal_array_indexOf_generic(array, (generic_union_t)value, (length * elem_size), elem_size))
#endif

#define ARRAY_INDEXOF_4(array, value, length, start)	INTERNAL_ARRAY_GET_TRUE_INDEX(	INTERNAL_ARRAY_INDEXOF((array + start), value, length, sizeof(array[0])), start)
#define ARRAY_INDEXOF_3(array, value, length)											INTERNAL_ARRAY_INDEXOF(array, value, length, sizeof(array[0]))
#define ARRAY_INDEXOF_2(array, value)													INTERNAL_ARRAY_INDEXOF(array, value, (sizeof(array) / sizeof(array[0])), sizeof(array[0]))
#define ARRAY_INDEXOF_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,					\
ARRAY_INDEXOF_4,							\
ARRAY_INDEXOF_3,							\
ARRAY_INDEXOF_2, )



/*
*	Macro used to select functions by type.
*	*More info in array_geneticc.c
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to search for.
*	/param	length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The last index of the value in the array, or -1 if it does not exist.
*/
#define REPEATER_MACRO_GENETICC_ARRAY_LASTINDEXOF_MEMORY(array, value, length, elem_size) internal_array_lastIndexOf_memory(array, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size)

#if GENETICC_OPTIMIZATION_LEVEL <= GENETICC_OPTIMIZATION_LEVEL_SPEED_1 || GENETICC_ALWAYS_INCLUDE_TYPE_SPECIFIC == true
#define INTERNAL_ARRAY_LASTINDEXOF(array, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ARRAY_LASTINDEXOF_MEMORY(array, value, length, elem_size))										\
default:				internal_array_lastIndexOf_generic(array, (generic_union_t)value, (length * elem_size), elem_size))
#else
#define INTERNAL_ARRAY_LASTINDEXOF(array, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT(REPEATER_MACRO_GENETICC_ARRAY_LASTINDEXOF_MEMORY(array, value, length, elem_size))								\
float :					internal_call_array_indexOf_special(array, (generic_union_t)value, length, TYPE_FLOAT),										\
double :				internal_call_array_indexOf_special(array, (generic_union_t)value, length, TYPE_DOUBLE),									\
default:				internal_array_lastIndexOf_generic(array, (generic_union_t)value, (length * elem_size), elem_size))
#endif

#define ARRAY_LASTINDEXOF_4(array, value, length, start)	INTERNAL_ARRAY_GET_TRUE_INDEX(	INTERNAL_ARRAY_LASTINDEXOF(array + start), (value, length, sizeof(array[0])), start)
#define ARRAY_LASTINDEXOF_3(array, value, length)											INTERNAL_ARRAY_LASTINDEXOF(array, value, length, sizeof(array[0]))
#define ARRAY_LASTINDEXOF_2(array, value)													INTERNAL_ARRAY_LASTINDEXOF(array, value, (sizeof(array) / sizeof(array[0])), sizeof(array[0]))
#define ARRAY_LASTINDEXOF_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,						\
ARRAY_LASTINDEXOF_4,							\
ARRAY_LASTINDEXOF_3,							\
ARRAY_LASTINDEXOF_2, )


\


/*
*	Macro used to select functions by type.
*	*More info in array_geneticc.c
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to match and count.
*	/param	length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The amount of times "value" is in the array.
*/
#define REPEATER_MACRO_GENETICC_ARRAY_VALUECOUNT_MEMORY(array, value, length, elem_size) internal_array_valueCount_memory(array, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size)
#define INTERNAL_ARRAY_VALUECOUNT(array, value, length, elem_size) _Generic((value),										\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ARRAY_VALUECOUNT_MEMORY(array, value, length, elem_size))				\
default:				internal_array_valueCount_generic(array, (generic_union_t)value, (length * elem_size), elem_size))

#define ARRAY_VALUECOUNT_4(array, value, length, start)		INTERNAL_ARRAY_VALUECOUNT((array + start), value, length, sizeof(array[0]))
#define ARRAY_VALUECOUNT_3(array, value, length)			INTERNAL_ARRAY_VALUECOUNT(array, value, length, sizeof(array[0]))
#define ARRAY_VALUECOUNT_2(array, value)					INTERNAL_ARRAY_VALUECOUNT(array, value, (sizeof(array) / sizeof(array[0])), sizeof(array[0]))
#define ARRAY_VALUECOUNT_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,					\
ARRAY_VALUECOUNT_4,							\
ARRAY_VALUECOUNT_3,							\
ARRAY_VALUECOUNT_2, )


/*
*	Macro used to select functions by type.
*	*More info in array_geneticc.c
*
*	/param	array		Pointer to the start of the array.
*	/param	length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the largest value in the array.
*/
#define REPEATER_MACRO_GENETICC_ARRAY_MAX_MEMORY_SIGNED(array, length, elem_size) internal_array_max_memory(array, (length * elem_size),elem_size, true)
#define REPEATER_MACRO_GENETICC_ARRAY_MAX_MEMORY_UNSIGNED(array, length, elem_size) internal_array_max_memory(array, (length * elem_size),elem_size, false)
#define INTERNAL_ARRAY_MAX(array, length, elem_size) _Generic((array),															\
GENETICC_REPEAT_POINTER_TYPES_SIGNED(REPEATER_MACRO_GENETICC_ARRAY_MAX_MEMORY_SIGNED(array, (length * elem_size),elem_size))		\
GENETICC_REPEAT_POINTER_TYPES_UNSIGNED(REPEATER_MACRO_GENETICC_ARRAY_MAX_MEMORY_UNSIGNED(array, (length * elem_size),elem_size))	\
float* :					internal_array_maxf_memory(array, (length * elem_size),elem_size),									\
double* :					internal_array_maxf_memory(array, (length * elem_size),elem_size))

#define ARRAY_MAX_3(array, length, start)	INTERNAL_ARRAY_MAX((array + start), length, sizeof(array[0]), start)
#define ARRAY_MAX_2(array, length)			INTERNAL_ARRAY_MAX(array, length, sizeof(array[0]))
#define ARRAY_MAX_1(array)					INTERNAL_ARRAY_MAX(array, (sizeof(array) / sizeof(array[0])), sizeof(array[0]))
#define ARRAY_MAX_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,				\
ARRAY_MAX_3,							\
ARRAY_MAX_2,							\
ARRAY_MAX_1, )

#define ARRAY_MAX_VALUE_3(array, length, start)		array[INTERNAL_ARRAY_GET_TRUE_INDEX(	INTERNAL_ARRAY_MAX((array + start), length, sizeof(array[0])), start)]
#define ARRAY_MAX_VALUE_2(array, length)			array[									INTERNAL_ARRAY_MAX(array, length, sizeof(array[0]))]
#define ARRAY_MAX_VALUE_1(array)					array[									INTERNAL_ARRAY_MAX(array, (sizeof(array) / sizeof(array[0])), sizeof(array[0]))]
#define ARRAY_MAX_VALUE_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,					\
ARRAY_MAX_VALUE_3,							\
ARRAY_MAX_VALUE_2,							\
ARRAY_MAX_VALUE_1, )



/*
*	Macro used to select functions by type.
*	*More info in array_geneticc.c
*
*	/param	array		Pointer to the start of the array.
*	/param	length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the smallest value in the array.
*/
#define REPEATER_MACRO_GENETICC_ARRAY_MIN_MEMORY_SIGNED(array, length, elem_size) internal_array_min_memory(array, (length * elem_size),elem_size, true)
#define REPEATER_MACRO_GENETICC_ARRAY_MIN_MEMORY_UNSIGNED(array, length, elem_size) internal_array_min_memory(array, (length * elem_size),elem_size, false)
#define INTERNAL_ARRAY_MIN(array, length, elem_size) _Generic((array),															\
GENETICC_REPEAT_POINTER_TYPES_SIGNED(REPEATER_MACRO_GENETICC_ARRAY_MIN_MEMORY_SIGNED(array, (length * elem_size),elem_size))		\
GENETICC_REPEAT_POINTER_TYPES_UNSIGNED(REPEATER_MACRO_GENETICC_ARRAY_MIN_MEMORY_UNSIGNED(array, (length * elem_size),elem_size))	\
float* :						internal_array_minf_memory(array, (length * elem_size),elem_size),								\
double* :						internal_array_minf_memory(array, (length * elem_size),elem_size))

#define ARRAY_MIN_3(array, length, start)	INTERNAL_ARRAY_MIN((array + start), length, sizeof(array[0]))
#define ARRAY_MIN_2(array, length)			INTERNAL_ARRAY_MIN(array, length, sizeof(array[0]))
#define ARRAY_MIN_1(array)					INTERNAL_ARRAY_MIN(array, (sizeof(array) / sizeof(array[0])), sizeof(array[0]))
#define ARRAY_MIN_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,				\
ARRAY_MIN_3,							\
ARRAY_MIN_2,							\
ARRAY_MIN_1, )

#define ARRAY_MIN_VALUE_3(array, length, start)		array[INTERNAL_ARRAY_GET_TRUE_INDEX(	INTERNAL_ARRAY_MIN((array + start), length, sizeof(array[0])), start)]
#define ARRAY_MIN_VALUE_2(array, length)			array[									INTERNAL_ARRAY_MIN(array, length, sizeof(array[0]))]
#define ARRAY_MIN_VALUE_1(array)					array[									INTERNAL_ARRAY_MIN(array, (sizeof(array) / sizeof(array[0])), sizeof(array[0]))]
#define ARRAY_MIN_VALUE_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,					\
ARRAY_MIN_VALUE_3,							\
ARRAY_MIN_VALUE_2,							\
ARRAY_MIN_VALUE_1, )


/*
*	Macro used to select functions by type.
*	*More info in array_geneticc.c
*
*	/param	array		Pointer to the start of the array.
*	/param	length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the smallest value in the array.
*/
#define REPEATER_MACRO_GENETICC_ARRAY_SUM_MEMORY(array, length, elem_size) internal_array_sum_memory(array, (length * elem_size),elem_size, false)


#define INTERNAL_ARRAY_SUM(array, length, elem_size) _Generic((array),														\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT_UNION(REPEATER_MACRO_GENETICC_ARRAY_SUM_MEMORY(array, length, elem_size))				\
float* :						internal_array_sumf_memory(array, (length * elem_size),elem_size),							\
double* :						internal_array_sumf_memory(array, (length * elem_size),elem_size))

#define ARRAY_SUM_3(array, length, start)	INTERNAL_ARRAY_SUM((array + start), length, sizeof(array[0]))
#define ARRAY_SUM_2(array, length)			INTERNAL_ARRAY_SUM(array, length, sizeof(array[0]))
#define ARRAY_SUM_1(array)					INTERNAL_ARRAY_SUM(array, (sizeof(array) / sizeof(array[0])), sizeof(array[0]))
#define ARRAY_SUM_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,				\
ARRAY_SUM_3,							\
ARRAY_SUM_2,							\
ARRAY_SUM_1, )



/*
*	Macro used to select functions by type.
*	*More info in array_geneticc.c
*
*	/param	array		Pointer to the start of the array.
*	/param	length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the smallest value in the array.
*/
#define REPEATER_MACRO_GENETICC_ARRAY_AVERAGE_MEMORY(array, length, elem_size) internal_array_average_memory(array, (length * elem_size),elem_size, false)

#define INTERNAL_ARRAY_AVERAGE(array, length, elem_size) _Generic((array),													\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT_UNION(REPEATER_MACRO_GENETICC_ARRAY_AVERAGE_MEMORY(array, length, elem_size))	\
float* :						internal_array_averagef_memory(array, (length * elem_size),elem_size),						\
double* :						internal_array_averagef_memory(array, (length * elem_size),elem_size))

#define ARRAY_AVERAGE_3(array, length, start)	INTERNAL_ARRAY_AVERAGE((array + start), length, sizeof(array[0]))// / length
#define ARRAY_AVERAGE_2(array, length)			INTERNAL_ARRAY_AVERAGE(array, length, sizeof(array[0])) /// length
#define ARRAY_AVERAGE_1(array)					INTERNAL_ARRAY_AVERAGE(array, (sizeof(array) / sizeof(array[0])), sizeof(array[0]))// / (sizeof(array) / sizeof(array[0]))
#define ARRAY_AVERAGE_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,					\
ARRAY_AVERAGE_3,							\
ARRAY_AVERAGE_2,							\
ARRAY_AVERAGE_1, )

/*
*	Macro used to select functions by type.
*	*More info in array_geneticc.c
*
*	/param	array		Pointer to the start of the array.
*	/param	length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	predicate	A pointer to a function (ARRAY_PREDICATE) which checks each value to its conditions.
*
*	/returns	uint8_t*	A pointer to the first value in the array that matches the predicate
*/
#define INTERNAL_ARRAY_SELECT(array, length, elem_size, predicate)	\
internal_array_select_memory(array, (length * elem_size), elem_size, predicate)

#define ARRAY_FINDINDEX_4(array, predicate, length, start)		INTERNAL_ARRAY_GET_TRUE_INDEX(INTERNAL_ARRAY_SELECT((array + start), length, sizeof(array[0]), predicate), start)
#define ARRAY_FINDINDEX_3(array, predicate, length)			INTERNAL_ARRAY_SELECT(array, length, sizeof(array[0]), predicate)
#define ARRAY_FINDINDEX_2(array, predicate)					INTERNAL_ARRAY_SELECT(array, (sizeof(array) / sizeof(array[0])), sizeof(array[0]), predicate)
#define ARRAY_FINDINDEX_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,				\
ARRAY_FINDINDEX_4,							\
ARRAY_FINDINDEX_3,							\
ARRAY_FINDINDEX_2, )

/*
*	Select Pointer
*/
#define ARRAY_FIND_4(array, predicate, length, start)	array +	INTERNAL_ARRAY_GET_TRUE_INDEX(	INTERNAL_ARRAY_SELECT((array + start), length, sizeof(array[0]), predicate), start)
#define ARRAY_FIND_3(array, predicate, length)			array +	INTERNAL_ARRAY_SELECT(array, length, sizeof(array[0]), predicate)
#define ARRAY_FIND_2(array, predicate)					array +	INTERNAL_ARRAY_SELECT(array, (sizeof(array) / sizeof(array[0])), sizeof(array[0]), predicate)
#define ARRAY_FIND_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,				\
ARRAY_FIND_4,							\
ARRAY_FIND_3,							\
ARRAY_FIND_2, )

/*
*	Array Select Vargs
*/
#define INTERNAL_ARRAY_FINDINDEX_VARGS(array, predicate, arg_count, ap, length, elem_size)	\
internal_array_select_vargs_memory(array, (length * elem_size), elem_size, predicate, arg_count, ap)


#define ARRAY_FINDINDEX_VARGS_6(array, predicate, arg_count, ap, length, start)				INTERNAL_ARRAY_GET_TRUE_INDEX(	INTERNAL_ARRAY_FINDINDEX_VARGS((array + start), predicate, arg_count, ap, length, sizeof(array[0])), start)
#define ARRAY_FINDINDEX_VARGS_5(array, predicate, arg_count, ap, length)														INTERNAL_ARRAY_FINDINDEX_VARGS(array, predicate, arg_count, ap, length, sizeof(array[0]))
#define ARRAY_FINDINDEX_VARGS_4(array, predicate, arg_count, ap)																INTERNAL_ARRAY_FINDINDEX_VARGS(array, predicate, arg_count, ap, sizeof(array) / sizeof(array[0]), sizeof(array[0]))
#define ARRAY_FINDINDEX_VARGS_MACRO_CHOOSER(...)	\
GET_ARGUMENT_6(__VA_ARGS__,						\
ARRAY_FINDINDEX_VARGS_6,							\
ARRAY_FINDINDEX_VARGS_5,							\
ARRAY_FINDINDEX_VARGS_4, )

/*
*	Select Pointer Vargs
*/
#define ARRAY_FIND_VARGS_6(array, predicate, arg_count, ap, length, start)					array +	INTERNAL_ARRAY_GET_TRUE_INDEX(	INTERNAL_ARRAY_FINDINDEX_VARGS((array + start), predicate, arg_count, ap, length,  sizeof(array[0])), start)
#define ARRAY_FIND_VARGS_5(array, predicate, arg_count, ap, length)						array +									INTERNAL_ARRAY_FINDINDEX_VARGS(array, predicate, arg_count, ap, length, sizeof(array[0]))
#define ARRAY_FIND_VARGS_4(array, predicate, arg_count, ap)																		INTERNAL_ARRAY_FINDINDEX_VARGS(array, predicate, arg_count, ap, sizeof(array) / sizeof(array[0]), sizeof(array[0]))
#define ARRAY_FIND_VARGS_MACRO_CHOOSER(...)	\				
GET_ARGUMENT_6(__VA_ARGS__,								\
ARRAY_FIND_VARGS_6,							\
ARRAY_FIND_VARGS_5,							\
ARRAY_FIND_VARGS_4, )



#pragma endregion INTERNAL Macros

/*
*	unsafe macros
*
*	Note :	If you use any of these functions more than once on the same array, its recommended that you use "list_geneticc.h" instead.
*			You can get a big performance improvement depending on which function is used, in trade for more memory being used (more memory but also less fragmentation).
*
*/
#pragma region INTERNAL Unsafe Macros - INTERNAL USE

/*
*	Macro used to select functions by type.
*	*More info in array_geneticc.c
*
*
*	/param	array		Pointer to the start of the array.
*	/param	length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	predicate	A pointer to a function which checks each value to its conditions.
*	/param	out_count	The amount of matches in the array.
*
*	/returns	uint8t**	A pointer to the first value in the array that matches the predicate
*/
#define INTERNAL_ARRAY_FINDALL(array, length, elem_size, predicate, out_length)	\
internal_array_selectMany_memory(array, (length * elem_size), elem_size, predicate, out_length)

/*
*	Macros which implement the overloading
*/
#define ARRAY_FINDALL_5(array, predicate, length, out_length, start)		INTERNAL_ARRAY_FINDALL((array + start), length, sizeof(array[0]), predicate, out_length)
#define ARRAY_FINDALL_4(array, predicate, length, out_length)			INTERNAL_ARRAY_FINDALL(array, length, sizeof(array[0]), predicate, out_length)
#define ARRAY_FINDALL_3(array, predicate, out_length)					INTERNAL_ARRAY_FINDALL(array, (sizeof(array) / sizeof(array[0])), sizeof(array[0]), predicate, out_length)
#define ARRAY_FINDALL_MACRO_CHOOSER(...)	\
GET_ARGUMENT_5(__VA_ARGS__,					\
ARRAY_FINDALL_5,							\
ARRAY_FINDALL_4,							\
ARRAY_FINDALL_3, )

/*
*	Macro used to select functions by type.
*	*More info in array_geneticc.c
*
*	/param	array		Pointer to the start of the array.
*	/param	range		The range of values to insert.
*	/param	index		The index where the new values should be inserted at.
*	/param	count		The amount of values to insert (eg. 4 integers = 4, not sizeof(int) * 4)
*	/param	length		The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	free_old		If true, the array passed to "array" will be freed after the copy.
*
*	/returns			A pointer to the start of the new array.
*/
#define INTERNAL_ARRAY_INSERTRANGE(array, range, index, count, length, elem_size, free_old ) \
internal_array_insertRange_memory(array, (generic_union_t){.u64 = (uint32_t)range}.cp, (index == 0 ? 0 : (index * elem_size)),  (count * elem_size), (length * elem_size), elem_size, free_old)

/*
*	Macros which implement the overloading
*/
#define ARRAY_INSERTRANGE_GENETICC_6(array, range, index,  count, length, free_old)			INTERNAL_ARRAY_INSERTRANGE(array, range, index, count, length, sizeof(array[0]), free_old)
#define ARRAY_INSERTRANGE_GENETICC_5(array, range, index,  count, length)					INTERNAL_ARRAY_INSERTRANGE(array, range, index, count, length, count,sizeof(array[0]), false)
#define ARRAY_INSERTRANGE_GENETICC_4(array, range, index,  count)							INTERNAL_ARRAY_INSERTRANGE(array, range, index, count, (sizeof(array) / sizeof(array[0])),  sizeof(array[0]), false)
#define ARRAY_INSERTRANGE_GENETICC_3(array, range, index)									INTERNAL_ARRAY_INSERTRANGE(array, range, index,(sizeof(range) * sizeof(range[0])), (sizeof(array) / sizeof(array[0])),  sizeof(array[0]), false)
#define ARRAY_INSERTRANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_6(__VA_ARGS__,							\
ARRAY_INSERTRANGE_GENETICC_6,						\
ARRAY_INSERTRANGE_GENETICC_5,						\
ARRAY_INSERTRANGE_GENETICC_4,						\
ARRAY_INSERTRANGE_GENETICC_3,)


/*
*	Macro used to select functions by type.
*	Can be used with arrays or pointers.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to search for.
*	/param	index		The index where the new value should be inserted at.
*	/param	length		The element count.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	free_old		If true, the array passed to "array" will be freed after the copy.
*
*	/returns			If the array contains the value.
*/
#define REPEATER_MACRO_GENETICC_ARRAY_INSERT_MEMORY(array, value, index,  length, elem_size, free_old) internal_array_insert_memory(array, (generic_union_t){.u32 = (uint32_t)value}.cp,  (index == 0 ? 0 : (index * elem_size)),  (length * elem_size), elem_size, free_old)
#define INTERNAL_ARRAY_INSERT(array, value, index,  length, elem_size, free_old ) _Generic((value),																			\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ARRAY_INSERT_MEMORY(array, value, index,  length, elem_size, free_old))													\
default:				internal_array_insert_generic(array, (generic_union_t)value, (index == 0 ? 0 : (index * elem_size)),  (length * elem_size),  elem_size, free_old))




/*
*	Macros which implement the overloading
*/
#define ARRAY_INSERT_GENETICC_5(array, value, index, length, free_old)			INTERNAL_ARRAY_INSERT(array, value, index,  length, sizeof(array[0]), free_old)
#define ARRAY_INSERT_GENETICC_4(array, value, index, length)						INTERNAL_ARRAY_INSERT(array, value, index,  length, sizeof(array[0]), false)
#define ARRAY_INSERT_GENETICC_3(array, value, index)								INTERNAL_ARRAY_INSERT(array, value, index, (sizeof(array) / sizeof(array[0])), sizeof(array[0]), false)
#define ARRAY_INSERT_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,					\
ARRAY_INSERT_GENETICC_5,						\
ARRAY_INSERT_GENETICC_4,						\
ARRAY_INSERT_GENETICC_3, )



/*
*	Macro used to select functions by type.
*	*More info in array_geneticc.c
*
*	/param	array		Pointer to the start of the array.
*	/param	range		The range of values to add.
*	/param	index		The index where the new values should be added at.
*	/param	count		The amount of values to add (eg. 4 integers = 4, not sizeof(int) * 4)
*	/param	length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	free_old		If true, the array passed to "array" will be freed after the copy.
*
*	/returns			A pointer to the start of the new array.
*/
#define INTERNAL_ARRAY_ADDRANGE(array, range, length, count, elem_size, free_old ) \
internal_array_insertRange_memory(array, (generic_union_t){.u64 = (uint32_t)range}.cp, length, (count * elem_size), (length * elem_size), elem_size, free_old)
//array_addRange_memory((generic_union_t){.u32 = (uint32_t)value}.cp,  array, count, (length * elem_size),  elem_size, free_old)

/*
*	Macros which implement the overloading
*/
#define ARRAY_ADDRANGE_GENETICC_5(array, range, length, count, free_old)			INTERNAL_ARRAY_ADDRANGE(array, range, count, length, sizeof(array[0]), free_old)
#define ARRAY_ADDRANGE_GENETICC_4(array, range, length, count)					INTERNAL_ARRAY_ADDRANGE(array, range, count, length, sizeof(array[0]), false)
#define ARRAY_ADDRANGE_GENETICC_3(array, range, count)							INTERNAL_ARRAY_ADDRANGE(array, range, count, (sizeof(range) * sizeof(range[0])), sizeof(array[0]), false)
#define ARRAY_ADDRANGE_GENETICC_2(array, range)									INTERNAL_ARRAY_ADDRANGE(array, range, (sizeof(array) / sizeof(array[0])), (sizeof(range) * sizeof(range[0])),sizeof(array[0]), false)
#define ARRAY_ADDRANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,						\
ARRAY_ADDRANGE_GENETICC_5,						\
ARRAY_ADDRANGE_GENETICC_4,						\
ARRAY_ADDRANGE_GENETICC_3,						\
ARRAY_ADDRANGE_GENETICC_2,)



/*
*	Macro used to select functions by type.
*	Can be used with arrays or pointers.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to search for.
*	/param	length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	free_old	If true, the array passed to "array" will be freed after the copy.
*
*	/returns			If the array contains the value.
*/
#define REPEATER_MACRO_GENETICC_ARRAY_ADD_MEMORY(array, value, length, elem_size, free_old ) internal_array_add_memory(array, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size, free_old)
#define INTERNAL_ARRAY_ADD(array, value, length, elem_size, free_old ) _Generic((value),											\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ARRAY_ADD_MEMORY(array, value, length, elem_size, free_old ))					\
default:				internal_array_add_generic(array, (generic_union_t)value, (length * elem_size), elem_size, free_old))



/*
*	Macros which implement the overloading
*/
#define ARRAY_ADD_GENETICC_4(array, value, length, free_old)			INTERNAL_ARRAY_ADD(array, value, length, sizeof(array[0]), free_old)
#define ARRAY_ADD_GENETICC_3(array, value, length)					INTERNAL_ARRAY_ADD(array, value, length, sizeof(array[0]), false)
#define ARRAY_ADD_GENETICC_2(array, value)							INTERNAL_ARRAY_ADD(array, value, (sizeof(array) / sizeof(array[0])), sizeof(array[0]), false)
#define ARRAY_ADD_MACRO_CHOOSER(...)		\
GET_ARGUMENT_4(__VA_ARGS__,					\
ARRAY_ADD_GENETICC_4,						\
ARRAY_ADD_GENETICC_3,						\
ARRAY_ADD_GENETICC_2, )




/*
*	Macro used to select functions by type.
*	*More info in array_geneticc.c
*
*	/param	array		Pointer to the start of the array.
*	/param	range		The range of values to prepend.
*	/param	index		The index where the new values should be prepended at.
*	/param	count		The amount of values to prepend (eg. 4 integers = 4, not sizeof(int) * 4)
*	/param  length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	free_old		If true, the array passed to "array" will be freed after the copy.
*
*	/returns			A pointer to the start of the new array.
*/
#define INTERNAL_ARRAY_PREPENDRANGE(array, range,  count, length,  elem_size, free_old ) \
internal_array_insertRange_memory(array,  (generic_union_t){.u64 = (uint32_t)range}.cp, 0,  (count * elem_size), (length * elem_size), elem_size, free_old)
//array_prependRange_memory((generic_union_t){.u32 = (uint32_t)value}.cp,  array, (count * elem_size), (length * elem_size), elem_size, free_old)

/*
*	Prepends the ability to call one macro with different amounts of parameters
*/
#define ARRAY_PREPENDRANGE_GENETICC_5(array, range, count, length, free_old)			INTERNAL_ARRAY_PREPENDRANGE(array, range, count,  length, sizeof(array[0]), free_old)
#define ARRAY_PREPENDRANGE_GENETICC_4(array, range, count, length)					INTERNAL_ARRAY_PREPENDRANGE(array, range, count,  length, sizeof(array[0]), false)
#define ARRAY_PREPENDRANGE_GENETICC_3(array, range, count)							INTERNAL_ARRAY_PREPENDRANGE(array, range, count, (sizeof(array) / sizeof(array[0])), sizeof(array[0]), false)
#define ARRAY_PREPENDRANGE_GENETICC_2(array, range)									INTERNAL_ARRAY_PREPENDRANGE(array, range, (sizeof(range) * sizeof(range[0])), (sizeof(array) / sizeof(array[0])), sizeof(array[0]), false)
#define ARRAY_PREPENDRANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,							\
ARRAY_PREPENDRANGE_GENETICC_5,						\
ARRAY_PREPENDRANGE_GENETICC_4,						\
ARRAY_PREPENDRANGE_GENETICC_3,						\
ARRAY_PREPENDRANGE_GENETICC_2,)




/*
*	Macro used to select functions by type.
*	Can be used with arrays or pointers.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to search for.
*	/param  length  The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	free_old		If true, the array passed to "array" will be freed after the copy.
*
*	/returns			If the array contains the value.
*/
#define REPEATER_MACRO_GENETICC_ARRAY_PREPEND_MEMORY(array, value, length, elem_size, free_old ) internal_array_prepend_memory(array, (generic_union_t){.u32 = (uint32_t)value}.cp,  (length * elem_size), elem_size, free_old)
#define INTERNAL_ARRAY_PREPEND(array, value, length, elem_size, free_old ) _Generic((value),											\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ARRAY_PREPEND_MEMORY(array, value, length, elem_size, free_old ))					\
default:				internal_array_prepend_generic(array, (generic_union_t)value, (length * elem_size), elem_size, free_old))



/*
*	Prepends the ability to call one macro with different amounts of parameters
*/
#define ARRAY_PREPEND_GENETICC_4(array, value, length, free_old)			INTERNAL_ARRAY_PREPEND(array, value, length, sizeof(array[0]), free_old)
#define ARRAY_PREPEND_GENETICC_3(array, value, length)					INTERNAL_ARRAY_PREPEND(array, value, length, sizeof(array[0]), false)
#define ARRAY_PREPEND_GENETICC_2(array, value)							INTERNAL_ARRAY_PREPEND(array, value, (sizeof(array) / sizeof(array[0])), sizeof(array[0]), false)
#define ARRAY_PREPEND_MACRO_CHOOSER(...)		\
GET_ARGUMENT_4(__VA_ARGS__,						\
ARRAY_PREPEND_GENETICC_4,						\
ARRAY_PREPEND_GENETICC_3,						\
ARRAY_PREPEND_GENETICC_2, )


/*
*	Macro used to select functions by type.
*	Can be used with arrays or pointers.
*
*	/param	array			Pointer to the start of the array.
*	/param	index			The index of the element to start removing at.
*	/param	count			The amount of elements to remove
*	/param	length			The count of elements
*	/param	elem_size		The size of value's type (in bytes).
*	/param	free_old		If true, the array passed to "array" will be freed after the copy.
*
*	/returns	uint8_t*	If the array contains the value.
*/
#define INTERNAL_ARRAY_REMOVERANGE(array, index, count, length,  elem_size, free_old ) \
internal_array_removeRange_memory(array, (index * elem_size), (count * elem_size), (length * elem_size), elem_size, free_old)



/*
*	Macros which implement the overloading
*/
#define ARRAY_REMOVERANGE_GENETICC_5(array, index, count, length, free_old)			INTERNAL_ARRAY_REMOVERANGE(array, index, count, length, sizeof(array[0]), free_old)
#define ARRAY_REMOVERANGE_GENETICC_4(array, index, count, length)					INTERNAL_ARRAY_REMOVERANGE(array, index, count, length, sizeof(array[0]), false)
#define ARRAY_REMOVERANGE_GENETICC_3(array, index, count)							INTERNAL_ARRAY_REMOVERANGE(array, index, count,  (sizeof(array) / sizeof(array[0])), sizeof(array[0]), false)
#define ARRAY_REMOVERANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,							\
ARRAY_REMOVERANGE_GENETICC_5,						\
ARRAY_REMOVERANGE_GENETICC_4,						\
ARRAY_REMOVERANGE_GENETICC_3,)





/*
*	Macro used to select functions by type.
*	Can be used with arrays or pointers.
*
*	/param	array			Pointer to the start of the array.
*	/param	index			The index of the element to remove.
*	/param	length			The count of elements
*	/param	elem_size		The size of value's type (in bytes).
*	/param	free_old		If true, the array passed to "array" will be freed after the copy.
*
*	/returns	uint8_t*	If the array contains the value.
*/
#define INTERNAL_ARRAY_REMOVEAT(array, index, length,  elem_size, free_old ) \
internal_array_removeAt_memory(array, (index * elem_size), (length * elem_size), elem_size, free_old)



/*
*	Macros which implement the overloading
*/
#define ARRAY_REMOVEAT_GENETICC_4(array, index, length, free_old)			INTERNAL_ARRAY_REMOVEAT(array, index, length, sizeof(array[0]), free_old)
#define ARRAY_REMOVEAT_GENETICC_3(array, index, length)						INTERNAL_ARRAY_REMOVEAT(array, index, length, sizeof(array[0]), false)
#define ARRAY_REMOVEAT_GENETICC_2(array, index)								INTERNAL_ARRAY_REMOVEAT(array, index, (sizeof(array) / sizeof(array[0])), sizeof(array[0]), false)
#define ARRAY_REMOVEAT_MACRO_CHOOSER(...)		\
GET_ARGUMENT_4(__VA_ARGS__,						\
ARRAY_REMOVEAT_GENETICC_4,						\
ARRAY_REMOVEAT_GENETICC_3,						\
ARRAY_REMOVEAT_GENETICC_2,)


/*
*	Macro used to select functions by type.
*	Can be used with arrays or pointers.
*
*	/param	array			Pointer to the start of the array.
*	/param	value			The value to remove.
*	/param	length			The count of elements
*	/param	elem_size		The size of value's type (in bytes).
*	/param	free_old		If true, the array passed to "array" will be freed after the copy.
*	/param	out_removed*	(Out)Returns true or false depending if a value was found and removed.
*
*	/returns	uint8_t*	If the array contains the value.
*/
#define REPEATER_MACRO_GENETICC_ARRAY_REMOVE_MEMORY(array, value, length, elem_size, free_old, out_removed  ) internal_array_remove_memory(array, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size, free_old, out_removed)
#define INTERNAL_ARRAY_REMOVE(array, value, length, elem_size, free_old, out_removed )  _Generic((value),											\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ARRAY_REMOVE_MEMORY(array, value, length, elem_size, free_old, out_removed  ))					\
default:				internal_array_remove_generic(array, (generic_union_t)value, (length * elem_size), elem_size, free_old, out_removed))

#define ARRAY_REMOVE_GENETICC_5(array, value, length, free_old, out_removed )		INTERNAL_ARRAY_REMOVE(array, value, length, sizeof(array[0]), free_old, out_removed )
#define ARRAY_REMOVE_GENETICC_4(array, value, length, out_removed)					INTERNAL_ARRAY_REMOVE(array, value, length, sizeof(array[0]), false, out_removed )
#define ARRAY_REMOVE_GENETICC_3(array, value, out_removed)							INTERNAL_ARRAY_REMOVE(array, value, (sizeof(array) / sizeof(array[0])), sizeof(array[0]), false, out_removed )
#define ARRAY_REMOVE_GENETICC_2(array, value)										INTERNAL_ARRAY_REMOVE(array, value, (sizeof(array) / sizeof(array[0])), sizeof(array[0]), false, 0 )
#define ARRAY_REMOVE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,					\
ARRAY_REMOVE_GENETICC_5,						\
ARRAY_REMOVE_GENETICC_4,						\
ARRAY_REMOVE_GENETICC_3,						\
ARRAY_REMOVE_GENETICC_2,)



/*
*	Macro used to select functions by type.
*	Can be used with arrays or pointers.
*
*	/param	array			Pointer to the start of the array.
*	/param	value			The value to determine which elements to remove.
*	/param	length			The count of elements
*	/param	elem_size		The size of value's type (in bytes).
*	/param	free_old		If true, the array passed to "array" will be freed after the copy.
*	/param	out_count*		(Out)Returns amount of values removed from the array.
*
*	/returns	uint8_t*	If the array contains the value.
*/
#define REPEATER_MACRO_GENETICC_ARRAY_REMOVEALL_MEMORY(array, value, length, elem_size, free_old, out_count  ) internal_array_removeAll_memory(array, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size, free_old, out_count)
#define INTERNAL_ARRAY_REMOVEALL(array, value, length, elem_size, free_old, out_count )  _Generic((value),											\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ARRAY_REMOVEALL_MEMORY(array, value, length, elem_size, free_old, out_count  ))					\
default:				internal_array_removeAll_generic(array, (generic_union_t)value, (length * elem_size), elem_size, free_old, out_count))


#define ARRAY_REMOVEALL_GENETICC_5(array, value, length, free_old, out_count )			INTERNAL_ARRAY_REMOVEALL(array, value, length, sizeof(array[0]), free_old, out_count )
#define ARRAY_REMOVEALL_GENETICC_4(array, value, length, out_count)						INTERNAL_ARRAY_REMOVEALL(array, value, length, sizeof(array[0]), false, out_count )
#define ARRAY_REMOVEALL_GENETICC_3(array, value, out_count)								INTERNAL_ARRAY_REMOVEALL(array, value, (sizeof(array) / sizeof(array[0])), sizeof(array[0]), false, out_count )
#define ARRAY_REMOVEALL_GENETICC_2(array, value)											INTERNAL_ARRAY_REMOVEALL(array, value, (sizeof(array) / sizeof(array[0])), sizeof(array[0]), false, 0 )
#define ARRAY_REMOVEALL_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,						\
ARRAY_REMOVEALL_GENETICC_5,						\
ARRAY_REMOVEALL_GENETICC_4,						\
ARRAY_REMOVEALL_GENETICC_3,						\
ARRAY_REMOVEALL_GENETICC_2,)





/*
*	Macro used to select functions by type.
*	Can be used with arrays or pointers.
*
*	/param	array			Pointer to the start of the array.
*	/param	length			The count of elements
*	/param	elem_size		The size of value's type (in bytes).
*	/param	free_old		If true, the array passed to "array" will be freed after the copy.
*
*	/returns	uint8_t*	If the array contains the value.
*/
#define INTERNAL_ARRAY_REVERSE(array, length,  elem_size, free_old ) \
internal_array_reverse_memory(array, (length * elem_size), elem_size, free_old)


#define ARRAY_REVERSE_GENETICC_3(array, length, free_old)		INTERNAL_ARRAY_REVERSE(array, length, sizeof(array[0]), free_old)
#define ARRAY_REVERSE_GENETICC_2(array, length)					INTERNAL_ARRAY_REVERSE(array, length, sizeof(array[0]), false)
#define ARRAY_REVERSE_GENETICC_1(array)							INTERNAL_ARRAY_REVERSE(array, (sizeof(array) / sizeof(array[0])), sizeof(array[0]), false)
#define ARRAY_REVERSE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_3(__VA_ARGS__,						\
ARRAY_REVERSE_GENETICC_3,						\
ARRAY_REVERSE_GENETICC_2,						\
ARRAY_REVERSE_GENETICC_1,)



/*
*	/param	array		Pointer to the start of the array.
*	/param	value		Assigns the given value to each element of the specified array.
*	/param	length		The count of elements in the array, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			If the array fill the value.
*/
#warning reminder to add optimization levels for all macros.
#define REPEATER_MACRO_GENETICC_ARRAY_FILL_MEMORY(array, value, length, elem_size) internal_array_fill_memory(array, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size)

#if GENETICC_OPTIMIZATION_LEVEL <= GENETICC_OPTIMIZATION_LEVEL_SPEED_1 || GENETICC_ALWAYS_INCLUDE_TYPE_SPECIFIC == true
#define INTERNAL_ARRAY_FILL(array, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ARRAY_FILL_MEMORY(array, value, length, elem_size))									\
default:				internal_array_fill_generic(array, (generic_union_t)value, (length * elem_size), elem_size))
#else
#define INTERNAL_ARRAY_FILL(array, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT(REPEATER_MACRO_GENETICC_ARRAY_FILL_MEMORY(array, value, length, elem_size))							\
default:				internal_array_fill_generic(array, (generic_union_t)value, (length * elem_size), elem_size))
#endif

#define ARRAY_FILL_4(array, value, length, start)	INTERNAL_ARRAY_FILL(array + start, value, length, sizeof(array[0]))
#define ARRAY_FILL_3(array, value, length)			INTERNAL_ARRAY_FILL(array, value, length, sizeof(array[0]))
#define ARRAY_FILL_2(array, value)					INTERNAL_ARRAY_FILL(array, value, (sizeof(array) / sizeof(array[0]) ), sizeof(array[0]))
#define ARRAY_FILL_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,				\
ARRAY_FILL_4,							\
ARRAY_FILL_3,							\
ARRAY_FILL_2, )


/*
*	Macro used to select functions by type.
*	Can be used with arrays or pointers.
*
*	/param	array			Pointer to the start of the array.
*	/param	length			The count of elements
*	/param	oldType			The type of values the original array stores.
*	/param	newType			The new type to convert to.
*	/param	free_old		If true, the array passed to "array" will be freed after the copy.
*
*	/returns	uint8_t*	If the array contains the value.
*/
#define INTERNAL_ARRAY_CONVERTTO(array, length,  oldType, newType, free_old ) \
internal_array_convertTo(array, (length * geneticc_get_sized_type_abs(oldType) ), oldType, newType, free_old)


#define ARRAY_CONVERTTO_GENETICC_5(array, length, oldType, newType, free_old)			INTERNAL_ARRAY_CONVERTTO(array, length, oldType, newType, free_old)
#define ARRAY_CONVERTTO_GENETICC_4(array, length, oldType, newType)						INTERNAL_ARRAY_CONVERTTO(array, length, oldType, newType, false)
#define ARRAY_CONVERTTO_GENETICC_3(array, oldType, newType)								INTERNAL_ARRAY_CONVERTTO(array, (sizeof(array) / geneticc_get_sized_type_abs(oldType)), oldType, newType, false)
#define ARRAY_CONVERTTO_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,						\
ARRAY_CONVERTTO_GENETICC_5,						\
ARRAY_CONVERTTO_GENETICC_4,						\
ARRAY_CONVERTTO_GENETICC_3)

#pragma endregion INTERNAL Unsafe Macros





#endif /* GENETICC_ARRAY_INTERNAL_MACROS_H_ */