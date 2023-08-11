#ifndef GENETICC_ARRAY_INTERNAL_FUNCTIONS_H_
#define GENETICC_ARRAY_INTERNAL_FUNCTIONS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

#include "Addio/GeNETiCC/geneticc.h"
#include "Addio/GeNETiCC/geneticc_defs.h"
#include "Addio/GeNETiCC/Collections/array/array_geneticc_defs.h"
#include "Addio/GeNETiCC/Collections/array/array_geneticc_config.h"


#pragma region Generic Methods

inline bool internal_array_contains_memory(const ARRAY_PTR array, const ELEMENT_PTR value, size_t size,  element_size_t elem_size );
inline bool internal_array_contains_generic(const ARRAY_PTR array, generic_union_t value, size_t size,  element_size_t elem_size);

inline bool internal_array_exists(const ARRAY_PTR array, size_t length,  element_size_t elem_size, PREDICATE predicate);

int internal_array_indexOf_memory(const ARRAY_PTR array, const ELEMENT_PTR value, size_t size,  element_size_t elem_size);
inline int internal_array_indexOf_generic(const ARRAY_PTR array, generic_union_t value,  size_t size,  element_size_t elem_size);

int internal_array_lastIndexOf_memory(const ARRAY_PTR array, const ELEMENT_PTR value, size_t size,  element_size_t elem_size);
inline int internal_array_lastIndexOf_generic(const ARRAY_PTR array, generic_union_t value, size_t size,  element_size_t elem_size);

index_t internal_array_max_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, bool isSigned);
index_t internal_array_maxf_memory(const double* array, size_t size,  element_size_t elem_size);

index_t internal_array_min_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, bool isSigned);
index_t internal_array_minf_memory(const double* array, size_t size,  element_size_t elem_size);

generic_union_t internal_array_sum_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, bool isSigned);
double internal_array_sumf_memory(const double* array, size_t length,  element_size_t elem_size);

generic_union_t internal_array_average_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, bool isSigned);
GENOPTI_INLINE double internal_array_averagef_memory(const double* array, size_t size,  element_size_t elem_size);

unsigned int internal_array_valueCount_memory(const ARRAY_PTR array,const ELEMENT_PTR value, size_t size,  element_size_t elem_size);
inline unsigned int internal_array_valueCount_generic(const ARRAY_PTR array, generic_union_t value, size_t size,  element_size_t elem_size);

int internal_array_select_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, PREDICATE predicate);

#warning remember to create macros for this.
int internal_array_select_args_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, PREDICATE_ARGS predicate, int arg_count, ...);
int internal_array_select_vargs_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, PREDICATE_ARGS predicate, int arg_count, va_list ap);

#pragma endregion Generic Methods

#pragma region Type Specific Methods - Generally faster than generic functions, but will increase code size


inline bool internal_array_contains_float(const float value, const float* array, length_t length);
inline bool internal_array_contains_double(const double value, const double* array, length_t length);

int internal_array_indexOf_float(const float value, const float* array, length_t length);
int internal_array_indexOf_double(const double value, const double* array, length_t length);

int internal_array_lastIndexOf_float(const float value, const float* array, length_t length);
int internal_array_lastIndexOf_double(const double value, const double* array, length_t length);

/*
*	To include these type specific functions in the macros, we need an intermediate function as pointers complain when they try to be cast to a floating point value.
*	Even though in macros, the pointers will never call the floating point functions, anything in _Generic has to be compatible with the value that is calling.
*	That is what the internal_call_array_..._special methods are for.
*/

/*
*	Internal function used to call type specific array_contains functions.
*/
static int internal_call_array_contains_special(ARRAY_PTR array,  generic_union_t value, size_t length, type_t elem_type)
{
	switch(elem_type)
	{
		case TYPE_FLOAT:
		return internal_array_contains_float(value.f, array, length);
		case TYPE_DOUBLE:
		return internal_array_contains_double(value.d, array, length);
	}
}

/*
*	Internal function used to call type specific array_indexOf functions.
*/
static int internal_call_array_indexOf_special(ARRAY_PTR array,  generic_union_t value, size_t length, type_t elem_type)
{
	switch(elem_type)
	{
		case TYPE_FLOAT:
		return internal_array_indexOf_float(value.f, array, length);
		case TYPE_DOUBLE:
		return internal_array_indexOf_double(value.d, array, length);
	}
}

/*
*	Internal function used to call type specific array_lastIndexOf functions.
*/
static int internal_call_array_lastIndexOf_special(ARRAY_PTR array,  generic_union_t value, size_t length, type_t elem_type)
{
	switch(elem_type)
	{
		case TYPE_FLOAT:
		return internal_array_lastIndexOf_float(value.f, array, length);
		case TYPE_DOUBLE:
		return internal_array_lastIndexOf_double(value.d, array, length);
	}
}



#pragma endregion Type Specific Methods

/*
*	unsafe functions			
*
*	The "unsafe" functions use malloc internally, and return a pointer to the allocated array and must be freed when you are done using.
*/
#pragma region Unsafe Generic Methods

uint8_t** internal_array_selectMany_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, PREDICATE predicate, length_t* out_length);

uint8_t** internal_array_selectMany_vargs_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, PREDICATE_ARGS predicate, int arg_count, va_list ap, length_t* out_length);

ARRAY_PTR internal_array_insertRange_memory(ARRAY_PTR array, const ELEMENT_PTR range, int offset, size_t byte_count, size_t size,  element_size_t elem_size, bool free_old );
inline ARRAY_PTR internal_array_insert_memory(ARRAY_PTR array, const ELEMENT_PTR value, int offset,  size_t size,  element_size_t elem_size, bool free_old );
inline ARRAY_PTR internal_array_insert_generic(ARRAY_PTR array, generic_union_t value, int offset, size_t size,  element_size_t elem_size, bool free_old);

inline ARRAY_PTR internal_array_add_memory(ARRAY_PTR array, const ELEMENT_PTR value, size_t size,  element_size_t elem_size, bool free_old );
inline ARRAY_PTR internal_array_add_generic(ARRAY_PTR array, generic_union_t value, size_t size,  element_size_t elem_size, bool free_old);

inline ARRAY_PTR internal_array_prepend_memory(ARRAY_PTR array, const ELEMENT_PTR value, size_t size,  element_size_t elem_size, bool free_old );
inline ARRAY_PTR internal_array_prepend_generic(ARRAY_PTR array, generic_union_t value, size_t size,  element_size_t elem_size, bool free_old);


ARRAY_PTR internal_array_removeRange_memory(ARRAY_PTR array, int offset, size_t byte_count, size_t size,  element_size_t elem_size, bool free_old );
inline ARRAY_PTR internal_array_removeAt_memory(ARRAY_PTR array, int offset, size_t size,  element_size_t elem_size, bool free_old );

ARRAY_PTR internal_array_remove_memory(ARRAY_PTR array, const ELEMENT_PTR value, size_t size,  element_size_t elem_size, bool free_old, bool* out_removed );
inline ARRAY_PTR internal_array_remove_generic(ARRAY_PTR array, generic_union_t value, size_t size,  element_size_t elem_size, bool free_old, bool* out_removed);

ARRAY_PTR internal_array_removeAll_memory(ARRAY_PTR array, const ELEMENT_PTR value, size_t size,  element_size_t elem_size, bool free_old, unsigned int* out_count );
inline ARRAY_PTR internal_array_removeAll_generic(ARRAY_PTR array, generic_union_t value, size_t size,  element_size_t elem_size, bool free_old, unsigned int* out_count );

ARRAY_PTR internal_array_reverse_memory(ARRAY_PTR array, size_t size,  element_size_t elem_size, bool free_old);

//ARRAY_PTR internal_array_sort_memory(ARRAY_PTR array, size_t size,  element_size_t elem_size, COMPARISON comparison, bool free_old);

void internal_array_fill_memory(ARRAY_PTR array, ELEMENT_PTR value, size_t size, element_size_t elem_size);
GENOPTI_INLINE void internal_array_fill_generic(ARRAY_PTR array, generic_union_t value, size_t size, element_size_t elem_size);

/*
	Not Implemented at this time
*/


//
//ARRAY_PTR array_sort_(ARRAY_PTR array, size_t length, type_size_t elem_size, bool free_old);

#pragma endregion Unsafe Generic Methods

#pragma region Unsafe Conversion Methods



uint8_t* internal_array_convertTo_cast(ARRAY_PTR array, size_t length, type_sized_t oldType, type_sized_t newType, bool free_old);
uint8_t* internal_array_convertTo_memory(ARRAY_PTR array, size_t length, type_sized_t oldType, type_sized_t newType, bool free_old);

/*
*	Calls either convertTo_cast or convertTo_memory depending on GENETICC_OPTIMIZATION_LEVEL
*/
static inline uint8_t* __attribute__((always_inline)) internal_array_convertTo(ARRAY_PTR array, size_t length, type_sized_t oldType, type_sized_t newType, bool free_old)
{
	#warning come back and time these convert functions
	#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	return internal_array_convertTo_cast(array, length, oldType, newType, free_old);
	#else
	return internal_array_convertTo_memory(array, length, oldType, newType, free_old);
	#endif
}

uint8_t* internal_array_convertToInteger_memory(ARRAY_PTR array, size_t length, signed_type_size_t old_elem_size, signed_type_size_t new_elem_size, bool free_old );
uint8_t* internal_array_convertToIntegerFromFloat_memory(float* array, length_t length, signed_type_size_t new_elem_size, bool free_old );
uint8_t* internal_array_convertToIntegerFromDouble_memory(double* array, length_t length, signed_type_size_t new_elem_size, bool free_old );
float* internal_array_convertToFloatFromInteger_memory(int8_t* array, size_t length, type_sized_t old_elem_type, bool free_old );
double* internal_array_convertToDoubleFromInteger_memory(int8_t* array, size_t length, signed_type_size_t old_elem_size, bool free_old );
double* internal_array_convertToDoubleFromFloat_memory(float* array, length_t length, bool free_old );
float* internal_array_convertToFloatFromDouble_memory(double* array, length_t length, bool free_old );

#pragma endregion Unsafe Conversion Methods

#pragma region Helper Functions

/*
*	Returns the true index returned for macros that use "start".
*
*	/param		index	The index returned by the macro/function.
*	/param		start	The value that was passed to start.
*
*	/returns	int		The true index of the element.
*/
static int INTERNAL_ARRAY_GET_TRUE_INDEX(int index, size_t start)
{
	if(index != -1)
	return index + start;
	
	return index;
}

static void __attribute__((__always_inline__)) INTERNAL_ARRAY_FREE(ARRAY_PTR array, bool free_old)
{
	if(free_old)
	free(array);
}

/*
*	Gets a pointer to the element at the index.
*
*	/param	array		Pointer to the start of the array.
*	/param	index		Index of the value
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			Pointer to the value at the index, or null if it does not exist.
*/
static ELEMENT_PTR INTERNAL_ARRAY_GET_POINTER(const ARRAY_PTR array, int index, element_size_t elem_size)
{
	if(index < 0) return NULL;
	
	return array + (index * elem_size);
}

#pragma endregion Helper Functions


#pragma region DMA Functions
#if GENETICC_USE_DMA == true

bool internal_array_copy_dma(ARRAY_PTR array, ARRAY_PTR dest, size_t size, bool free_source);



#endif
#pragma endregion DMA Functions


#endif /* GENETICC_ARRAY_INTERNAL_FUNCTIONS_H_ */