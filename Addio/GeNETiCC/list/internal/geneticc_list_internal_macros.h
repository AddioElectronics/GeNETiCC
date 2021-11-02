#ifndef GENETICC_LIST_INTERNAL_MACROS_H_
#define GENETICC_LIST_INTERNAL_MACROS_H_

#include "geneticc_list_internal_functions.h"
//#include "../config/geneticc_list_config.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

#include "../../geneticc.h"

#define LIST_SIZE_TO_COUNT(size, elem_size)		(size / elem_size)
#define LIST_COUNT_TO_SIZE(count, elem_size)	(count * elem_size)

#pragma region Initialization Macros


/*
*	Allocates and Initializes a list
*
*	/param	elem_size		The size of the value's type (in bytes).
*	/param	capacity		The amount of elements that can be stored in the list.
*
*	/Returns list_t	The newly allocated list.
*/
#define LIST_NEW_2(elem_size, capacity)		internal_list_new(elem_size, capacity )
#define LIST_NEW_1(elem_size)				internal_list_new(elem_size, LIST_DEFAULT_CAPACITY)
#define LIST_NEW_MACRO_CHOOSER(...)	\
GET_ARGUMENT_2(__VA_ARGS__,			\
LIST_NEW_2,							\
LIST_NEW_1, )


#define LIST_INIT_3(out_list, elem_size, capacity)		internal_list_init(out_list, elem_size, capacity )
#define LIST_INIT_2(out_list, elem_size)				internal_list_init(out_list, elem_size, LIST_DEFAULT_CAPACITY)
#define LIST_INIT_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,				\
LIST_INIT_3,							\
LIST_INIT_2, )

/*
*	Allocates and Initializes a list from an already existing array.
*
*	/param	array				The initial size of the list. (How many values can be stored)
*	/param	length				Either the length of the array, or the new capacity(only if "populated_length is used).	*Required for pointer arrays.
*	/param	elem_size			The size of 1 element in the array.
*	/param	populated_length	The count of elements currently in the array											*If length == capacity, this is cannot be 0.
*
*	/Returns list_t				The newly allocated list.
*/
#define INTERNAL_ARRAY_TO_LIST(array, length, elem_size, populated_length) \
internal_array_to_list( array, (length * elem_size), elem_size, (populated_length * elem_size));

#define ARRAY_TO_LIST_4(array, length, populated_length, elem_size)		INTERNAL_ARRAY_TO_LIST(array, length, elem_size, populated_length)
#define ARRAY_TO_LIST_3(array, length, populated_length)				INTERNAL_ARRAY_TO_LIST(array, length, sizeof(array[0]), populated_length)
#define ARRAY_TO_LIST_2(array, length)									INTERNAL_ARRAY_TO_LIST(array, length, sizeof(array[0]), 0 )
#define ARRAY_TO_LIST_1(array)											INTERNAL_ARRAY_TO_LIST(array, sizeof(array) / sizeof(array[0]), sizeof(array[0]), 0)
#define ARRAY_TO_LIST_MACRO_CHOOSER(...)			\
GET_ARGUMENT_4(__VA_ARGS__,							\
ARRAY_TO_LIST_4,									\
ARRAY_TO_LIST_3,									\
ARRAY_TO_LIST_2,									\
ARRAY_TO_LIST_1, )

/*
*	Initializes a list by copying an already existing array.
*	Copies the array into a new block of memory. (Use Array_To_list to use the same array)
*
*	/param	array				The initial size of the list. (How many values can be stored)
*	/param	length				Either the length of the array, or the new capacity(only if "populated_length is used).	*Required for pointer arrays.
*	/param	elem_size			The size of 1 element in the array.
*	/param	populated_length	The count of elements currently in the array											*If length == capacity, this is cannot be 0.
*
*	/Returns list_t		The newly allocated list.
*/
#define INTERNAL_ARRAY_COPYTO_LIST(array, length, elem_size, populated_length) \
internal_array_copyTo_list(array, (length * elem_size), elem_size, (populated_length * elem_size));

#define ARRAY_COPYTO_LIST_4(array, length_capacity, populated_length, elem_size)		INTERNAL_ARRAY_COPYTO_LIST(array, length_capacity, elem_size, populated_length)
#define ARRAY_COPYTO_LIST_3(array, length_capacity, populated_length)					INTERNAL_ARRAY_COPYTO_LIST(array, length_capacity, sizeof(array[0]), populated_length)
#define ARRAY_COPYTO_LIST_2(array, length)												INTERNAL_ARRAY_COPYTO_LIST(array, length, sizeof(array[0]), 0 )
#define ARRAY_COPYTO_LIST_1(array)														INTERNAL_ARRAY_COPYTO_LIST(array, sizeof(array) / sizeof(array[0]), sizeof(array[0]), 0)
#define ARRAY_COPYTO_LIST_MACRO_CHOOSER(...)			\
GET_ARGUMENT_4(__VA_ARGS__,								\
ARRAY_COPYTO_LIST_4,									\
ARRAY_COPYTO_LIST_3,									\
ARRAY_COPYTO_LIST_2,									\
ARRAY_COPYTO_LIST_1, )



/*
*	Initializes a list from an already existing array.
*
*	/param	array				The initial size of the list. (How many values can be stored)
*	/param	length				The length the array was created with.	*Required for pointer arrays.
*	/param	*out_list			(Out)The initialized list.
*	/param	elem_size			The size of the value's type (in bytes).
*	/param	populated_length	The count of elements currently in the array.
*
*/
#define INTERNAL_ARRAY_INIT_LIST(array, length, out_list, elem_size, populated_length) \
internal_array_init_list(array, length * elem_size, elem_size, (populated_length * elem_size), out_list);

#define ARRAY_INIT_LIST_5(array, length, populated_length, elem_size, out_list)		INTERNAL_ARRAY_INIT_LIST(array, length, out_list, elem_size, populated_length)
#define ARRAY_INIT_LIST_4(array, length, populated_length, out_list)				INTERNAL_ARRAY_INIT_LIST(array, length, out_list, sizeof(array[0]), populated_length)
#define ARRAY_INIT_LIST_3(array, length, out_list)									INTERNAL_ARRAY_INIT_LIST(array, length, out_list,sizeof(array[0]), 0 )
#define ARRAY_INIT_LIST_2(array, out_list)											INTERNAL_ARRAY_INIT_LIST(array, sizeof(array) / sizeof(array[0]), out_list, sizeof(array[0]), 0)
#define ARRAY_INIT_LIST_MACRO_CHOOSER(...)			\
GET_ARGUMENT_5(__VA_ARGS__,							\
ARRAY_INIT_LIST_5,									\
ARRAY_INIT_LIST_4,									\
ARRAY_INIT_LIST_3,									\
ARRAY_INIT_LIST_2, )

/*
*	Initializes a list, and copies an already existing array to the list.
*
*	/param	array				The initial size of the list. (How many values can be stored)
*	/param	length				The length the array was created with.	*Required for pointer arrays.
*	/param	*out_list			(Out)The initialized list.
*	/param	elem_size			The size of the value's type (in bytes).
*	/param	populated_length	The count of elements currently in the array.
*
*/
#define INTERNAL_ARRAY_COPY_INIT_LIST(array, length, out_list, elem_size, populated_length) \
internal_array_copy_init_list(array, length * elem_size, elem_size, (populated_length * elem_size), out_list);

#define ARRAY_COPY_INIT_LIST_5(array, length, populated_length, elem_size, out_list)	INTERNAL_ARRAY_COPY_INIT_LIST(array, length, out_list, elem_size, populated_length)
#define ARRAY_COPY_INIT_LIST_4(array, length, populated_length, out_list)				INTERNAL_ARRAY_COPY_INIT_LIST(array, length, out_list, sizeof(array[0]), populated_length)
#define ARRAY_COPY_INIT_LIST_3(array, length, out_list)									INTERNAL_ARRAY_COPY_INIT_LIST(array, length, out_list,sizeof(array[0]), 0 )
#define ARRAY_COPY_INIT_LIST_2(array, out_list)											INTERNAL_ARRAY_COPY_INIT_LIST(array, sizeof(array) / sizeof(array[0]), out_list, sizeof(array[0]), 0)
#define ARRAY_COPY_INIT_LIST_MACRO_CHOOSER(...)			\
GET_ARGUMENT_5(__VA_ARGS__,								\
ARRAY_COPY_INIT_LIST_5,									\
ARRAY_COPY_INIT_LIST_4,									\
ARRAY_COPY_INIT_LIST_3,									\
ARRAY_COPY_INIT_LIST_2, )

#pragma endregion Initialization Macros

#pragma region Get/Set Macros


/*
*	Moves the array so the tail is at index 0.
*
*	List_Set(list, value, index)
*
*	/param		list		Pointer to the list.
*	/param		value		Value to set.
*	/param		index		Index being set to value.
*/
#define REPEATER_MACRO_GENETICC_LIST_SET_MEMORY(list, value, index) internal_list_set_memory(list, (generic_union_t){.u32 = (uint32_t)value}.cp, index)
#define INTERNAL_LIST_SET(list, value, index) _Generic((value),														\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_SET_MEMORY(list, value, index))							\
default:						internal_list_set_generic(list, (generic_union_t)value, index))




/*
*	Val
*/
#define LIST_VAL_3(list, value, index)		INTERNAL_LIST_SET(list, value, index )
#define LIST_VAL_2(list, index)				internal_list_get(list, index)
#define LIST_VAL_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,			\
LIST_VAL_3,							\
LIST_VAL_2, )

#pragma endregion Get/Set Macros

#pragma region Size and Capacity Macros


/*
*	Capacity
*/
#define LIST_CAPACITY_2(list, capacity)		internal_list_set_capacity(list, capacity )
#define LIST_CAPACITY_1(list)				internal_list_get_capacity(list)
#define LIST_CAPACITY_MACRO_CHOOSER(...)	\
GET_ARGUMENT_2(__VA_ARGS__,					\
LIST_CAPACITY_2,							\
LIST_CAPACITY_1, )


/*
*	Adjust Head
*/
#define LIST_ADJUSTHEAD_3(list, n, erase)		internal_list_adjustHead(list, n, erase)
#define LIST_ADJUSTHEAD_2(list, n)				internal_list_adjustHead(list, n, false)
#define LIST_ADJUSTHEAD_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,					\
LIST_ADJUSTHEAD_3,							\
LIST_ADJUSTHEAD_2, )

/*
*	Adjust tail
*/
#define LIST_ADJUSTTAIL_3(list, n, erase)		internal_list_adjustTail(list, n, erase)
#define LIST_ADJUSTTAIL_2(list, n)				internal_list_adjustTail(list, n, false)
#define LIST_ADJUSTTAIL_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,					\
LIST_ADJUSTTAIL_3,							\
LIST_ADJUSTTAIL_2, )





#pragma endregion Size and Capacity Macros

#pragma region Macros

#include "../../preprocess/geneticc_macro_type_repeater.h"



/*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to search for.
*	/param	size		The total size of the array
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			If the array contains the value.
*/
#define REPEATER_MACRO_GENETICC_LIST_CONTAINS_MEMORY(array, value, size, elem_size) internal_array_contains_memory(array, (generic_union_t){.u32 = (uint32_t)value}.cp, size, elem_size)
#define INTERNAL_LIST_CONTAINS(array, value, size, elem_size) _Generic((value),													\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_CONTAINS_MEMORY(array, value, size, elem_size))						\
float :					internal_call_array_contains_special(array, (generic_union_t)value, size / elem_size, TYPE_FLOAT),		\
double :				internal_call_array_contains_special(array, (generic_union_t)value, size / elem_size,  TYPE_DOUBLE),	\
default:				internal_array_contains_generic(array, (generic_union_t)value, size, elem_size))

#define LIST_CONTAINS_4(list, value, start, length)	INTERNAL_LIST_CONTAINS((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), value, length * INTERNAL_LIST_GET_ELEM_SIZE(list), INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_CONTAINS_3(list, value, start)			INTERNAL_LIST_CONTAINS((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), value, (List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_CONTAINS_2(list, value)				INTERNAL_LIST_CONTAINS(List_Array(list), value, List_Size(list), INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_CONTAINS_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,					\
LIST_CONTAINS_4,							\
LIST_CONTAINS_3,							\
LIST_CONTAINS_2, )


/*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to search for.
*	/param	size		The total size of the array
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the value in the array, or -1 if it does not exist.
*/
#define REPEATER_MACRO_GENETICC_LIST_INDEXOF_MEMORY(array, value, size, elem_size) internal_array_indexOf_memory(array, (generic_union_t){.u32 = (uint32_t)value}.cp, size, elem_size)
#define INTERNAL_LIST_INDEXOF(array, value, size, elem_size) _Generic((value),													\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_INDEXOF_MEMORY(array, value, size, elem_size))							\
float :					internal_call_array_indexOf_special(array, (generic_union_t)value, size / elem_size, TYPE_FLOAT),		\
double :				internal_call_array_indexOf_special(array, (generic_union_t)value, size / elem_size, TYPE_DOUBLE),		\
default:				internal_array_indexOf_generic(array, (generic_union_t)value, size, elem_size))

#define LIST_INDEXOF_4(list, value, start, length)	INTERNAL_ARRAY_GET_TRUE_INDEX(	INTERNAL_LIST_INDEXOF((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), value, length * INTERNAL_LIST_GET_ELEM_SIZE(list),INTERNAL_LIST_GET_ELEM_SIZE(list)), start)
#define LIST_INDEXOF_3(list, value, start)			INTERNAL_ARRAY_GET_TRUE_INDEX(	INTERNAL_LIST_INDEXOF((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), value, List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list)),INTERNAL_LIST_GET_ELEM_SIZE(list)), start)
#define LIST_INDEXOF_2(list, value)													INTERNAL_LIST_INDEXOF(List_Array(list), value, List_Count(list), INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_INDEXOF_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,				\
LIST_INDEXOF_4,							\
LIST_INDEXOF_3,							\
LIST_INDEXOF_2, )

/*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to search for.
*	/param	size		The total size of the array
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The last index of the value in the array, or -1 if it does not exist.
*/
#define REPEATER_MACRO_GENETICC_LIST_LASTINDEXOF_MEMORY(array, value, size, elem_size) internal_array_lastIndexOf_memory( array, (generic_union_t){.u32 = (uint32_t)value}.cp, size, elem_size)
#define INTERNAL_LIST_LASTINDEXOF(array, value, size, elem_size) _Generic((value),													\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_LASTINDEXOF_MEMORY(array, value, size, elem_size))							\
float :					internal_call_array_indexOf_special(array, (generic_union_t)value, size / elem_size, TYPE_FLOAT),			\
double :				internal_call_array_indexOf_special(array, (generic_union_t)value, size / elem_size, TYPE_DOUBLE),			\
default:				internal_array_lastIndexOf_generic(array, (generic_union_t)value, size, elem_size))


#define LIST_LASTINDEXOF_4(list, value, start, length)	INTERNAL_ARRAY_GET_TRUE_INDEX(	INTERNAL_LIST_LASTINDEXOF((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), value, length * INTERNAL_LIST_GET_ELEM_SIZE(list), INTERNAL_LIST_GET_ELEM_SIZE(list)), start)
#define LIST_LASTINDEXOF_3(list, value, start)			INTERNAL_ARRAY_GET_TRUE_INDEX(	INTERNAL_LIST_LASTINDEXOF((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), value, List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), INTERNAL_LIST_GET_ELEM_SIZE(list)), start)
#define LIST_LASTINDEXOF_2(list, value)													INTERNAL_LIST_LASTINDEXOF(List_Array(list), value, List_Size(list), INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_LASTINDEXOF_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,					\
LIST_LASTINDEXOF_4,							\
LIST_LASTINDEXOF_3,							\
LIST_LASTINDEXOF_2, )


/*
*	/param	value		The value to match and count.
*	/param	array		Pointer to the start of the array.
*	/param	size		The total size of the array
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The amount of times "value" is in the array.
*/
#define REPEATER_MACRO_GENETICC_LIST_VALUECOUNT_MEMORY(array, value, size, elem_size) internal_array_valueCount_memory(array, (generic_union_t){.u32 = (uint32_t)value}.cp, size, elem_size)
#define INTERNAL_LIST_VALUECOUNT(array, value, size, elem_size) _Generic((value),												\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_VALUECOUNT_MEMORY(array, value, size, elem_size))						\
default:				internal_array_valueCount_generic(array, (generic_union_t)value, size, elem_size))

#define LIST_VALUECOUNT_4(list, value, start, length)		INTERNAL_LIST_VALUECOUNT(value, (List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), length * INTERNAL_LIST_GET_ELEM_SIZE(list), INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_VALUECOUNT_3(list, value, start)				INTERNAL_LIST_VALUECOUNT(value, (List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list)),INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_VALUECOUNT_2(list, value)						INTERNAL_LIST_VALUECOUNT(value, List_Array(list), List_Size(list),INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_VALUECOUNT_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,					\
LIST_VALUECOUNT_4,							\
LIST_VALUECOUNT_3,							\
LIST_VALUECOUNT_2, )


/*
*	/param	array		Pointer to the start of the array.
*	/param	size		The total size of the array
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the largest value in the array.
*/
#define REPEATER_MACRO_GENETICC_LIST_MAX_MEMORY_SIGNED(array, length, elem_size) internal_array_max_memory(array, size, elem_size, false)
#define REPEATER_MACRO_GENETICC_LIST_MAX_MEMORY_UNSIGNED(array, length, elem_size) internal_array_max_memory(array, size, elem_size, true)
#define INTERNAL_LIST_MAX(array, length, elem_size) _Generic((array),															\
GENETICC_REPEAT_POINTER_TYPES_SIGNED(REPEATER_MACRO_GENETICC_LIST_MAX_MEMORY_SIGNED(array, (length * elem_size),elem_size))		\
GENETICC_REPEAT_POINTER_TYPES_UNSIGNED(REPEATER_MACRO_GENETICC_LIST_MAX_MEMORY_UNSIGNED(array, (length * elem_size),elem_size))	\
float* :					internal_array_maxf_memory(array, size, elem_size),													\
double* :					internal_array_maxf_memory(array, size, elem_size))

#define LIST_MAX_3(list, start, length)		INTERNAL_LIST_MAX((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), length * INTERNAL_LIST_GET_ELEM_SIZE(list), sizeof(list[0]), start)
#define LIST_MAX_2(list, start)				INTERNAL_LIST_MAX((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), sizeof(list[0]), start)
#define LIST_MAX_1(list)					INTERNAL_LIST_MAX(list, (sizeof(list) / sizeof(list[0])), sizeof(list[0]))
#define LIST_MAX_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,			\
LIST_MAX_3,							\
LIST_MAX_2,							\
LIST_MAX_1, )


#define LIST_MAX_VALUE_3(list, start, length)		List_Array(list)[INTERNAL_ARRAY_GET_TRUE_INDEX(	INTERNAL_LIST_MAX((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), length * INTERNAL_LIST_GET_ELEM_SIZE(list), INTERNAL_LIST_GET_ELEM_SIZE(list)), start)]
#define LIST_MAX_VALUE_2(list, start)				List_Array(list)[INTERNAL_ARRAY_GET_TRUE_INDEX(	INTERNAL_LIST_MAX((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), INTERNAL_LIST_GET_ELEM_SIZE(list)), start)]
#define LIST_MAX_VALUE_1(list)						List_Array(list)[								INTERNAL_LIST_MAX((List_Array(list) + start), List_Size(list), INTERNAL_LIST_GET_ELEM_SIZE(list))]
#define LIST_MAX_VALUE_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,					\
LIST_MAX_VALUE_3,							\
LIST_MAX_VALUE_2,							\
LIST_MAX_VALUE_1, )




/*
*	/param	list		Pointer to the start of the list.
*	/param	length		The count of elements in the list. (eg. int[10] = 10)
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the smallest value in the list.
*/
#define REPEATER_MACRO_GENETICC_LIST_MIN_MEMORY_SIGNED(array, length, elem_size) internal_array_min_memory(array, size, elem_size, false)
#define REPEATER_MACRO_GENETICC_LIST_MIN_MEMORY_UNSIGNED(array, length, elem_size) internal_array_min_memory(array, size, elem_size, true)
#define INTERNAL_LIST_MIN(array, length, elem_size) _Generic((array),															\
GENETICC_REPEAT_POINTER_TYPES_SIGNED(REPEATER_MACRO_GENETICC_LIST_MIN_MEMORY_SIGNED(array, (length * elem_size),elem_size))		\
GENETICC_REPEAT_POINTER_TYPES_UNSIGNED(REPEATER_MACRO_GENETICC_LIST_MIN_MEMORY_UNSIGNED(array, (length * elem_size),elem_size))	\
float* :						internal_array_minf_memory(array, size, elem_size),			\
double* :						internal_array_minf_memory(array, size, elem_size))


#define LIST_MIN_3(list, start, length)				INTERNAL_LIST_MIN((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), length * INTERNAL_LIST_GET_ELEM_SIZE(list) , INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_MIN_2(list, start)						INTERNAL_LIST_MIN((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_MIN_1(list)							INTERNAL_LIST_MIN(List_Array(list), List_Size(list), INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_MIN_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,			\
LIST_MIN_3,							\
LIST_MIN_2,							\
LIST_MIN_1, )

#define LIST_MIN_VALUE_3(list, start, length)		List_Array(list)[INTERNAL_ARRAY_GET_TRUE_INDEX(			INTERNAL_LIST_MIN((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), length * INTERNAL_LIST_GET_ELEM_SIZE(list), INTERNAL_LIST_GET_ELEM_SIZE(list))]
#define LIST_MIN_VALUE_2(list, start)				List_Array(list)[INTERNAL_ARRAY_GET_TRUE_INDEX(			INTERNAL_LIST_MIN((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), INTERNAL_LIST_GET_ELEM_SIZE(list))]
#define LIST_MIN_VALUE_1(list)						List_Array(list)[										INTERNAL_LIST_MIN(List_Array(list), List_Size(list), INTERNAL_LIST_GET_ELEM_SIZE(list))]
#define LIST_MIN_VALUE_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,					\
LIST_MIN_VALUE_3,							\
LIST_MIN_VALUE_2,							\
LIST_MIN_VALUE_1, )


/*
*	/param	list		Pointer to the start of the list.
*	/param	length		The count of elements in the list. (eg. int[10] = 10)
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the smallest value in the list.
*/
#define REPEATER_MACRO_GENETICC_LIST_SUM_MEMORY(array, length, elem_size) internal_array_sum_memory(array, length, elem_size)
#define INTERNAL_LIST_SUM(array, length, elem_size) _Generic((array),												\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT_UNION(REPEATER_MACRO_GENETICC_LIST_SUM_MEMORY(array, length, elem_size))	\
float* :						internal_array_sumf_memory(array, size,elem_size),									\
double* :						internal_array_sumf_memory(array, size,elem_size))

#define LIST_SUM_3(list, start, length)		INTERNAL_ARRAY_SUM((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), length * INTERNAL_LIST_GET_ELEM_SIZE(list), INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_SUM_2(list, start)				INTERNAL_ARRAY_SUM((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_SUM_1(list)					INTERNAL_ARRAY_SUM(List_Array(list), List_Size(list), INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_SUM_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,			\
LIST_SUM_3,							\
LIST_SUM_2,							\
LIST_SUM_1, )



/*
*	/param	list		Pointer to the start of the list.
*	/param	length		The count of elements in the list. (eg. int[10] = 10)
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			The index of the smallest value in the list.
*/
#define REPEATER_MACRO_GENETICC_LIST_AVERAGE_MEMORY(array, size, elem_size) internal_array_average_memory(array, size, elem_size)
#define INTERNAL_LIST_AVERAGE(array, size, elem_size) _Generic((array),													\
GENETICC_REPEAT_POINTER_TYPES_NO_FLOAT_UNION(REPEATER_MACRO_GENETICC_LIST_AVERAGE_MEMORY(array, size, elem_size))	\
float* :						internal_array_averagef_memory(array, size, elem_size),									\
double* :						internal_array_averagef_memory(array, size, elem_size))

#define LIST_AVERAGE_3(list, start, length)		INTERNAL_ARRAY_AVERAGE((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), length * INTERNAL_LIST_GET_ELEM_SIZE(list), INTERNAL_LIST_GET_ELEM_SIZE(list))// / length
#define LIST_AVERAGE_2(list, start)				INTERNAL_ARRAY_AVERAGE((List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list))), List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), INTERNAL_LIST_GET_ELEM_SIZE(list))// / length
#define LIST_AVERAGE_1(list)					INTERNAL_ARRAY_AVERAGE(List_Array(list), List_Size(list), INTERNAL_LIST_GET_ELEM_SIZE(list))// / (sizeof(list) / INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_AVERAGE_MACRO_CHOOSER(...)	\
GET_ARGUMENT_3(__VA_ARGS__,				\
LIST_AVERAGE_3,							\
LIST_AVERAGE_2,							\
LIST_AVERAGE_1, )


/*
*	For Each
*/
#define LIST_FOREACH_4(list, action, index, count)		internal_list_forEach(list, action, index * INTERNAL_LIST_GET_ELEM_SIZE(list), count * INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_FOREACH_3(list, action, index)				internal_list_forEach(list, action, index * INTERNAL_LIST_GET_ELEM_SIZE(list), 0)
#define LIST_FOREACH_2(list, action)					internal_list_forEach(list, action, 0, 0)
#define LIST_FOREACH_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,				\
LIST_FOREACH_4,							\
LIST_FOREACH_3,							\
LIST_FOREACH_2, )


/*
*	All
*/
#define LIST_ALL_4(list, action, index, count)		internal_list_all(list, action, index * INTERNAL_LIST_GET_ELEM_SIZE(list), count * INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_ALL_3(list, action, index)				internal_list_all(list, action, index * INTERNAL_LIST_GET_ELEM_SIZE(list), 0)
#define LIST_ALL_2(list, action)					internal_list_all(list, action, 0, 0)
#define LIST_ALL_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,			\
LIST_ALL_4,							\
LIST_ALL_3,							\
LIST_ALL_2, )


/*
*	Any
*/
#define LIST_ANY_4(list, action, index, count)		internal_list_any(list, action, index * INTERNAL_LIST_GET_ELEM_SIZE(list), count * INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_ANY_3(list, action, index)				internal_list_any(list, action, index * INTERNAL_LIST_GET_ELEM_SIZE(list), 0)
#define LIST_ANY_2(list, action)					internal_list_any(list, action, 0, 0)
#define LIST_ANY_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,			\
LIST_ANY_4,							\
LIST_ANY_3,							\
LIST_ANY_2, )


/*
*	Select
*/
#define LIST_SELECT_4(list, predicate, start, length)	INTERNAL_ARRAY_GET_TRUE_INDEX(	internal_array_select_memory(List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), length * INTERNAL_LIST_GET_ELEM_SIZE(list), INTERNAL_LIST_GET_ELEM_SIZE(list), predicate), start)
#define LIST_SELECT_3(list, predicate, start)			INTERNAL_ARRAY_GET_TRUE_INDEX(	internal_array_select_memory(List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), INTERNAL_LIST_GET_ELEM_SIZE(list), predicate), start)
#define LIST_SELECT_2(list, predicate)													internal_list_select_memory(list, predicate)
#define LIST_SELECT_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,				\
LIST_SELECT_4,							\
LIST_SELECT_3,							\
LIST_SELECT_2, )


/*
*	Select Pointer
*/
#define LIST_SELECTPOINTER_4(list, predicate, start, length)	internal_list_get(	INTERNAL_ARRAY_GET_TRUE_INDEX(	internal_array_select_memory(List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), length * INTERNAL_LIST_GET_ELEM_SIZE(list), INTERNAL_LIST_GET_ELEM_SIZE(list), predicate), start))
#define LIST_SELECTPOINTER_3(list, predicate, start)			internal_list_get(	INTERNAL_ARRAY_GET_TRUE_INDEX(	internal_array_select_memory(List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), INTERNAL_LIST_GET_ELEM_SIZE(list), predicate), start))
#define LIST_SELECTPOINTER_2(list, predicate)					internal_list_get(	list,							internal_list_select_memory(list, predicate))
#define LIST_SELECTPOINTER_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,						\
LIST_SELECTPOINTER_4,							\
LIST_SELECTPOINTER_3,							\
LIST_SELECTPOINTER_2, )

/*
*	Select Vargs
*/
#define LIST_SELECT_VARGS_6(list, predicate, arg_count, ap, start, length)	INTERNAL_ARRAY_GET_TRUE_INDEX(	internal_array_select_vargs_memory(List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), length * INTERNAL_LIST_GET_ELEM_SIZE(list), INTERNAL_LIST_GET_ELEM_SIZE(list), predicate, arg_count, ap), start)
#define LIST_SELECT_VARGS_5(list, predicate, arg_count, ap, start)			INTERNAL_ARRAY_GET_TRUE_INDEX(	internal_array_select_vargs_memory(List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), INTERNAL_LIST_GET_ELEM_SIZE(list), predicate, arg_count, ap), start)
#define LIST_SELECT_VARGS_4(list, predicate, arg_count, ap)													internal_list_select_vargs_memory(list, predicate, arg_count, ap)
#define LIST_SELECT_VARGS_MACRO_CHOOSER(...)	\
GET_ARGUMENT_6(__VA_ARGS__,						\
LIST_SELECT_VARGS_6,							\
LIST_SELECT_VARGS_5,							\
LIST_SELECT_VARGS_4, )

/*
*	Select Vargs
*/
#define LIST_SELECTPOINTER_VARGS_6(list, predicate, arg_count, ap, start, length)	internal_list_get(	INTERNAL_ARRAY_GET_TRUE_INDEX(	internal_array_select_vargs_memory(List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), length * INTERNAL_LIST_GET_ELEM_SIZE(list), INTERNAL_LIST_GET_ELEM_SIZE(list), predicate, arg_count, ap), start))
#define LIST_SELECTPOINTER_VARGS_5(list, predicate, arg_count, ap, start)			internal_list_get(	INTERNAL_ARRAY_GET_TRUE_INDEX(	internal_array_select_vargs_memory(List_Array(list) + (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), List_Size(list) - (start * INTERNAL_LIST_GET_ELEM_SIZE(list)), INTERNAL_LIST_GET_ELEM_SIZE(list), predicate, arg_count, ap), start))
#define LIST_SELECTPOINTER_VARGS_4(list, predicate, arg_count, ap)					internal_list_get(									internal_list_select_vargs_memory(list, predicate, arg_count, ap))
#define LIST_SELECTPOINTER_VARGS_MACRO_CHOOSER(...)	\
GET_ARGUMENT_6(__VA_ARGS__,						\
LIST_SELECTPOINTER_VARGS_6,							\
LIST_SELECTPOINTER_VARGS_5,							\
LIST_SELECTPOINTER_VARGS_4, )




#pragma endregion Macros


#pragma region Unsafe Macros


/*
*	/param	list			Pointer to the start of the list.
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*	/param	index			Zero based byte offset at where to start.
*	/param	size			The size of the range elements to check.
*
*	/returns	LIST_PTR	A list of zero based indexes to all values in the array that matched the predicate, or NULL if there were no matches.
*/
#define INTERNAL_LIST_SELECTMANY_INDEXES(list, predicate, offset, size)	\
internal_list_selectMany_indexes(list,  predicate, offset, size)

#define LIST_SELECTMANY_INDEXES_4(list, predicate, index, count)	INTERNAL_LIST_SELECTMANY_INDEXES(list, predicate, index * INTERNAL_LIST_GET_ELEM_SIZE(list), count * INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_SELECTMANY_INDEXES_3(list, predicate, index)			INTERNAL_LIST_SELECTMANY_INDEXES(list, predicate, index * INTERNAL_LIST_GET_ELEM_SIZE(list), 0)
#define LIST_SELECTMANY_INDEXES_2(list, predicate)					INTERNAL_LIST_SELECTMANY_INDEXES(list, predicate, 0, 0)
#define LIST_SELECTMANY_INDEXES_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,							\
LIST_SELECTMANY_INDEXES_4,							\
LIST_SELECTMANY_INDEXES_3,							\
LIST_SELECTMANY_INDEXES_2, )


/*
*	/param	list			Pointer to the start of the list.
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*	/param	index			Zero based byte offset at where to start.
*	/param	size			The size of the range elements to check.
*
*	/returns	LIST_PTR	A list containing pointers to each value in the list that matched the predicate's conditions, or NULL if there was no matches.
*/
#define INTERNAL_LIST_SELECTMANY_POINTERS(list, predicate, offset, size)	\
internal_list_selectMany_pointers(list,  predicate, offset, size)

#define LIST_SELECTMANY_POINTERS_4(list, predicate, index, count)	INTERNAL_LIST_SELECTMANY_POINTERS(list, predicate, index * INTERNAL_LIST_GET_ELEM_SIZE(list), count * INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_SELECTMANY_POINTERS_3(list, predicate, index)			INTERNAL_LIST_SELECTMANY_POINTERS(list, predicate, index * INTERNAL_LIST_GET_ELEM_SIZE(list), 0)
#define LIST_SELECTMANY_POINTERS_2(list, predicate)					INTERNAL_LIST_SELECTMANY_POINTERS(list, predicate, 0, 0)
#define LIST_SELECTMANY_POINTERS_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,							\
LIST_SELECTMANY_POINTERS_4,							\
LIST_SELECTMANY_POINTERS_3,							\
LIST_SELECTMANY_POINTERS_2, )

/*
*	/param	list			Pointer to the start of the list.
*	/param	predicate		A pointer to a function which checks each value to its conditions.
*	/param	index			Zero based byte offset at where to start.
*	/param	size			The size of the range elements to check.
*
*	/returns	LIST_PTR	A list of values from the original list that matched the conditions defined by predicate, or NULL if there was no matches.
*/
#define INTERNAL_LIST_SELECTMANY_VALUES(list, predicate, offset, size)	\
internal_list_selectMany_values(list,  predicate, offset, size)

#define LIST_SELECTMANY_VALUES_4(list, predicate, index, count)		INTERNAL_LIST_SELECTMANY_VALUES(list, predicate, index * INTERNAL_LIST_GET_ELEM_SIZE(list), count * INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_SELECTMANY_VALUES_3(list, predicate, index)			INTERNAL_LIST_SELECTMANY_VALUES(list, predicate, index * INTERNAL_LIST_GET_ELEM_SIZE(list), 0)
#define LIST_SELECTMANY_VALUES_2(list, predicate)					INTERNAL_LIST_SELECTMANY_VALUES(list, predicate, 0, 0)
#define LIST_SELECTMANY_VALUES_MACRO_CHOOSER(...)	\
GET_ARGUMENT_4(__VA_ARGS__,							\
LIST_SELECTMANY_VALUES_4,							\
LIST_SELECTMANY_VALUES_3,							\
LIST_SELECTMANY_VALUES_2, )

/*
*	/param	list		Pointer to the start of the list.
*	/param	range		The range of values to insert.
*	/param	offset		The byte offset where the new values should be inserted.
*	/param	byte_count	The amount of bytes to insert
*	/param	elem_size	The size of the elements in range. (Can be different than list, but if larger, and/or signed or floating type, you should cast the array first using Array_ConvertTo)
*
*	/returns	bool		True if the range was inserted, or false if the capacity was maxed out.
*/
#define INTERNAL_LIST_INSERTRANGE(list, range, offset, byte_count, elem_size ) \
internal_list_insertRange_memory(list, range, offset, byte_count, elem_size)

#define LIST_INSERTRANGE_GENETICC_5(list, range, index, length, elem_size)		INTERNAL_LIST_INSERTRANGE(list, range, index * INTERNAL_LIST_GET_ELEM_SIZE(list), length * INTERNAL_LIST_GET_ELEM_SIZE(list), elem_size)
#define LIST_INSERTRANGE_GENETICC_4(list, range, index, length)					INTERNAL_LIST_INSERTRANGE(list, range, index * INTERNAL_LIST_GET_ELEM_SIZE(list), length * INTERNAL_LIST_GET_ELEM_SIZE(list), sizeof(range[0]))
#define LIST_INSERTRANGE_GENETICC_3(list, range, index)							INTERNAL_LIST_INSERTRANGE(list, range, index * INTERNAL_LIST_GET_ELEM_SIZE(list), sizeof(range), sizeof(range[0]))
#define LIST_INSERTRANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,						\
LIST_INSERTRANGE_GENETICC_5,						\
LIST_INSERTRANGE_GENETICC_4,						\
LIST_INSERTRANGE_GENETICC_3,)



/*
*	/param	list		Pointer to the start of the list.
*	/param	value		The value to search for.
*	/param	offset		The byte offset where the new values should be inserted.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns	bool		True if the element was inserted, or false if the capacity was maxed out.
*/
#define REPEATER_MACRO_GENETICC_LIST_INSERT_MEMORY(list, value, offset, elem_size) internal_list_insert_memory(list, (generic_union_t){.u32 = (uint32_t)value}.cp, offset, elem_size)
#define INTERNAL_LIST_INSERT(list, value, offset, elem_size ) _Generic((value),											\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_INSERT_MEMORY(list, value, offset, elem_size))					\
default:				internal_list_insert_generic(list, (generic_union_t)value, offset, sizeof(value)))

#define LIST_INSERT_GENETICC_4(list, value, index, elem_size)				INTERNAL_LIST_INSERT(list, value, index * INTERNAL_LIST_GET_ELEM_SIZE(list), elem_size)
#define LIST_INSERT_GENETICC_3(list, value, index)							INTERNAL_LIST_INSERT(list, value, index * INTERNAL_LIST_GET_ELEM_SIZE(list), 0)
#define LIST_INSERT_MACRO_CHOOSER(...)		\
GET_ARGUMENT_4(__VA_ARGS__,					\
LIST_INSERT_GENETICC_4,						\
LIST_INSERT_GENETICC_3,)



/*
*	List_AddRange(list, range, range_length)			*count must be passed when range is not static
*	List_AddRange(list, range, range_length, elem_size)
*
*	/param	list			Pointer to the start of the list.
*	/param	range			The range of values to insert.
*	/param	index			The index where the new values should be inserted at.
*	/param	range_length	Count of how many elements are in range.
*	/param	elem_size		The size of the elements being inserted.
*
*	/returns	bool		True if the range was added, or false if the capacity was maxed out.
*/
#define INTERNAL_LIST_ADDRANGE(list, range, range_length, elem_size ) \
internal_list_insertRange_memory(list, (generic_union_t){.u64 = (uint32_t)range}.cp, internal_list_get_size(list), (range_length * elem_size), elem_size)
//list_addRange_memory((generic_union_t){.u32 = (uint32_t)value}.cp,  list, count, (length * elem_size),  elem_size)

#define LIST_ADDRANGE_GENETICC_4(list, range, range_length, elem_size)		INTERNAL_LIST_ADDRANGE(list, range, range_length, elem_size)
#define LIST_ADDRANGE_GENETICC_3(list, range, range_length)					INTERNAL_LIST_ADDRANGE(list, range, range_length, sizeof(range[0]))
#define LIST_ADDRANGE_GENETICC_2(list, range)								INTERNAL_LIST_ADDRANGE(list, range, sizeof(range) / sizeof(range[0]), sizeof(range[0]))
#define LIST_ADDRANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_4(__VA_ARGS__,						\
LIST_ADDRANGE_GENETICC_4,						\
LIST_ADDRANGE_GENETICC_3,						\
LIST_ADDRANGE_GENETICC_2,)

/*
*	List_Add(list, value)			*count must be passed when range is not static
*	List_Add(list, value, elem_size)
*
*	/param	list			Pointer to the start of the list.
*	/param	value			The value or pointer to the value to add.

*
*	/returns	bool		True if the element was added, or false if the capacity was maxed out.
*/
#define REPEATER_MACRO_GENETICC_LIST_ADD_MEMORY(list, value, elem_size) internal_list_insert_memory(list, (generic_union_t){.u32 = (uint32_t)value}.cp, internal_list_get_size(list), elem_size)
#define INTERNAL_LIST_ADD(list, value, elem_size) _Generic((value),												\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_ADD_MEMORY(list, value, elem_size))					\
default:				internal_list_add_generic(list, (generic_union_t)value, sizeof(value)))
	
#define LIST_ADD_GENETICC_3(list, value, elem_size)				INTERNAL_LIST_ADD(list, value, elem_size)
#define LIST_ADD_GENETICC_2(list, value)							INTERNAL_LIST_ADD(list, value, 0)
#define LIST_ADD_MACRO_CHOOSER(...)		\
GET_ARGUMENT_3(__VA_ARGS__,				\
LIST_ADD_GENETICC_3,						\
LIST_ADD_GENETICC_2, )




/*
*	List_PrependRange(list, range)
*	List_PrependRange(list, range, range_length)			*count must be passed when range is not static
*	List_PrependRange(list, range, range_length, elem_size)
*
*	/param	list			Pointer to the start of the list.
*	/param	range			The range of values to insert.
*	/param	range_length	(Optional)Count of how many elements are in range.
*	/param	elem_size		(Optional)The size of the elements in range.
*
*	/returns	bool		True if the range was prepended, or false if the capacity was maxed out.
*/
#define INTERNAL_LIST_PREPENDRANGE(list, range, range_length, elem_size ) \
internal_list_insertRange_memory(list, (generic_union_t){.u64 = (uint32_t)range}.cp, 0, (range_length * elem_size), elem_size)
//list_prependRange_memory((generic_union_t){.u32 = (uint32_t)value}.cp,  list, count, (length * elem_size),  elem_size)

#define LIST_PREPENDRANGE_GENETICC_4(list, range, range_length, elem_size)		INTERNAL_LIST_PREPENDRANGE(list, range, range_length, elem_size)
#define LIST_PREPENDRANGE_GENETICC_3(list, range, range_length)					INTERNAL_LIST_PREPENDRANGE(list, range, range_length, sizeof(range[0]))
#define LIST_PREPENDRANGE_GENETICC_2(list, range)								INTERNAL_LIST_PREPENDRANGE(list, range, sizeof(range) / sizeof(range[0]), sizeof(range[0]))
#define LIST_PREPENDRANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_4(__VA_ARGS__,							\
LIST_PREPENDRANGE_GENETICC_4,						\
LIST_PREPENDRANGE_GENETICC_3,						\
LIST_PREPENDRANGE_GENETICC_2,)



/*
*	List_Prepend(list, value)			*count must be passed when range is not static
*	List_Prepend(list, value, elem_size)
*
*	/param	list			Pointer to the start of the list.
*	/param	value			The value or pointer to the value to prepend.
*	/param	elem_size		The size of the elements being inserted.
*
*	/returns	bool		True if the range was prepended, or false if the capacity was maxed out.
*/
#define REPEATER_MACRO_GENETICC_LIST_PREPEND_MEMORY(list, value, elem_size) internal_list_insertRange_memory(list, (generic_union_t){.u32 = (uint32_t)value}.cp, 0, elem_size, elem_size)
#define INTERNAL_LIST_PREPEND(list, value, elem_size) _Generic((value),																	\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_PREPEND_MEMORY(list, value, elem_size))										\
default:				internal_list_prepend_generic(list, (generic_union_t)value, sizeof(value)))

#define LIST_PREPEND_GENETICC_3(list, value, elem_size)				INTERNAL_LIST_PREPEND(list, value, elem_size)
#define LIST_PREPEND_GENETICC_2(list, value)							INTERNAL_LIST_PREPEND(list, value, 0)
#define LIST_PREPEND_MACRO_CHOOSER(...)		\
GET_ARGUMENT_3(__VA_ARGS__,					\
LIST_PREPEND_GENETICC_3,						\
LIST_PREPEND_GENETICC_2, )


/*
*	List_RemoveRange(list, index, length)
*
*	/param	list			Pointer to the start of the list.
*	/param	index			The index where the new values should be inserted at.
*	/param	length			Count of how many elements to remove.
*/
#define INTERNAL_LIST_REMOVERANGE(list, index, length ) \
internal_list_removeRange_memory(list, index * INTERNAL_LIST_GET_ELEM_SIZE(list), length * INTERNAL_LIST_GET_ELEM_SIZE(list))
//list_removeRange_memory((generic_union_t){.u32 = (uint32_t)value}.cp,  list, count, (length * elem_size),  elem_size)



/*
*	/param	list			Pointer to the start of the list.
*	/param	index			The index of the element to remove.
*
*	/returns	uint8_t*	If the list contains the value.
*/
#define INTERNAL_LIST_REMOVEAT(list, index ) \
internal_list_removeAt_memory(list, (index * INTERNAL_LIST_GET_ELEM_SIZE(list)))



/*
*	List_Remove(list, value)
*
*	/param	list			Pointer to the start of the list.
*	/param	value			The value to remove.
*
*	/returns				True or false depending on if an item was found and removed.
*/
#define REPEATER_MACRO_GENETICC_LIST_REMOVE_MEMORY(list, value) internal_list_remove_memory(list, (generic_union_t){.u32 = (uint32_t)value}.cp)
#define INTERNAL_LIST_REMOVE(list, value )  _Generic((value),													\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_REMOVE_MEMORY(list, value))							\
default:				internal_list_remove_generic(list, (generic_union_t)value))


/*
*	List_RemoveAll(list, value)
*
*	/param	list			Pointer to the start of the list.
*	/param	value			The value to remove.
*
*	/returns				True or false depending on if an item was found and removed.
*/
#define REPEATER_MACRO_GENETICC_LIST_REMOVEALL_MEMORY(list, value) internal_list_removeAll_memory(list, (generic_union_t){.u32 = (uint32_t)value}.cp)
#define INTERNAL_LIST_REMOVEALL(list, value )  _Generic((value),												\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_REMOVEALL_MEMORY(list, value))							\
default:				internal_list_removeAll_generic(list, (generic_union_t)value))



#pragma region DMA Macros


/*
*	Initializes a list by copying an already existing array.
*	Copies the array_dma into a new block of memory. (Use Array_To_list to use the same array)
*
*	/param	array				The initial size of the list. (How many values can be stored)
*	/param	length				Either the length of the array_dma, or the new capacity(only if "populated_length is used).	*Required for pointer array_dmas.
*	/param	populated_length	The count of elements currently in the array_dma											*If length == capacity, this is cannot be 0.
*
*	/Returns list_array_dma_t		The newly allocated list.
*/
#define INTERNAL_ARRAY_DMA_COPYTO_LIST(array, length, elem_size, populated_length) \
internal_array_dma_copyTo_list(array, (length * elem_size), elem_size, (populated_length * elem_size));

#define ARRAY_DMA_COPYTO_LIST_4(array, length_capacity, populated_length, elem_size)	INTERNAL_ARRAY_DMA_COPYTO_LIST(array, length_capacity, elem_size, populated_length)
#define ARRAY_DMA_COPYTO_LIST_3(array, length_capacity, populated_length)				INTERNAL_ARRAY_DMA_COPYTO_LIST(array, length_capacity, sizeof(array[0]), populated_length)
#define ARRAY_DMA_COPYTO_LIST_2(array, length)											INTERNAL_ARRAY_DMA_COPYTO_LIST(array, length, sizeof(array[0]), 0 )
#define ARRAY_DMA_COPYTO_LIST_1(array)													INTERNAL_ARRAY_DMA_COPYTO_LIST(array, sizeof(array) / sizeof(array[0]), sizeof(array[0]), 0)
#define ARRAY_DMA_COPYTO_LIST_MACRO_CHOOSER(...)			\
GET_ARGUMENT_4(__VA_ARGS__,									\
ARRAY_DMA_COPYTO_LIST_4,									\
ARRAY_DMA_COPYTO_LIST_3,									\
ARRAY_DMA_COPYTO_LIST_2,									\
ARRAY_DMA_COPYTO_LIST_1, )

/*
*	Initializes a list, and copies an already existing array to the list.
*
*	/param	array				The initial size of the list. (How many values can be stored)
*	/param	length				The length the array_dma was created with.	*Required for pointer array_dmas.
*	/param	*out_list			(Out)The initialized list.
*	/param	elem_size			The size of the value's type (in bytes).
*	/param	populated_length	The count of elements currently in the array_dma.
*
*/
#define INTERNAL_ARRAY_DMA_COPY_INIT_LIST(array, length, out_list, elem_size, populated_length) \
internal_array_dma_copy_init_list(array, length * elem_size, elem_size, (populated_length * elem_size), out_list);

#define ARRAY_DMA_COPY_INIT_LIST_5(array, length, populated_length, elem_size, out_list)	INTERNAL_ARRAY_DMA_COPY_INIT_LIST(array, length, out_list, elem_size, populated_length)
#define ARRAY_DMA_COPY_INIT_LIST_4(array, length, populated_length, out_list)				INTERNAL_ARRAY_DMA_COPY_INIT_LIST(array, length, out_list, sizeof(array[0]), populated_length)
#define ARRAY_DMA_COPY_INIT_LIST_3(array, length, out_list)									INTERNAL_ARRAY_DMA_COPY_INIT_LIST(array, length, out_list,sizeof(array[0]), 0 )
#define ARRAY_DMA_COPY_INIT_LIST_2(array, out_list)											INTERNAL_ARRAY_DMA_COPY_INIT_LIST(array, sizeof(array) / sizeof(array[0]), out_list, sizeof(array[0]), 0)
#define ARRAY_DMA_COPY_INIT_LIST_MACRO_CHOOSER(...)			\
GET_ARGUMENT_5(__VA_ARGS__,									\
ARRAY_DMA_COPY_INIT_LIST_5,									\
ARRAY_DMA_COPY_INIT_LIST_4,									\
ARRAY_DMA_COPY_INIT_LIST_3,									\
ARRAY_DMA_COPY_INIT_LIST_2, )


/*
*	/param list	Pointer to the start of the list_dma.
*	/param	range		The range of values to insert.
*	/param	offset		The byte offset where the new values should be inserted.
*	/param	byte_count	The amount of bytes to insert
*
*	/returns			A pointer to the start of the new list_dma.
*/
#define INTERNAL_LIST_DMA_INSERTRANGE(list, range, offset, byte_count ) \
internal_list_dma_insertRange_memory(list, range, offset, byte_count)

#define LIST_DMA_INSERTRANGE_GENETICC_3(list, range, index, count)						INTERNAL_LIST_DMA_INSERTRANGE(list, range, index * INTERNAL_LIST_GET_ELEM_SIZE(list), count * INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_DMA_INSERTRANGE_GENETICC_2(list, range, index)								INTERNAL_LIST_DMA_INSERTRANGE(list, range, index * INTERNAL_LIST_GET_ELEM_SIZE(list))
#define LIST_DMA_INSERTRANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_4(__VA_ARGS__,							\
LIST_DMA_INSERTRANGE_GENETICC_4,						\
LIST_DMA_INSERTRANGE_GENETICC_3,)


/*
*	/param list		Pointer to the start of the list_dma.
*	/param	value		The value to search for.
*	/param	offset		The byte offset where the new values should be inserted.
*	/param	elem_size	The size of value's type (in bytes).
*
*	/returns			If the list_dma contains the value.
*/
#define REPEATER_MACRO_GENETICC_LIST_DMA_INSERT_MEMORY(list, value, offset) internal_list_dma_insert_memory(list, (generic_union_t){.u32 = (uint32_t)value}.cp, offset)
#define INTERNAL_LIST_DMA_INSERT(list, value, offset ) _Generic((value),																										\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_DMA_INSERT_MEMORY(list, value, offset))																			\
default:				internal_list_dma_insert_generic(list, (generic_union_t)value, offset))



/*
*	List_DMA_AddRange(list, range, range_length)			*count must be passed when range is not static
*	List_DMA_AddRange(list, range, range_length, elem_size)
*
*	/param list			Pointer to the start of the list_dma.
*	/param	range			The range of values to insert.
*	/param	index			The index where the new values should be inserted at.
*	/param	range_length	Count of how many elements are in range.
*	/param	elem_size		The size of the elements being inserted.
*
*	/returns	bool		True if the range was added, or false if the capacity was maxed out.
*/
#define INTERNAL_LIST_DMA_ADDRANGE(list, range, range_length ) \
internal_list_dma_insertRange_memory(list, (generic_union_t){.u64 = (uint32_t)range}.cp, internal_list_get_size(list), (range_length * INTERNAL_LIST_GET_ELEM_SIZE(list)))


#define LIST_DMA_ADDRANGE_GENETICC_3(list, range, range_length)					INTERNAL_LIST_DMA_ADDRANGE(list, range, range_length)
#define LIST_DMA_ADDRANGE_GENETICC_2(list, range)								INTERNAL_LIST_DMA_ADDRANGE(list, range, sizeof(range) / sizeof(range[0]))
#define LIST_DMA_ADDRANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_3(__VA_ARGS__,							\
LIST_DMA_ADDRANGE_GENETICC_3,						\
LIST_DMA_ADDRANGE_GENETICC_2,)

/*
*	List_DMA_Add(list, value)			*count must be passed when range is not static
*	List_DMA_Add(list, value, elem_size)
*
*	/param list			Pointer to the start of the list_dma.
*	/param	value			The value or pointer to the value to add.
*	/param	elem_size		(Optional*)The size of the elements being inserted.
*
*	/returns	bool		True if the range was added, or false if the capacity was maxed out.
*/
#define REPEATER_MACRO_GENETICC_LIST_DMA_ADD_MEMORY(list, value) internal_list_dma_insert_memory(list, (generic_union_t){.u32 = (uint32_t)value}.cp, internal_list_get_size(list))
#define INTERNAL_LIST_DMA_ADD(list, value) _Generic((value),										\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_DMA_ADD_MEMORY(list, value))				\
default:				internal_list_dma_add_generic(list, (generic_union_t)value))



/*
*	List_DMA_PrependRange(list, range)
*	List_DMA_PrependRange(list, range, range_length)			*count must be passed when range is not static
*
*	/param list			Pointer to the start of the list_dma.
*	/param	range			The range of values to insert.
*	/param	range_length	(Optional)Count of how many elements are in range.
*
*	/returns	bool		True if the range was prepended, or false if the capacity was maxed out.
*/
#define INTERNAL_LIST_DMA_PREPENDRANGE(list, range, range_length ) \
internal_list_dma_insertRange_memory(list, (generic_union_t){.u64 = (uint32_t)range}.cp, 0, (range_length * elem_size))

#define LIST_DMA_PREPENDRANGE_GENETICC_3(list, range, range_length)					INTERNAL_LIST_DMA_PREPENDRANGE(list, range, range_length)
#define LIST_DMA_PREPENDRANGE_GENETICC_2(list, range)								INTERNAL_LIST_DMA_PREPENDRANGE(list, range, sizeof(range) / sizeof(range[0]))
#define LIST_DMA_PREPENDRANGE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_3(__VA_ARGS__,								\
LIST_DMA_PREPENDRANGE_GENETICC_3,						\
LIST_DMA_PREPENDRANGE_GENETICC_2,)



/*
*	List_DMA_Prepend(list, value)
*
*	/param list				Pointer to the start of the list_dma.
*	/param	value			The value or pointer to the value to prepend.
*	/param	elem_size		The size of the elements being inserted.
*
*	/returns	bool		True if the range was prepended, or false if the capacity was maxed out.
*/
#define REPEATER_MACRO_GENETICC_LIST_DMA_PREPEND_MEMORY(list, value) internal_list_dma_insertRange_memory(list, (generic_union_t){.u32 = (uint32_t)value}.cp, 0, INTERNAL_LIST_GET_ELEM_SIZE(list))
#define INTERNAL_LIST_DMA_PREPEND(list, value) _Generic((value),								\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_DMA_PREPEND_MEMORY(list, value))				\
default:				internal_list_dma_prepend_generic(list, (generic_union_t)value))




/*
*	List_DMA_Remove(list, value)
*	List_DMA_Remove(list, value, *out_removed)
*
*	/param	list			Pointer to the start of the list_dma.
*	/param	value			The value to remove.
*	/param	*out_removed	(Optional, out)True or false depending off if a value was found and removed.
*
*	/returns				True or false depending on if an item was found and removed.
*/
#define REPEATER_MACRO_GENETICC_LIST_DMA_REMOVE_MEMORY(list, value, out_removed) internal_list_dma_remove_memory(list, (generic_union_t){.u32 = (uint32_t)value}.cp, out_removed)
#define INTERNAL_LIST_DMA_REMOVE(list, value, out_removed )  _Generic((value),													\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_DMA_REMOVE_MEMORY(list, value, out_removed))							\
default:				internal_list_dma_remove_generic(list, (generic_union_t)value, out_removed))

#define LIST_DMA_REMOVE_GENETICC_3(list, value, out_removed)					INTERNAL_LIST_DMA_REMOVE(list, value, out_removed)
#define LIST_DMA_REMOVE_GENETICC_2(list, value)								INTERNAL_LIST_DMA_REMOVE(list, value, NULL)
#define LIST_DMA_REMOVE_MACRO_CHOOSER(...)		\
GET_ARGUMENT_3(__VA_ARGS__,						\
LIST_DMA_REMOVE_GENETICC_3,						\
LIST_DMA_REMOVE_GENETICC_2,)

/*
*	List_DMA_RemoveAll(list, value)
*	List_DMA_RemoveAll(list, value, *out_remove_count)
*
*	/param list					Pointer to the start of the list_dma.
*	/param	value				The value to remove.
*	/param	*out_remove_count	(Optional, out)A count of how many elements were removed.
*
*	/returns				True or false depending on if an item was found and removed.
*/
#define REPEATER_MACRO_GENETICC_LIST_DMA_REMOVEALL_MEMORY(list, value, out_remove_count) internal_list_dma_removeAll_memory(list, (generic_union_t){.u32 = (uint32_t)value}.cp, out_remove_count)
#define INTERNAL_LIST_DMA_REMOVEALL(list, value, out_remove_count )  _Generic((value),									\
GENETICC_REPEAT_POINTER_TYPES(REPEATER_MACRO_GENETICC_LIST_DMA_REMOVEALL_MEMORY(list, value, out_remove_count))			\
default:				internal_list_dma_removeAll_generic(list, (generic_union_t)value, out_remove_count))

#define LIST_DMA_REMOVEALL_GENETICC_3(list, value, out_remove_count)				INTERNAL_LIST_DMA_REMOVEALL(list, value, out_remove_count)
#define LIST_DMA_REMOVEALL_GENETICC_2(list, value)								INTERNAL_LIST_DMA_REMOVEALL(list, value, NULL)
#define LIST_DMA_REMOVEALL_MACRO_CHOOSER(...)		\
GET_ARGUMENT_3(__VA_ARGS__,						\
LIST_DMA_REMOVEALL_GENETICC_3,						\
LIST_DMA_REMOVEALL_GENETICC_2,)


/*
*	Remove Match
*/
#define LIST_DMA_REMOVEMATCH_GENETICC_3(list, predicate, out_removed)					internal_list_dma_removeMatch_memory(list, predicate, out_removed)
#define LIST_DMA_REMOVEMATCH_GENETICC_2(list, predicate)									internal_list_dma_removeMatch_memory(list, predicate, NULL)
#define LIST_DMA_REMOVEMATCH_MACRO_CHOOSER(...)		\
GET_ARGUMENT_3(__VA_ARGS__,						\
LIST_DMA_REMOVEMATCH_GENETICC_3,						\
LIST_DMA_REMOVEMATCH_GENETICC_2,)


/*
*	Remove All Matching
*/
#define LIST_DMA_REMOVEALLMATCHING_GENETICC_3(list, predicate, out_remove_count)				internal_list_dma_removeAllMatching_memory(list, predicate, out_remove_count)
#define LIST_DMA_REMOVEALLMATCHING_GENETICC_2(list, predicate)								internal_list_dma_removeAllMatching_memory(list, predicate, NULL)
#define LIST_DMA_REMOVEALLMATCHING_MACRO_CHOOSER(...)		\
GET_ARGUMENT_3(__VA_ARGS__,									\
LIST_DMA_REMOVEALLMATCHING_GENETICC_3,						\
LIST_DMA_REMOVEALLMATCHING_GENETICC_2,)

/*
*	Remove Match Vargs
*/
#define LIST_DMA_REMOVEMATCH_VARGS_GENETICC_5(list, predicate, arg_count, ap, out_removed)			internal_list_dma_removeMatch_vargs_memory(list, predicate, out_removed, arg_count, ap)
#define LIST_DMA_REMOVEMATCH_VARGS_GENETICC_4(list, predicate, arg_count, ap)						internal_list_dma_removeMatch_vargs_memory(list, predicate, NULL, arg_count, ap)
#define LIST_DMA_REMOVEMATCH_VARGS_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,									\
LIST_DMA_REMOVEMATCH_VARGS_GENETICC_5,						\
LIST_DMA_REMOVEMATCH_VARGS_GENETICC_4,)


/*
*	Remove All Matching Vargs
*/
#define LIST_DMA_REMOVEALLMATCHING_VARGS_GENETICC_5(list, predicate, arg_count, ap, out_remove_count)			internal_list_dma_removeAllMatching_vargs_memory(list, predicate, out_remove_count, arg_count, ap)
#define LIST_DMA_REMOVEALLMATCHING_VARGS_GENETICC_4(list, predicate, arg_count, ap)								internal_list_dma_removeAllMatching_vargs_memory(list, predicate, NULL, arg_count, ap)
#define LIST_DMA_REMOVEALLMATCHING_VARGS_MACRO_CHOOSER(...)		\
GET_ARGUMENT_5(__VA_ARGS__,										\
LIST_DMA_REMOVEALLMATCHING_VARGS_GENETICC_5,						\
LIST_DMA_REMOVEALLMATCHING_VARGS_GENETICC_4,)

#pragma endregion DMA Macros


#pragma endregion Unsafe Macros



#endif /* GENETICC_LIST_INTERNAL_MACROS_H_ */