#ifndef ENUMERABLE_GENETICC_INTERNAL_FUNCTIONS_H_
#define ENUMERABLE_GENETICC_INTERNAL_FUNCTIONS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

#include "Addio/GeNETiCC/geneticc.h"
#include "Addio/GeNETiCC/geneticc_defs.h"
#include "Addio/GeNETiCC/Collections/enumerable/enumerable_geneticc_defs.h"
#include "Addio/GeNETiCC/Collections/enumerable/enumerable_geneticc_config.h"


#pragma region Generic Methods

inline bool internal_enumerable_contains_memory(const ENUMERABLE_PTR enumerable, const ELEMENT_PTR value, size_t size,  element_size_t elem_size );
inline bool internal_enumerable_contains_generic(const ENUMERABLE_PTR enumerable, generic_union_t value, size_t size,  element_size_t elem_size);

inline bool internal_enumerable_exists(const ENUMERABLE_PTR enumerable, size_t length,  element_size_t elem_size, PREDICATE predicate);

int internal_enumerable_indexOf_memory(const ENUMERABLE_PTR enumerable, const ELEMENT_PTR value, size_t size,  element_size_t elem_size);
inline int internal_enumerable_indexOf_generic(const ENUMERABLE_PTR enumerable, generic_union_t value,  size_t size,  element_size_t elem_size);

int internal_enumerable_lastIndexOf_memory(const ENUMERABLE_PTR enumerable, const ELEMENT_PTR value, size_t size,  element_size_t elem_size);
inline int internal_enumerable_lastIndexOf_generic(const ENUMERABLE_PTR enumerable, generic_union_t value, size_t size,  element_size_t elem_size);

index_t internal_enumerable_max_memory(const ENUMERABLE_PTR enumerable, size_t size,  element_size_t elem_size, bool isSigned);
index_t internal_enumerable_maxf_memory(const double* enumerable, size_t size,  element_size_t elem_size);

index_t internal_enumerable_min_memory(const ENUMERABLE_PTR enumerable, size_t size,  element_size_t elem_size, bool isSigned);
index_t internal_enumerable_minf_memory(const double* enumerable, size_t size,  element_size_t elem_size);

generic_union_t internal_enumerable_sum_memory(const ENUMERABLE_PTR enumerable, size_t size,  element_size_t elem_size, bool isSigned);
double internal_enumerable_sumf_memory(const double* enumerable, size_t length,  element_size_t elem_size);

generic_union_t internal_enumerable_average_memory(const ENUMERABLE_PTR enumerable, size_t size,  element_size_t elem_size, bool isSigned);
GENOPTI_INLINE double internal_enumerable_averagef_memory(const double* enumerable, size_t size,  element_size_t elem_size);

unsigned int internal_enumerable_valueCount_memory(const ENUMERABLE_PTR enumerable,const ELEMENT_PTR value, size_t size,  element_size_t elem_size);
inline unsigned int internal_enumerable_valueCount_generic(const ENUMERABLE_PTR enumerable, generic_union_t value, size_t size,  element_size_t elem_size);

int internal_enumerable_select_memory(const ENUMERABLE_PTR enumerable, size_t size,  element_size_t elem_size, PREDICATE predicate);

#warning remember to create macros for this.
GENOPTI_INLINE int internal_enumerable_select_args_memory(const ENUMERABLE_PTR enumerable, size_t size,  element_size_t elem_size, PREDICATE_ARGS predicate, int arg_count, ...);
int internal_enumerable_select_vargs_memory(const ENUMERABLE_PTR enumerable, size_t size,  element_size_t elem_size, PREDICATE_ARGS predicate, int arg_count, va_list ap);

#pragma endregion Generic Methods

#pragma region Type Specific Methods - Generally faster than generic functions, but will increase code size


inline bool internal_enumerable_contains_float(const float value, const float* enumerable, length_t length);
inline bool internal_enumerable_contains_double(const double value, const double* enumerable, length_t length);

int internal_enumerable_indexOf_float(const float value, const float* enumerable, length_t length);
int internal_enumerable_indexOf_double(const double value, const double* enumerable, length_t length);

int internal_enumerable_lastIndexOf_float(const float value, const float* enumerable, length_t length);
int internal_enumerable_lastIndexOf_double(const double value, const double* enumerable, length_t length);

/*
*	To include these type specific functions in the macros, we need an intermediate function as pointers complain when they try to be cast to a floating point value.
*	Even though in macros, the pointers will never call the floating point functions, anything in _Generic has to be compatible with the value that is calling.
*	That is what the internal_call_enumerable_..._special methods are for.
*/

/*
*	Internal function used to call type specific enumerable_contains functions.
*/
static int internal_call_enumerable_contains_special(ENUMERABLE_PTR enumerable,  generic_union_t value, size_t length, type_t elem_type)
{
	switch(elem_type)
	{
		case TYPE_FLOAT:
		return internal_enumerable_contains_float(value.f, enumerable, length);
		case TYPE_DOUBLE:
		return internal_enumerable_contains_double(value.d, enumerable, length);
	}
}

/*
*	Internal function used to call type specific enumerable_indexOf functions.
*/
static int internal_call_enumerable_indexOf_special(ENUMERABLE_PTR enumerable,  generic_union_t value, size_t length, type_t elem_type)
{
	switch(elem_type)
	{
		case TYPE_FLOAT:
		return internal_enumerable_indexOf_float(value.f, enumerable, length);
		case TYPE_DOUBLE:
		return internal_enumerable_indexOf_double(value.d, enumerable, length);
	}
}

/*
*	Internal function used to call type specific enumerable_lastIndexOf functions.
*/
static int internal_call_enumerable_lastIndexOf_special(ENUMERABLE_PTR enumerable,  generic_union_t value, size_t length, type_t elem_type)
{
	switch(elem_type)
	{
		case TYPE_FLOAT:
		return internal_enumerable_lastIndexOf_float(value.f, enumerable, length);
		case TYPE_DOUBLE:
		return internal_enumerable_lastIndexOf_double(value.d, enumerable, length);
	}
}



#pragma endregion Type Specific Methods

/*
*	unsafe functions			
*
*	The "unsafe" functions use malloc internally, and return a pointer to the allocated enumerable and must be freed when you are done using.
*/
#pragma region Unsafe Generic Methods

uint8_t** internal_enumerable_selectMany_memory(const ENUMERABLE_PTR enumerable, size_t size,  element_size_t elem_size, PREDICATE predicate, length_t* out_length);

uint8_t** internal_enumerable_selectMany_vargs_memory(const ENUMERABLE_PTR enumerable, size_t size,  element_size_t elem_size, PREDICATE_ARGS predicate, int arg_count, va_list ap, length_t* out_length);

ENUMERABLE_PTR internal_enumerable_insertRange_memory(ENUMERABLE_PTR enumerable, const ELEMENT_PTR range, int offset, size_t byte_count, size_t size,  element_size_t elem_size, bool free_old );
inline ENUMERABLE_PTR internal_enumerable_insert_memory(ENUMERABLE_PTR enumerable, const ELEMENT_PTR value, int offset,  size_t size,  element_size_t elem_size, bool free_old );
inline ENUMERABLE_PTR internal_enumerable_insert_generic(ENUMERABLE_PTR enumerable, generic_union_t value, int offset, size_t size,  element_size_t elem_size, bool free_old);

inline ENUMERABLE_PTR internal_enumerable_add_memory(ENUMERABLE_PTR enumerable, const ELEMENT_PTR value, size_t size,  element_size_t elem_size, bool free_old );
inline ENUMERABLE_PTR internal_enumerable_add_generic(ENUMERABLE_PTR enumerable, generic_union_t value, size_t size,  element_size_t elem_size, bool free_old);

inline ENUMERABLE_PTR internal_enumerable_prepend_memory(ENUMERABLE_PTR enumerable, const ELEMENT_PTR value, size_t size,  element_size_t elem_size, bool free_old );
inline ENUMERABLE_PTR internal_enumerable_prepend_generic(ENUMERABLE_PTR enumerable, generic_union_t value, size_t size,  element_size_t elem_size, bool free_old);


ENUMERABLE_PTR internal_enumerable_removeRange_memory(ENUMERABLE_PTR enumerable, int offset, size_t byte_count, size_t size,  element_size_t elem_size, bool free_old );
inline ENUMERABLE_PTR internal_enumerable_removeAt_memory(ENUMERABLE_PTR enumerable, int offset, size_t size,  element_size_t elem_size, bool free_old );

ENUMERABLE_PTR internal_enumerable_remove_memory(ENUMERABLE_PTR enumerable, const ELEMENT_PTR value, size_t size,  element_size_t elem_size, bool free_old, bool* out_removed );
inline ENUMERABLE_PTR internal_enumerable_remove_generic(ENUMERABLE_PTR enumerable, generic_union_t value, size_t size,  element_size_t elem_size, bool free_old, bool* out_removed);

ENUMERABLE_PTR internal_enumerable_removeAll_memory(ENUMERABLE_PTR enumerable, const ELEMENT_PTR value, size_t size,  element_size_t elem_size, bool free_old, unsigned int* out_count );
inline ENUMERABLE_PTR internal_enumerable_removeAll_generic(ENUMERABLE_PTR enumerable, generic_union_t value, size_t size,  element_size_t elem_size, bool free_old, unsigned int* out_count );

ENUMERABLE_PTR internal_enumerable_reverse_memory(ENUMERABLE_PTR enumerable, size_t size,  element_size_t elem_size, bool free_old);

//ENUMERABLE_PTR internal_enumerable_sort_memory(ENUMERABLE_PTR enumerable, size_t size,  element_size_t elem_size, COMPARISON comparison, bool free_old);

void internal_enumerable_fill_memory(ENUMERABLE_PTR enumerable, ELEMENT_PTR value, size_t size, element_size_t elem_size);
inline void internal_enumerable_fill_generic(ENUMERABLE_PTR enumerable, generic_union_t value, size_t size, element_size_t elem_size);

/*
	Not Implemented at this time
*/


//
//ENUMERABLE_PTR enumerable_sort_(ENUMERABLE_PTR enumerable, size_t length, type_size_t elem_size, bool free_old);

#pragma endregion Unsafe Generic Methods

#pragma region Unsafe Conversion Methods



uint8_t* internal_enumerable_convertTo_cast(ENUMERABLE_PTR enumerable, size_t length, type_sized_t oldType, type_sized_t newType, bool free_old);
uint8_t* internal_enumerable_convertTo_memory(ENUMERABLE_PTR enumerable, size_t length, type_sized_t oldType, type_sized_t newType, bool free_old);

/*
*	Calls either convertTo_cast or convertTo_memory depending on GENETICC_OPTIMIZATION_LEVEL
*/
static inline uint8_t* __attribute__((__always_inline__)) internal_enumerable_convertTo(ENUMERABLE_PTR enumerable, size_t length, type_sized_t oldType, type_sized_t newType, bool free_old)
{
	#warning come back and time these convert functions
	#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	return internal_enumerable_convertTo_cast(enumerable, length, oldType, newType, free_old);
	#else
	return internal_enumerable_convertTo_memory(enumerable, length, oldType, newType, free_old);
	#endif
}

uint8_t* internal_enumerable_convertToInteger_memory(ENUMERABLE_PTR enumerable, size_t length, signed_type_size_t old_elem_size, signed_type_size_t new_elem_size, bool free_old );
uint8_t* internal_enumerable_convertToIntegerFromFloat_memory(float* enumerable, length_t length, signed_type_size_t new_elem_size, bool free_old );
uint8_t* internal_enumerable_convertToIntegerFromDouble_memory(double* enumerable, length_t length, signed_type_size_t new_elem_size, bool free_old );
float* internal_enumerable_convertToFloatFromInteger_memory(int8_t* enumerable, size_t length, type_sized_t old_elem_type, bool free_old );
double* internal_enumerable_convertToDoubleFromInteger_memory(int8_t* enumerable, size_t length, signed_type_size_t old_elem_size, bool free_old );
double* internal_enumerable_convertToDoubleFromFloat_memory(float* enumerable, length_t length, bool free_old );
float* internal_enumerable_convertToFloatFromDouble_memory(double* enumerable, length_t length, bool free_old );

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
static int INTERNAL_ENUMERABLE_GET_TRUE_INDEX(int index, size_t start)
{
	if(index != -1)
	return index + start;
	
	return index;
}

static void __attribute__((__always_inline__)) INTERNAL_ENUMERABLE_FREE(ENUMERABLE_PTR enumerable, bool free_old)
{
	if(free_old)
	free(enumerable);
}

/*
*	Gets a pointer to the element at the index.
*
*	/param	enumerable		Pointer to the start of the enumerable.
*	/param	index		Index of the value
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			Pointer to the value at the index, or null if it does not exist.
*/
static ELEMENT_PTR INTERNAL_ENUMERABLE_GET_POINTER(const ENUMERABLE_PTR enumerable, int index, element_size_t elem_size)
{
	if(index < 0) return NULL;
	
	return enumerable + (index * elem_size);
}

#pragma endregion Helper Functions


#pragma region DMA Functions
#if GENETICC_USE_DMA == true

bool internal_enumerable_copy_dma(ENUMERABLE_PTR enumerable, ENUMERABLE_PTR dest, size_t size, bool free_source);



#endif
#pragma endregion DMA Functions


#endif /* ENUMERABLE_GENETICC_INTERNAL_FUNCTIONS_H_ */