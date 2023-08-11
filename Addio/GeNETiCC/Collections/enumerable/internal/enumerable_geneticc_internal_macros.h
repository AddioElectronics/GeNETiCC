#ifndef ENUMERABLE_GENETICC_INTERNAL_MACROS_H_
#define ENUMERABLE_GENETICC_INTERNAL_MACROS_H_

#include "enumerable_geneticc_internal_functions.h"
//#include "Addio/GeNETiCC/config/enumerable_geneticc_config.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

#include "Addio/GeNETiCC/geneticc.h"


#pragma region INTERNAL Macros - INTERNAL USE


/*
*	Macro used to select functions by type.
*	*More info in enumerable_geneticc.c
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	value		The value to search for.
*	/param	length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			If the enumerable contains the value.
*/
#warning reminder to add optimization levels for all macros.
#define REPEATER_MACRO_GENETICC_ENUMERABLE_CONTAINS_MEMORY(enumerable, value, length, elem_size) internal_enumerable_contains_memory(enumerable, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size)

#if GENETICC_OPTIMIZATION_LEVEL <= GENETICC_OPTIMIZATION_LEVEL_SPEED_1 || GENETICC_ALWAYS_INCLUDE_TYPE_SPECIFIC == true
#define INTERNAL_ENUMERABLE_CONTAINS(enumerable, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ENUMERABLE_CONTAINS_MEMORY(enumerable, value, length, elem_size))										\
default:				internal_enumerable_contains_generic(enumerable, (generic_union_t)value, (length * elem_size), elem_size))
#else
#define INTERNAL_ENUMERABLE_CONTAINS(enumerable, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT(REPEATER_MACRO_GENETICC_ENUMERABLE_CONTAINS_MEMORY(enumerable, value, length, elem_size))							\
float :					internal_call_enumerable_contains_special(enumerable, (generic_union_t)value, length, TYPE_FLOAT),								\
double :				internal_call_enumerable_contains_special(enumerable, (generic_union_t)value, length, TYPE_DOUBLE),								\
default:				internal_enumerable_contains_generic(enumerable, (generic_union_t)value, (length * elem_size), elem_size))
#endif

#define ENUMERABLE_CONTAINS_4(enumerable, value, length, start)	INTERNAL_ENUMERABLE_CONTAINS((enumerable + start), value, length, sizeof(enumerable[0]))
#define ENUMERABLE_CONTAINS_3(enumerable, value, length)			INTERNAL_ENUMERABLE_CONTAINS(enumerable, value, length, sizeof(enumerable[0]))
#define ENUMERABLE_CONTAINS_2(enumerable, value)					INTERNAL_ENUMERABLE_CONTAINS(enumerable, value, (sizeof(enumerable) / sizeof(enumerable[0]) ), sizeof(enumerable[0]))
#define ENUMERABLE_CONTAINS_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,					\
ENUMERABLE_CONTAINS_4,							\
ENUMERABLE_CONTAINS_3,							\
ENUMERABLE_CONTAINS_2, )

/*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	length		The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	predicate	A function which checks each value to its conditions.
*
*	/returns			True if an element in the enumerable satisfied the predicate's condition, false if the predicate was unsatisfied.
*/
#warning reminder to add optimization levels for all macros.
#define INTERNAL_ENUMERABLE_EXISTS(enumerable, size, elem_size, predicate) \
internal_enumerable_exists(enumerable, size, elem_size, predicate)

#define ENUMERABLE_EXISTS_4(enumerable, predicate, length, start)		INTERNAL_ENUMERABLE_EXISTS((enumerable + start), length, sizeof(enumerable[0]), predicate)
#define ENUMERABLE_EXISTS_3(enumerable, predicate, length)			INTERNAL_ENUMERABLE_EXISTS(enumerable, length, sizeof(enumerable[0]), predicate)
#define ENUMERABLE_EXISTS_2(enumerable, predicate)					INTERNAL_ENUMERABLE_EXISTS(enumerable, (sizeof(enumerable) / sizeof(enumerable[0]) ), sizeof(enumerable[0]), predicate)
#define ENUMERABLE_EXISTS_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,					\
ENUMERABLE_EXISTS_4,							\
ENUMERABLE_EXISTS_3,							\
ENUMERABLE_EXISTS_2, )

/*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	value		The value to search for.
*	/param	length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the value in the enumerable, or -1 if it does not exist.
*/
#define REPEATER_MACRO_GENETICC_ENUMERABLE_INDEXOF_MEMORY(enumerable, value, length, elem_size) internal_enumerable_indexOf_memory(enumerable, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size)

#if GENETICC_OPTIMIZATION_LEVEL <= GENETICC_OPTIMIZATION_LEVEL_SPEED_1 || GENETICC_ALWAYS_INCLUDE_TYPE_SPECIFIC == true
#define INTERNAL_ENUMERABLE_INDEXOF(enumerable, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ENUMERABLE_INDEXOF_MEMORY(enumerable, value, length, elem_size))										\
default:				internal_enumerable_indexOf_generic(enumerable, (generic_union_t)value, (length * elem_size), elem_size))
#else
#define INTERNAL_ENUMERABLE_INDEXOF(enumerable, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT(REPEATER_MACRO_GENETICC_ENUMERABLE_INDEXOF_MEMORY(enumerable, value, length, elem_size))								\
float :					internal_call_enumerable_indexOf_special(enumerable, (generic_union_t)value, length, TYPE_FLOAT),									\
double :				internal_call_enumerable_indexOf_special(enumerable, (generic_union_t)value, length, TYPE_DOUBLE),								\
default:				internal_enumerable_indexOf_generic(enumerable, (generic_union_t)value, (length * elem_size), elem_size))
#endif

#define ENUMERABLE_INDEXOF_4(enumerable, value, length, start)	INTERNAL_ENUMERABLE_GET_TRUE_INDEX(	INTERNAL_ENUMERABLE_INDEXOF((enumerable + start), value, length, sizeof(enumerable[0])), start)
#define ENUMERABLE_INDEXOF_3(enumerable, value, length)											INTERNAL_ENUMERABLE_INDEXOF(enumerable, value, length, sizeof(enumerable[0]))
#define ENUMERABLE_INDEXOF_2(enumerable, value)													INTERNAL_ENUMERABLE_INDEXOF(enumerable, value, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]))
#define ENUMERABLE_INDEXOF_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,					\
ENUMERABLE_INDEXOF_4,							\
ENUMERABLE_INDEXOF_3,							\
ENUMERABLE_INDEXOF_2, )



/*
*	Macro used to select functions by type.
*	*More info in enumerable_geneticc.c
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	value		The value to search for.
*	/param	length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The last index of the value in the enumerable, or -1 if it does not exist.
*/
#define REPEATER_MACRO_GENETICC_ENUMERABLE_LASTINDEXOF_MEMORY(enumerable, value, length, elem_size) internal_enumerable_lastIndexOf_memory(enumerable, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size)

#if GENETICC_OPTIMIZATION_LEVEL <= GENETICC_OPTIMIZATION_LEVEL_SPEED_1 || GENETICC_ALWAYS_INCLUDE_TYPE_SPECIFIC == true
#define INTERNAL_ENUMERABLE_LASTINDEXOF(enumerable, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ENUMERABLE_LASTINDEXOF_MEMORY(enumerable, value, length, elem_size))										\
default:				internal_enumerable_lastIndexOf_generic(enumerable, (generic_union_t)value, (length * elem_size), elem_size))
#else
#define INTERNAL_ENUMERABLE_LASTINDEXOF(enumerable, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT(REPEATER_MACRO_GENETICC_ENUMERABLE_LASTINDEXOF_MEMORY(enumerable, value, length, elem_size))								\
float :					internal_call_enumerable_indexOf_special(enumerable, (generic_union_t)value, length, TYPE_FLOAT),										\
double :				internal_call_enumerable_indexOf_special(enumerable, (generic_union_t)value, length, TYPE_DOUBLE),									\
default:				internal_enumerable_lastIndexOf_generic(enumerable, (generic_union_t)value, (length * elem_size), elem_size))
#endif

#define ENUMERABLE_LASTINDEXOF_4(enumerable, value, length, start)	INTERNAL_ENUMERABLE_GET_TRUE_INDEX(	INTERNAL_ENUMERABLE_LASTINDEXOF(enumerable + start), (value, length, sizeof(enumerable[0])), start)
#define ENUMERABLE_LASTINDEXOF_3(enumerable, value, length)											INTERNAL_ENUMERABLE_LASTINDEXOF(enumerable, value, length, sizeof(enumerable[0]))
#define ENUMERABLE_LASTINDEXOF_2(enumerable, value)													INTERNAL_ENUMERABLE_LASTINDEXOF(enumerable, value, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]))
#define ENUMERABLE_LASTINDEXOF_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,						\
ENUMERABLE_LASTINDEXOF_4,							\
ENUMERABLE_LASTINDEXOF_3,							\
ENUMERABLE_LASTINDEXOF_2, )


\


/*
*	Macro used to select functions by type.
*	*More info in enumerable_geneticc.c
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	value		The value to match and count.
*	/param	length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The amount of times "value" is in the enumerable.
*/
#define REPEATER_MACRO_GENETICC_ENUMERABLE_VALUECOUNT_MEMORY(enumerable, value, length, elem_size) internal_enumerable_valueCount_memory(enumerable, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size)
#define INTERNAL_ENUMERABLE_VALUECOUNT(enumerable, value, length, elem_size) _Generic((value),										\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ENUMERABLE_VALUECOUNT_MEMORY(enumerable, value, length, elem_size))				\
default:				internal_enumerable_valueCount_generic(enumerable, (generic_union_t)value, (length * elem_size), elem_size))

#define ENUMERABLE_VALUECOUNT_4(enumerable, value, length, start)		INTERNAL_ENUMERABLE_VALUECOUNT((enumerable + start), value, length, sizeof(enumerable[0]))
#define ENUMERABLE_VALUECOUNT_3(enumerable, value, length)			INTERNAL_ENUMERABLE_VALUECOUNT(enumerable, value, length, sizeof(enumerable[0]))
#define ENUMERABLE_VALUECOUNT_2(enumerable, value)					INTERNAL_ENUMERABLE_VALUECOUNT(enumerable, value, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]))
#define ENUMERABLE_VALUECOUNT_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,					\
ENUMERABLE_VALUECOUNT_4,							\
ENUMERABLE_VALUECOUNT_3,							\
ENUMERABLE_VALUECOUNT_2, )


/*
*	Macro used to select functions by type.
*	*More info in enumerable_geneticc.c
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the largest value in the enumerable.
*/
#define REPEATER_MACRO_GENETICC_ENUMERABLE_MAX_MEMORY_SIGNED(enumerable, length, elem_size) internal_enumerable_max_memory(enumerable, (length * elem_size),elem_size, true)
#define REPEATER_MACRO_GENETICC_ENUMERABLE_MAX_MEMORY_UNSIGNED(enumerable, length, elem_size) internal_enumerable_max_memory(enumerable, (length * elem_size),elem_size, false)
#define INTERNAL_ENUMERABLE_MAX(enumerable, length, elem_size) _Generic((enumerable),															\
GENETICC_REPEAT_POINTER_TYPES_SIGNED(REPEATER_MACRO_GENETICC_ENUMERABLE_MAX_MEMORY_SIGNED(enumerable, (length * elem_size),elem_size))		\
GENETICC_REPEAT_POINTER_TYPES_UNSIGNED(REPEATER_MACRO_GENETICC_ENUMERABLE_MAX_MEMORY_UNSIGNED(enumerable, (length * elem_size),elem_size))	\
float* :					internal_enumerable_maxf_memory(enumerable, (length * elem_size),elem_size),									\
double* :					internal_enumerable_maxf_memory(enumerable, (length * elem_size),elem_size))

#define ENUMERABLE_MAX_3(enumerable, length, start)	INTERNAL_ENUMERABLE_MAX((enumerable + start), length, sizeof(enumerable[0]), start)
#define ENUMERABLE_MAX_2(enumerable, length)			INTERNAL_ENUMERABLE_MAX(enumerable, length, sizeof(enumerable[0]))
#define ENUMERABLE_MAX_1(enumerable)					INTERNAL_ENUMERABLE_MAX(enumerable, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]))
#define ENUMERABLE_MAX_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,				\
ENUMERABLE_MAX_3,							\
ENUMERABLE_MAX_2,							\
ENUMERABLE_MAX_1, )

#define ENUMERABLE_MAX_VALUE_3(enumerable, length, start)		enumerable[INTERNAL_ENUMERABLE_GET_TRUE_INDEX(	INTERNAL_ENUMERABLE_MAX((enumerable + start), length, sizeof(enumerable[0])), start)]
#define ENUMERABLE_MAX_VALUE_2(enumerable, length)			enumerable[									INTERNAL_ENUMERABLE_MAX(enumerable, length, sizeof(enumerable[0]))]
#define ENUMERABLE_MAX_VALUE_1(enumerable)					enumerable[									INTERNAL_ENUMERABLE_MAX(enumerable, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]))]
#define ENUMERABLE_MAX_VALUE_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,					\
ENUMERABLE_MAX_VALUE_3,							\
ENUMERABLE_MAX_VALUE_2,							\
ENUMERABLE_MAX_VALUE_1, )



/*
*	Macro used to select functions by type.
*	*More info in enumerable_geneticc.c
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the smallest value in the enumerable.
*/
#define REPEATER_MACRO_GENETICC_ENUMERABLE_MIN_MEMORY_SIGNED(enumerable, length, elem_size) internal_enumerable_min_memory(enumerable, (length * elem_size),elem_size, true)
#define REPEATER_MACRO_GENETICC_ENUMERABLE_MIN_MEMORY_UNSIGNED(enumerable, length, elem_size) internal_enumerable_min_memory(enumerable, (length * elem_size),elem_size, false)
#define INTERNAL_ENUMERABLE_MIN(enumerable, length, elem_size) _Generic((enumerable),															\
GENETICC_REPEAT_POINTER_TYPES_SIGNED(REPEATER_MACRO_GENETICC_ENUMERABLE_MIN_MEMORY_SIGNED(enumerable, (length * elem_size),elem_size))		\
GENETICC_REPEAT_POINTER_TYPES_UNSIGNED(REPEATER_MACRO_GENETICC_ENUMERABLE_MIN_MEMORY_UNSIGNED(enumerable, (length * elem_size),elem_size))	\
float* :						internal_enumerable_minf_memory(enumerable, (length * elem_size),elem_size),								\
double* :						internal_enumerable_minf_memory(enumerable, (length * elem_size),elem_size))

#define ENUMERABLE_MIN_3(enumerable, length, start)	INTERNAL_ENUMERABLE_MIN((enumerable + start), length, sizeof(enumerable[0]))
#define ENUMERABLE_MIN_2(enumerable, length)			INTERNAL_ENUMERABLE_MIN(enumerable, length, sizeof(enumerable[0]))
#define ENUMERABLE_MIN_1(enumerable)					INTERNAL_ENUMERABLE_MIN(enumerable, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]))
#define ENUMERABLE_MIN_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,				\
ENUMERABLE_MIN_3,							\
ENUMERABLE_MIN_2,							\
ENUMERABLE_MIN_1, )

#define ENUMERABLE_MIN_VALUE_3(enumerable, length, start)		enumerable[INTERNAL_ENUMERABLE_GET_TRUE_INDEX(	INTERNAL_ENUMERABLE_MIN((enumerable + start), length, sizeof(enumerable[0])), start)]
#define ENUMERABLE_MIN_VALUE_2(enumerable, length)			enumerable[									INTERNAL_ENUMERABLE_MIN(enumerable, length, sizeof(enumerable[0]))]
#define ENUMERABLE_MIN_VALUE_1(enumerable)					enumerable[									INTERNAL_ENUMERABLE_MIN(enumerable, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]))]
#define ENUMERABLE_MIN_VALUE_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,					\
ENUMERABLE_MIN_VALUE_3,							\
ENUMERABLE_MIN_VALUE_2,							\
ENUMERABLE_MIN_VALUE_1, )


/*
*	Macro used to select functions by type.
*	*More info in enumerable_geneticc.c
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the smallest value in the enumerable.
*/
#define REPEATER_MACRO_GENETICC_ENUMERABLE_SUM_MEMORY(enumerable, length, elem_size) internal_enumerable_sum_memory(enumerable, (length * elem_size),elem_size, false)


#define INTERNAL_ENUMERABLE_SUM(enumerable, length, elem_size) _Generic((enumerable),														\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT_UNION(REPEATER_MACRO_GENETICC_ENUMERABLE_SUM_MEMORY(enumerable, length, elem_size))				\
float* :						internal_enumerable_sumf_memory(enumerable, (length * elem_size),elem_size),							\
double* :						internal_enumerable_sumf_memory(enumerable, (length * elem_size),elem_size))

#define ENUMERABLE_SUM_3(enumerable, length, start)	INTERNAL_ENUMERABLE_SUM((enumerable + start), length, sizeof(enumerable[0]))
#define ENUMERABLE_SUM_2(enumerable, length)			INTERNAL_ENUMERABLE_SUM(enumerable, length, sizeof(enumerable[0]))
#define ENUMERABLE_SUM_1(enumerable)					INTERNAL_ENUMERABLE_SUM(enumerable, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]))
#define ENUMERABLE_SUM_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,				\
ENUMERABLE_SUM_3,							\
ENUMERABLE_SUM_2,							\
ENUMERABLE_SUM_1, )



/*
*	Macro used to select functions by type.
*	*More info in enumerable_geneticc.c
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the smallest value in the enumerable.
*/
#define REPEATER_MACRO_GENETICC_ENUMERABLE_AVERAGE_MEMORY(enumerable, length, elem_size) internal_enumerable_average_memory(enumerable, (length * elem_size),elem_size, false)

#define INTERNAL_ENUMERABLE_AVERAGE(enumerable, length, elem_size) _Generic((enumerable),													\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT_UNION(REPEATER_MACRO_GENETICC_ENUMERABLE_AVERAGE_MEMORY(enumerable, length, elem_size))	\
float* :						internal_enumerable_averagef_memory(enumerable, (length * elem_size),elem_size),						\
double* :						internal_enumerable_averagef_memory(enumerable, (length * elem_size),elem_size))

#define ENUMERABLE_AVERAGE_3(enumerable, length, start)	INTERNAL_ENUMERABLE_AVERAGE((enumerable + start), length, sizeof(enumerable[0]))// / length
#define ENUMERABLE_AVERAGE_2(enumerable, length)			INTERNAL_ENUMERABLE_AVERAGE(enumerable, length, sizeof(enumerable[0])) /// length
#define ENUMERABLE_AVERAGE_1(enumerable)					INTERNAL_ENUMERABLE_AVERAGE(enumerable, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]))// / (sizeof(enumerable) / sizeof(enumerable[0]))
#define ENUMERABLE_AVERAGE_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,					\
ENUMERABLE_AVERAGE_3,							\
ENUMERABLE_AVERAGE_2,							\
ENUMERABLE_AVERAGE_1, )

/*
*	Macro used to select functions by type.
*	*More info in enumerable_geneticc.c
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	predicate	A pointer to a function (ENUMERABLE_PREDICATE) which checks each value to its conditions.
*
*	/returns	uint8_t*	A pointer to the first value in the enumerable that matches the predicate
*/
#define INTERNAL_ENUMERABLE_SELECT(enumerable, length, elem_size, predicate)	\
internal_enumerable_select_memory(enumerable, (length * elem_size), elem_size, predicate)

#define ENUMERABLE_FINDINDEX_4(enumerable, predicate, length, start)		INTERNAL_ENUMERABLE_GET_TRUE_INDEX(INTERNAL_ENUMERABLE_SELECT((enumerable + start), length, sizeof(enumerable[0]), predicate), start)
#define ENUMERABLE_FINDINDEX_3(enumerable, predicate, length)			INTERNAL_ENUMERABLE_SELECT(enumerable, length, sizeof(enumerable[0]), predicate)
#define ENUMERABLE_FINDINDEX_2(enumerable, predicate)					INTERNAL_ENUMERABLE_SELECT(enumerable, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), predicate)
#define ENUMERABLE_FINDINDEX_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,				\
ENUMERABLE_FINDINDEX_4,							\
ENUMERABLE_FINDINDEX_3,							\
ENUMERABLE_FINDINDEX_2, )

/*
*	Select Pointer
*/
#define ENUMERABLE_FIND_4(enumerable, predicate, length, start)	enumerable +	INTERNAL_ENUMERABLE_GET_TRUE_INDEX(	INTERNAL_ENUMERABLE_SELECT((enumerable + start), length, sizeof(enumerable[0]), predicate), start)
#define ENUMERABLE_FIND_3(enumerable, predicate, length)			enumerable +	INTERNAL_ENUMERABLE_SELECT(enumerable, length, sizeof(enumerable[0]), predicate)
#define ENUMERABLE_FIND_2(enumerable, predicate)					enumerable +	INTERNAL_ENUMERABLE_SELECT(enumerable, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), predicate)
#define ENUMERABLE_FIND_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,				\
ENUMERABLE_FIND_4,							\
ENUMERABLE_FIND_3,							\
ENUMERABLE_FIND_2, )

/*
*	Enumerable Select Vargs
*/
#define INTERNAL_ENUMERABLE_FINDINDEX_VARGS(enumerable, predicate, arg_count, ap, length, elem_size)	\
internal_enumerable_select_vargs_memory(enumerable, (length * elem_size), elem_size, predicate, arg_count, ap)


#define ENUMERABLE_FINDINDEX_VARGS_6(enumerable, predicate, arg_count, ap, length, start)				INTERNAL_ENUMERABLE_GET_TRUE_INDEX(	INTERNAL_ENUMERABLE_FINDINDEX_VARGS((enumerable + start), predicate, arg_count, ap, length, sizeof(enumerable[0])), start)
#define ENUMERABLE_FINDINDEX_VARGS_5(enumerable, predicate, arg_count, ap, length)														INTERNAL_ENUMERABLE_FINDINDEX_VARGS(enumerable, predicate, arg_count, ap, length, sizeof(enumerable[0]))
#define ENUMERABLE_FINDINDEX_VARGS_4(enumerable, predicate, arg_count, ap)																INTERNAL_ENUMERABLE_FINDINDEX_VARGS(enumerable, predicate, arg_count, ap, sizeof(enumerable) / sizeof(enumerable[0]), sizeof(enumerable[0]))
#define ENUMERABLE_FINDINDEX_VARGS_MACRO_CHOOSER(...)	\
GET_ARGUMENT_6(__VA_ARGS__,						\
ENUMERABLE_FINDINDEX_VARGS_6,							\
ENUMERABLE_FINDINDEX_VARGS_5,							\
ENUMERABLE_FINDINDEX_VARGS_4, )

/*
*	Select Pointer Vargs
*/
#define ENUMERABLE_FIND_VARGS_6(enumerable, predicate, arg_count, ap, length, start)					enumerable +	INTERNAL_ENUMERABLE_GET_TRUE_INDEX(	INTERNAL_ENUMERABLE_FINDINDEX_VARGS((enumerable + start), predicate, arg_count, ap, length,  sizeof(enumerable[0])), start)
#define ENUMERABLE_FIND_VARGS_5(enumerable, predicate, arg_count, ap, length)						enumerable +									INTERNAL_ENUMERABLE_FINDINDEX_VARGS(enumerable, predicate, arg_count, ap, length, sizeof(enumerable[0]))
#define ENUMERABLE_FIND_VARGS_4(enumerable, predicate, arg_count, ap)																		INTERNAL_ENUMERABLE_FINDINDEX_VARGS(enumerable, predicate, arg_count, ap, sizeof(enumerable) / sizeof(enumerable[0]), sizeof(enumerable[0]))
#define ENUMERABLE_FIND_VARGS_MACRO_CHOOSER(...)	\				
GET_ARGUMENT_6(__VA_ARGS__,								\
ENUMERABLE_FIND_VARGS_6,							\
ENUMERABLE_FIND_VARGS_5,							\
ENUMERABLE_FIND_VARGS_4, )



#pragma endregion INTERNAL Macros

/*
*	unsafe macros
*
*	Note :	If you use any of these functions more than once on the same enumerable, its recommended that you use "list_geneticc.h" instead.
*			You can get a big performance improvement depending on which function is used, in trade for more memory being used (more memory but also less fragmentation).
*
*/
#pragma region INTERNAL Unsafe Macros - INTERNAL USE

/*
*	Macro used to select functions by type.
*	*More info in enumerable_geneticc.c
*
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	predicate	A pointer to a function which checks each value to its conditions.
*	/param	out_count	The amount of matches in the enumerable.
*
*	/returns	uint8t**	A pointer to the first value in the enumerable that matches the predicate
*/
#define INTERNAL_ENUMERABLE_FINDALL(enumerable, length, elem_size, predicate, out_length)	\
internal_enumerable_selectMany_memory(enumerable, (length * elem_size), elem_size, predicate, out_length)

/*
*	Macros which implement the overloading
*/
#define ENUMERABLE_FINDALL_5(enumerable, predicate, length, out_length, start)		INTERNAL_ENUMERABLE_FINDALL((enumerable + start), length, sizeof(enumerable[0]), predicate, out_length)
#define ENUMERABLE_FINDALL_4(enumerable, predicate, length, out_length)			INTERNAL_ENUMERABLE_FINDALL(enumerable, length, sizeof(enumerable[0]), predicate, out_length)
#define ENUMERABLE_FINDALL_3(enumerable, predicate, out_length)					INTERNAL_ENUMERABLE_FINDALL(enumerable, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), predicate, out_length)
#define ENUMERABLE_FINDALL_MACRO_CHOOSER(...)	\
GET_ARGUMENT_5(__VA_ARGS__,					\
ENUMERABLE_FINDALL_5,							\
ENUMERABLE_FINDALL_4,							\
ENUMERABLE_FINDALL_3, )

/*
*	Macro used to select functions by type.
*	*More info in enumerable_geneticc.c
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	range		The range of values to insert.
*	/param	index		The index where the new values should be inserted at.
*	/param	count		The amount of values to insert (eg. 4 integers = 4, not sizeof(int) * 4)
*	/param	length		The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	free_old		If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns			A pointer to the start of the new enumerable.
*/
#define INTERNAL_ENUMERABLE_INSERTRANGE(enumerable, range, index, count, length, elem_size, free_old ) \
internal_enumerable_insertRange_memory(enumerable, (generic_union_t){.u64 = (uint32_t)range}.cp, (index == 0 ? 0 : (index * elem_size)),  (count * elem_size), (length * elem_size), elem_size, free_old)

/*
*	Macros which implement the overloading
*/
#define ENUMERABLE_INSERTRANGE_GENETICC_6(enumerable, range, index,  count, length, free_old)			INTERNAL_ENUMERABLE_INSERTRANGE(enumerable, range, index, count, length, sizeof(enumerable[0]), free_old)
#define ENUMERABLE_INSERTRANGE_GENETICC_5(enumerable, range, index,  count, length)					INTERNAL_ENUMERABLE_INSERTRANGE(enumerable, range, index, count, length, count,sizeof(enumerable[0]), false)
#define ENUMERABLE_INSERTRANGE_GENETICC_4(enumerable, range, index,  count)							INTERNAL_ENUMERABLE_INSERTRANGE(enumerable, range, index, count, (sizeof(enumerable) / sizeof(enumerable[0])),  sizeof(enumerable[0]), false)
#define ENUMERABLE_INSERTRANGE_GENETICC_3(enumerable, range, index)									INTERNAL_ENUMERABLE_INSERTRANGE(enumerable, range, index,(sizeof(range) * sizeof(range[0])), (sizeof(enumerable) / sizeof(enumerable[0])),  sizeof(enumerable[0]), false)
#define ENUMERABLE_INSERTRANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_6(__VA_ARGS__,							\
ENUMERABLE_INSERTRANGE_GENETICC_6,						\
ENUMERABLE_INSERTRANGE_GENETICC_5,						\
ENUMERABLE_INSERTRANGE_GENETICC_4,						\
ENUMERABLE_INSERTRANGE_GENETICC_3,)


/*
*	Macro used to select functions by type.
*	Can be used with enumerables or pointers.
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	value		The value to search for.
*	/param	index		The index where the new value should be inserted at.
*	/param	length		The element count.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	free_old		If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns			If the enumerable contains the value.
*/
#define REPEATER_MACRO_GENETICC_ENUMERABLE_INSERT_MEMORY(enumerable, value, index,  length, elem_size, free_old) internal_enumerable_insert_memory(enumerable, (generic_union_t){.u32 = (uint32_t)value}.cp,  (index == 0 ? 0 : (index * elem_size)),  (length * elem_size), elem_size, free_old)
#define INTERNAL_ENUMERABLE_INSERT(enumerable, value, index,  length, elem_size, free_old ) _Generic((value),																			\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ENUMERABLE_INSERT_MEMORY(enumerable, value, index,  length, elem_size, free_old))													\
default:				internal_enumerable_insert_generic(enumerable, (generic_union_t)value, (index == 0 ? 0 : (index * elem_size)),  (length * elem_size),  elem_size, free_old))




/*
*	Macros which implement the overloading
*/
#define ENUMERABLE_INSERT_GENETICC_5(enumerable, value, index, length, free_old)			INTERNAL_ENUMERABLE_INSERT(enumerable, value, index,  length, sizeof(enumerable[0]), free_old)
#define ENUMERABLE_INSERT_GENETICC_4(enumerable, value, index, length)						INTERNAL_ENUMERABLE_INSERT(enumerable, value, index,  length, sizeof(enumerable[0]), false)
#define ENUMERABLE_INSERT_GENETICC_3(enumerable, value, index)								INTERNAL_ENUMERABLE_INSERT(enumerable, value, index, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), false)
#define ENUMERABLE_INSERT_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,					\
ENUMERABLE_INSERT_GENETICC_5,						\
ENUMERABLE_INSERT_GENETICC_4,						\
ENUMERABLE_INSERT_GENETICC_3, )



/*
*	Macro used to select functions by type.
*	*More info in enumerable_geneticc.c
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	range		The range of values to add.
*	/param	index		The index where the new values should be added at.
*	/param	count		The amount of values to add (eg. 4 integers = 4, not sizeof(int) * 4)
*	/param	length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	free_old		If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns			A pointer to the start of the new enumerable.
*/
#define INTERNAL_ENUMERABLE_ADDRANGE(enumerable, range, length, count, elem_size, free_old ) \
internal_enumerable_insertRange_memory(enumerable, (generic_union_t){.u64 = (uint32_t)range}.cp, length, (count * elem_size), (length * elem_size), elem_size, free_old)
//enumerable_addRange_memory((generic_union_t){.u32 = (uint32_t)value}.cp,  enumerable, count, (length * elem_size),  elem_size, free_old)

/*
*	Macros which implement the overloading
*/
#define ENUMERABLE_ADDRANGE_GENETICC_5(enumerable, range, length, count, free_old)			INTERNAL_ENUMERABLE_ADDRANGE(enumerable, range, count, length, sizeof(enumerable[0]), free_old)
#define ENUMERABLE_ADDRANGE_GENETICC_4(enumerable, range, length, count)					INTERNAL_ENUMERABLE_ADDRANGE(enumerable, range, count, length, sizeof(enumerable[0]), false)
#define ENUMERABLE_ADDRANGE_GENETICC_3(enumerable, range, count)							INTERNAL_ENUMERABLE_ADDRANGE(enumerable, range, count, (sizeof(range) * sizeof(range[0])), sizeof(enumerable[0]), false)
#define ENUMERABLE_ADDRANGE_GENETICC_2(enumerable, range)									INTERNAL_ENUMERABLE_ADDRANGE(enumerable, range, (sizeof(enumerable) / sizeof(enumerable[0])), (sizeof(range) * sizeof(range[0])),sizeof(enumerable[0]), false)
#define ENUMERABLE_ADDRANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,						\
ENUMERABLE_ADDRANGE_GENETICC_5,						\
ENUMERABLE_ADDRANGE_GENETICC_4,						\
ENUMERABLE_ADDRANGE_GENETICC_3,						\
ENUMERABLE_ADDRANGE_GENETICC_2,)



/*
*	Macro used to select functions by type.
*	Can be used with enumerables or pointers.
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	value		The value to search for.
*	/param	length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	free_old	If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns			If the enumerable contains the value.
*/
#define REPEATER_MACRO_GENETICC_ENUMERABLE_ADD_MEMORY(enumerable, value, length, elem_size, free_old ) internal_enumerable_add_memory(enumerable, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size, free_old)
#define INTERNAL_ENUMERABLE_ADD(enumerable, value, length, elem_size, free_old ) _Generic((value),											\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ENUMERABLE_ADD_MEMORY(enumerable, value, length, elem_size, free_old ))					\
default:				internal_enumerable_add_generic(enumerable, (generic_union_t)value, (length * elem_size), elem_size, free_old))



/*
*	Macros which implement the overloading
*/
#define ENUMERABLE_ADD_GENETICC_4(enumerable, value, length, free_old)			INTERNAL_ENUMERABLE_ADD(enumerable, value, length, sizeof(enumerable[0]), free_old)
#define ENUMERABLE_ADD_GENETICC_3(enumerable, value, length)					INTERNAL_ENUMERABLE_ADD(enumerable, value, length, sizeof(enumerable[0]), false)
#define ENUMERABLE_ADD_GENETICC_2(enumerable, value)							INTERNAL_ENUMERABLE_ADD(enumerable, value, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), false)
#define ENUMERABLE_ADD_MACRO_CHOOSER(...)		\
GET_ARGUMENT_4(__VA_ARGS__,					\
ENUMERABLE_ADD_GENETICC_4,						\
ENUMERABLE_ADD_GENETICC_3,						\
ENUMERABLE_ADD_GENETICC_2, )




/*
*	Macro used to select functions by type.
*	*More info in enumerable_geneticc.c
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	range		The range of values to prepend.
*	/param	index		The index where the new values should be prepended at.
*	/param	count		The amount of values to prepend (eg. 4 integers = 4, not sizeof(int) * 4)
*	/param  length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	free_old		If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns			A pointer to the start of the new enumerable.
*/
#define INTERNAL_ENUMERABLE_PREPENDRANGE(enumerable, range,  count, length,  elem_size, free_old ) \
internal_enumerable_insertRange_memory(enumerable,  (generic_union_t){.u64 = (uint32_t)range}.cp, 0,  (count * elem_size), (length * elem_size), elem_size, free_old)
//enumerable_prependRange_memory((generic_union_t){.u32 = (uint32_t)value}.cp,  enumerable, (count * elem_size), (length * elem_size), elem_size, free_old)

/*
*	Prepends the ability to call one macro with different amounts of parameters
*/
#define ENUMERABLE_PREPENDRANGE_GENETICC_5(enumerable, range, count, length, free_old)			INTERNAL_ENUMERABLE_PREPENDRANGE(enumerable, range, count,  length, sizeof(enumerable[0]), free_old)
#define ENUMERABLE_PREPENDRANGE_GENETICC_4(enumerable, range, count, length)					INTERNAL_ENUMERABLE_PREPENDRANGE(enumerable, range, count,  length, sizeof(enumerable[0]), false)
#define ENUMERABLE_PREPENDRANGE_GENETICC_3(enumerable, range, count)							INTERNAL_ENUMERABLE_PREPENDRANGE(enumerable, range, count, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), false)
#define ENUMERABLE_PREPENDRANGE_GENETICC_2(enumerable, range)									INTERNAL_ENUMERABLE_PREPENDRANGE(enumerable, range, (sizeof(range) * sizeof(range[0])), (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), false)
#define ENUMERABLE_PREPENDRANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,							\
ENUMERABLE_PREPENDRANGE_GENETICC_5,						\
ENUMERABLE_PREPENDRANGE_GENETICC_4,						\
ENUMERABLE_PREPENDRANGE_GENETICC_3,						\
ENUMERABLE_PREPENDRANGE_GENETICC_2,)




/*
*	Macro used to select functions by type.
*	Can be used with enumerables or pointers.
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	value		The value to search for.
*	/param  length  The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*	/param	free_old		If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns			If the enumerable contains the value.
*/
#define REPEATER_MACRO_GENETICC_ENUMERABLE_PREPEND_MEMORY(enumerable, value, length, elem_size, free_old ) internal_enumerable_prepend_memory(enumerable, (generic_union_t){.u32 = (uint32_t)value}.cp,  (length * elem_size), elem_size, free_old)
#define INTERNAL_ENUMERABLE_PREPEND(enumerable, value, length, elem_size, free_old ) _Generic((value),											\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ENUMERABLE_PREPEND_MEMORY(enumerable, value, length, elem_size, free_old ))					\
default:				internal_enumerable_prepend_generic(enumerable, (generic_union_t)value, (length * elem_size), elem_size, free_old))



/*
*	Prepends the ability to call one macro with different amounts of parameters
*/
#define ENUMERABLE_PREPEND_GENETICC_4(enumerable, value, length, free_old)			INTERNAL_ENUMERABLE_PREPEND(enumerable, value, length, sizeof(enumerable[0]), free_old)
#define ENUMERABLE_PREPEND_GENETICC_3(enumerable, value, length)					INTERNAL_ENUMERABLE_PREPEND(enumerable, value, length, sizeof(enumerable[0]), false)
#define ENUMERABLE_PREPEND_GENETICC_2(enumerable, value)							INTERNAL_ENUMERABLE_PREPEND(enumerable, value, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), false)
#define ENUMERABLE_PREPEND_MACRO_CHOOSER(...)		\
GET_ARGUMENT_4(__VA_ARGS__,						\
ENUMERABLE_PREPEND_GENETICC_4,						\
ENUMERABLE_PREPEND_GENETICC_3,						\
ENUMERABLE_PREPEND_GENETICC_2, )


/*
*	Macro used to select functions by type.
*	Can be used with enumerables or pointers.
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	index			The index of the element to start removing at.
*	/param	count			The amount of elements to remove
*	/param	length			The count of elements
*	/param	elem_size		The size of value's type (in bytes).
*	/param	free_old		If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns	uint8_t*	If the enumerable contains the value.
*/
#define INTERNAL_ENUMERABLE_REMOVERANGE(enumerable, index, count, length,  elem_size, free_old ) \
internal_enumerable_removeRange_memory(enumerable, (index * elem_size), (count * elem_size), (length * elem_size), elem_size, free_old)



/*
*	Macros which implement the overloading
*/
#define ENUMERABLE_REMOVERANGE_GENETICC_5(enumerable, index, count, length, free_old)			INTERNAL_ENUMERABLE_REMOVERANGE(enumerable, index, count, length, sizeof(enumerable[0]), free_old)
#define ENUMERABLE_REMOVERANGE_GENETICC_4(enumerable, index, count, length)					INTERNAL_ENUMERABLE_REMOVERANGE(enumerable, index, count, length, sizeof(enumerable[0]), false)
#define ENUMERABLE_REMOVERANGE_GENETICC_3(enumerable, index, count)							INTERNAL_ENUMERABLE_REMOVERANGE(enumerable, index, count,  (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), false)
#define ENUMERABLE_REMOVERANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,							\
ENUMERABLE_REMOVERANGE_GENETICC_5,						\
ENUMERABLE_REMOVERANGE_GENETICC_4,						\
ENUMERABLE_REMOVERANGE_GENETICC_3,)





/*
*	Macro used to select functions by type.
*	Can be used with enumerables or pointers.
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	index			The index of the element to remove.
*	/param	length			The count of elements
*	/param	elem_size		The size of value's type (in bytes).
*	/param	free_old		If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns	uint8_t*	If the enumerable contains the value.
*/
#define INTERNAL_ENUMERABLE_REMOVEAT(enumerable, index, length,  elem_size, free_old ) \
internal_enumerable_removeAt_memory(enumerable, (index * elem_size), (length * elem_size), elem_size, free_old)



/*
*	Macros which implement the overloading
*/
#define ENUMERABLE_REMOVEAT_GENETICC_4(enumerable, index, length, free_old)			INTERNAL_ENUMERABLE_REMOVEAT(enumerable, index, length, sizeof(enumerable[0]), free_old)
#define ENUMERABLE_REMOVEAT_GENETICC_3(enumerable, index, length)						INTERNAL_ENUMERABLE_REMOVEAT(enumerable, index, length, sizeof(enumerable[0]), false)
#define ENUMERABLE_REMOVEAT_GENETICC_2(enumerable, index)								INTERNAL_ENUMERABLE_REMOVEAT(enumerable, index, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), false)
#define ENUMERABLE_REMOVEAT_MACRO_CHOOSER(...)		\
GET_ARGUMENT_4(__VA_ARGS__,						\
ENUMERABLE_REMOVEAT_GENETICC_4,						\
ENUMERABLE_REMOVEAT_GENETICC_3,						\
ENUMERABLE_REMOVEAT_GENETICC_2,)


/*
*	Macro used to select functions by type.
*	Can be used with enumerables or pointers.
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	value			The value to remove.
*	/param	length			The count of elements
*	/param	elem_size		The size of value's type (in bytes).
*	/param	free_old		If true, the enumerable passed to "enumerable" will be freed after the copy.
*	/param	out_removed*	(Out)Returns true or false depending if a value was found and removed.
*
*	/returns	uint8_t*	If the enumerable contains the value.
*/
#define REPEATER_MACRO_GENETICC_ENUMERABLE_REMOVE_MEMORY(enumerable, value, length, elem_size, free_old, out_removed  ) internal_enumerable_remove_memory(enumerable, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size, free_old, out_removed)
#define INTERNAL_ENUMERABLE_REMOVE(enumerable, value, length, elem_size, free_old, out_removed )  _Generic((value),											\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ENUMERABLE_REMOVE_MEMORY(enumerable, value, length, elem_size, free_old, out_removed  ))					\
default:				internal_enumerable_remove_generic(enumerable, (generic_union_t)value, (length * elem_size), elem_size, free_old, out_removed))

#define ENUMERABLE_REMOVE_GENETICC_5(enumerable, value, length, free_old, out_removed )		INTERNAL_ENUMERABLE_REMOVE(enumerable, value, length, sizeof(enumerable[0]), free_old, out_removed )
#define ENUMERABLE_REMOVE_GENETICC_4(enumerable, value, length, out_removed)					INTERNAL_ENUMERABLE_REMOVE(enumerable, value, length, sizeof(enumerable[0]), false, out_removed )
#define ENUMERABLE_REMOVE_GENETICC_3(enumerable, value, out_removed)							INTERNAL_ENUMERABLE_REMOVE(enumerable, value, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), false, out_removed )
#define ENUMERABLE_REMOVE_GENETICC_2(enumerable, value)										INTERNAL_ENUMERABLE_REMOVE(enumerable, value, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), false, 0 )
#define ENUMERABLE_REMOVE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,					\
ENUMERABLE_REMOVE_GENETICC_5,						\
ENUMERABLE_REMOVE_GENETICC_4,						\
ENUMERABLE_REMOVE_GENETICC_3,						\
ENUMERABLE_REMOVE_GENETICC_2,)



/*
*	Macro used to select functions by type.
*	Can be used with enumerables or pointers.
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	value			The value to determine which elements to remove.
*	/param	length			The count of elements
*	/param	elem_size		The size of value's type (in bytes).
*	/param	free_old		If true, the enumerable passed to "enumerable" will be freed after the copy.
*	/param	out_count*		(Out)Returns amount of values removed from the enumerable.
*
*	/returns	uint8_t*	If the enumerable contains the value.
*/
#define REPEATER_MACRO_GENETICC_ENUMERABLE_REMOVEALL_MEMORY(enumerable, value, length, elem_size, free_old, out_count  ) internal_enumerable_removeAll_memory(enumerable, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size, free_old, out_count)
#define INTERNAL_ENUMERABLE_REMOVEALL(enumerable, value, length, elem_size, free_old, out_count )  _Generic((value),											\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ENUMERABLE_REMOVEALL_MEMORY(enumerable, value, length, elem_size, free_old, out_count  ))					\
default:				internal_enumerable_removeAll_generic(enumerable, (generic_union_t)value, (length * elem_size), elem_size, free_old, out_count))


#define ENUMERABLE_REMOVEALL_GENETICC_5(enumerable, value, length, free_old, out_count )			INTERNAL_ENUMERABLE_REMOVEALL(enumerable, value, length, sizeof(enumerable[0]), free_old, out_count )
#define ENUMERABLE_REMOVEALL_GENETICC_4(enumerable, value, length, out_count)						INTERNAL_ENUMERABLE_REMOVEALL(enumerable, value, length, sizeof(enumerable[0]), false, out_count )
#define ENUMERABLE_REMOVEALL_GENETICC_3(enumerable, value, out_count)								INTERNAL_ENUMERABLE_REMOVEALL(enumerable, value, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), false, out_count )
#define ENUMERABLE_REMOVEALL_GENETICC_2(enumerable, value)											INTERNAL_ENUMERABLE_REMOVEALL(enumerable, value, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), false, 0 )
#define ENUMERABLE_REMOVEALL_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,						\
ENUMERABLE_REMOVEALL_GENETICC_5,						\
ENUMERABLE_REMOVEALL_GENETICC_4,						\
ENUMERABLE_REMOVEALL_GENETICC_3,						\
ENUMERABLE_REMOVEALL_GENETICC_2,)





/*
*	Macro used to select functions by type.
*	Can be used with enumerables or pointers.
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	length			The count of elements
*	/param	elem_size		The size of value's type (in bytes).
*	/param	free_old		If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns	uint8_t*	If the enumerable contains the value.
*/
#define INTERNAL_ENUMERABLE_REVERSE(enumerable, length,  elem_size, free_old ) \
internal_enumerable_reverse_memory(enumerable, (length * elem_size), elem_size, free_old)


#define ENUMERABLE_REVERSE_GENETICC_3(enumerable, length, free_old)		INTERNAL_ENUMERABLE_REVERSE(enumerable, length, sizeof(enumerable[0]), free_old)
#define ENUMERABLE_REVERSE_GENETICC_2(enumerable, length)					INTERNAL_ENUMERABLE_REVERSE(enumerable, length, sizeof(enumerable[0]), false)
#define ENUMERABLE_REVERSE_GENETICC_1(enumerable)							INTERNAL_ENUMERABLE_REVERSE(enumerable, (sizeof(enumerable) / sizeof(enumerable[0])), sizeof(enumerable[0]), false)
#define ENUMERABLE_REVERSE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_3(__VA_ARGS__,						\
ENUMERABLE_REVERSE_GENETICC_3,						\
ENUMERABLE_REVERSE_GENETICC_2,						\
ENUMERABLE_REVERSE_GENETICC_1,)



/*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	value		Assigns the given value to each element of the specified enumerable.
*	/param	length		The count of elements in the enumerable, or how many elements are affected.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			If the enumerable fill the value.
*/
#warning reminder to add optimization levels for all macros.
#define REPEATER_MACRO_GENETICC_ENUMERABLE_FILL_MEMORY(enumerable, value, length, elem_size) internal_enumerable_fill_memory(enumerable, (generic_union_t){.u32 = (uint32_t)value}.cp, (length * elem_size), elem_size)

#if GENETICC_OPTIMIZATION_LEVEL <= GENETICC_OPTIMIZATION_LEVEL_SPEED_1 || GENETICC_ALWAYS_INCLUDE_TYPE_SPECIFIC == true
#define INTERNAL_ENUMERABLE_FILL(enumerable, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_ENUMERABLE_FILL_MEMORY(enumerable, value, length, elem_size))									\
default:				internal_enumerable_fill_generic(enumerable, (generic_union_t)value, (length * elem_size), elem_size))
#else
#define INTERNAL_ENUMERABLE_FILL(enumerable, value, length, elem_size) _Generic((value),																\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT(REPEATER_MACRO_GENETICC_ENUMERABLE_FILL_MEMORY(enumerable, value, length, elem_size))							\
default:				internal_enumerable_fill_generic(enumerable, (generic_union_t)value, (length * elem_size), elem_size))
#endif

#define ENUMERABLE_FILL_4(enumerable, value, length, start)	INTERNAL_ENUMERABLE_FILL(enumerable + start, value, length, sizeof(enumerable[0]))
#define ENUMERABLE_FILL_3(enumerable, value, length)			INTERNAL_ENUMERABLE_FILL(enumerable, value, length, sizeof(enumerable[0]))
#define ENUMERABLE_FILL_2(enumerable, value)					INTERNAL_ENUMERABLE_FILL(enumerable, value, (sizeof(enumerable) / sizeof(enumerable[0]) ), sizeof(enumerable[0]))
#define ENUMERABLE_FILL_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,				\
ENUMERABLE_FILL_4,							\
ENUMERABLE_FILL_3,							\
ENUMERABLE_FILL_2, )


/*
*	Macro used to select functions by type.
*	Can be used with enumerables or pointers.
*
*	/param	enumerable			Pointer to the start of the enumerable.
*	/param	length			The count of elements
*	/param	oldType			The type of values the original enumerable stores.
*	/param	newType			The new type to convert to.
*	/param	free_old		If true, the enumerable passed to "enumerable" will be freed after the copy.
*
*	/returns	uint8_t*	If the enumerable contains the value.
*/
#define INTERNAL_ENUMERABLE_CONVERTTO(enumerable, length,  oldType, newType, free_old ) \
internal_enumerable_convertTo(enumerable, (length * geneticc_get_sized_type_abs(oldType) ), oldType, newType, free_old)


#define ENUMERABLE_CONVERTTO_GENETICC_5(enumerable, length, oldType, newType, free_old)			INTERNAL_ENUMERABLE_CONVERTTO(enumerable, length, oldType, newType, free_old)
#define ENUMERABLE_CONVERTTO_GENETICC_4(enumerable, length, oldType, newType)						INTERNAL_ENUMERABLE_CONVERTTO(enumerable, length, oldType, newType, false)
#define ENUMERABLE_CONVERTTO_GENETICC_3(enumerable, oldType, newType)								INTERNAL_ENUMERABLE_CONVERTTO(enumerable, (sizeof(enumerable) / geneticc_get_sized_type_abs(oldType)), oldType, newType, false)
#define ENUMERABLE_CONVERTTO_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,						\
ENUMERABLE_CONVERTTO_GENETICC_5,						\
ENUMERABLE_CONVERTTO_GENETICC_4,						\
ENUMERABLE_CONVERTTO_GENETICC_3)

#pragma endregion INTERNAL Unsafe Macros





#endif /* ENUMERABLE_GENETICC_INTERNAL_MACROS_H_ */