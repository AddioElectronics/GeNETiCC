#ifndef LIST_GENETICC_INTERNAL_FUNCTIONS_H_
#define LIST_GENETICC_INTERNAL_FUNCTIONS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

#include "../../../geneticc.h"
//#include "../../../geneticc_defs.h"
#include "../list_geneticc_defs.h"
#include "../list_geneticc_config.h"
//#include "list_geneticc_defs.h"


#pragma region INTERNAL Functions

LIST_PTR internal_list_new( element_size_t elem_size, capacity_size_t capacity);
//Initialize "array" with size and capacity.
void internal_list_init(LIST_PTR out_list,  element_size_t elem_size, capacity_size_t capacity);

LIST_PTR internal_array_to_list(ARRAY_PTR array, size_t size,  element_size_t elem_size, size_t populated_size);

LIST_PTR internal_array_copyTo_list(ARRAY_PTR array, size_t size,  element_size_t elem_size, size_t populated_size);



void internal_array_init_list(ARRAY_PTR array, size_t size,  element_size_t elem_size, size_t populated_size, LIST_PTR out_list);

void internal_array_copy_init_list(ARRAY_PTR array, size_t size,  element_size_t elem_size, size_t populated_size, LIST_PTR out_list);

ARRAY_PTR internal_list_toArray(LIST_PTR list);

inline void internal_list_delete(LIST_PTR list);


bool internal_list_ensure_capacity(LIST_PTR list, capacity_size_t capacity);

bool internal_list_set_capacity(LIST_PTR list, capacity_count_t newCapacity);

GENOPTI_INLINE capacity_count_t internal_list_get_capacity(LIST_PTR list);

GENOPTI_INLINE size_t internal_list_get_size(LIST_PTR list);

GENOPTI_INLINE length_t internal_list_get_count(LIST_PTR list);

length_t internal_list_adjustHead(LIST_PTR list, int n, bool erase );

length_t internal_list_adjustTail(LIST_PTR list, int n, bool erase );

void internal_list_realign_array(LIST_PTR list);

GENOPTI_INLINE void internal_list_trimExcess(LIST_PTR list);

inline ARRAY_PTR internal_list_get_aligned_array(LIST_PTR list);

bool internal_list_set_memory(LIST_PTR list, const ELEMENT_PTR value, index_t index);

inline bool internal_list_set_generic(LIST_PTR list, generic_union_t value, index_t index);

ELEMENT_PTR internal_list_get(LIST_PTR list, index_t index);



GENOPTI_INLINE bool internal_list_exists(LIST_PTR list, PREDICATE predicate);

void internal_list_forEach(LIST_PTR list, ACTION action, byte_offset_t offset, size_t size);

bool internal_list_all(LIST_PTR list, PREDICATE predicate, byte_offset_t offset, size_t size);

bool internal_list_any(LIST_PTR list, PREDICATE predicate, byte_offset_t offset, size_t size);

GENOPTI_INLINE int internal_list_find_memory(LIST_PTR list, PREDICATE predicate);

GENOPTI_INLINE int internal_list_find_args_memory(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, ...);

int internal_list_find_vargs_memory(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, va_list ap);


#pragma endregion INTERNAL Functions



#pragma region Unsafe Generic Methods


LIST_PTR internal_list_findAll_indexes(LIST_PTR list, PREDICATE predicate, byte_offset_t offset, size_t size);
LIST_PTR internal_list_findAll_pointers(LIST_PTR list, PREDICATE predicate, byte_offset_t offset, size_t size);
LIST_PTR internal_list_findAll_values(LIST_PTR list, PREDICATE predicate, byte_offset_t offset, size_t size);

LIST_PTR internal_list_findAll_indexes_args(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, ...);
LIST_PTR internal_list_findAll_pointers_args(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, ...);
LIST_PTR internal_list_findAll_values_args(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, ...);

LIST_PTR internal_list_findAll_indexes_vargs(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, va_list ap);
LIST_PTR internal_list_findAll_pointers_vargs(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, va_list ap);
LIST_PTR internal_list_findAll_values_vargs(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, va_list ap);

LIST_PTR internal_list_getRange_memory(LIST_PTR list, byte_offset_t offset, size_t range_size);

bool internal_list_insertRange_memory(LIST_PTR list, const ARRAY_PTR range, byte_offset_t offset, size_t range_size,  element_size_t elem_size );
inline bool internal_list_insert_memory(LIST_PTR list, const ELEMENT_PTR value, byte_offset_t offset, element_size_t elem_size);
inline bool internal_list_insert_generic(LIST_PTR list, generic_union_t value, byte_offset_t offset,  element_size_t elem_size);

inline bool internal_list_addRange_memory(LIST_PTR list, const ARRAY_PTR range, size_t range_size, element_size_t elem_size );
inline bool internal_list_add_memory(LIST_PTR list, const ELEMENT_PTR value,  element_size_t elem_size );
inline bool internal_list_add_generic(LIST_PTR list, generic_union_t value,  element_size_t elem_size);

inline bool internal_list_prependRange_memory(LIST_PTR list, const ARRAY_PTR range, size_t range_size,  element_size_t elem_size );
inline bool internal_list_prepend_memory(LIST_PTR list, const ELEMENT_PTR value,  element_size_t elem_size );
inline bool internal_list_prepend_generic(LIST_PTR list, generic_union_t value,  element_size_t elem_size);

void internal_list_removeRange_memory(LIST_PTR list, byte_offset_t offset, size_t byte_count );
inline void internal_list_removeAt_memory(LIST_PTR list, byte_offset_t offset );

bool internal_list_remove_memory(LIST_PTR list , const ELEMENT_PTR value);
inline bool internal_list_remove_generic(LIST_PTR list, generic_union_t value);

uint32_t internal_list_removeAll_memory(LIST_PTR list, ELEMENT_PTR value);
inline uint32_t internal_list_removeAll_generic(LIST_PTR list, generic_union_t value );

bool internal_list_removeMatch_memory(LIST_PTR list, PREDICATE predicate );
uint32_t internal_list_removeAllMatching_memory(LIST_PTR list, PREDICATE predicate);

bool internal_list_removeMatch_vargs_memory(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, va_list ap );
uint32_t internal_list_removeAllMatching_vargs_memory(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, va_list ap );

GENOPTI_INLINE bool internal_list_removeMatch_args_memory(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, ... );
GENOPTI_INLINE uint32_t internal_list_removeAllMatching_args_memory(LIST_PTR list, PREDICATE_ARGS predicate, int arg_count, ... );

inline void internal_list_reverse_memory(LIST_PTR list);

//void internal_list_sort_memory(LIST_PTR list, COMPARISON comparison);





#pragma endregion Unsafe Generic Methods

#pragma region Unsafe Conversion Methods



LIST_PTR internal_list_convertTo_cast(LIST_PTR list, type_sized_t oldType, type_sized_t newType);
LIST_PTR internal_list_convertTo_memory(LIST_PTR list, type_sized_t oldType, type_sized_t newType);

/*
*	Calls either convertTo_cast or convertTo_memory depending on GENETICC_OPTIMIZATION_LEVEL
*/
static inline LIST_PTR __attribute__((always_inline)) internal_list_convertTo(LIST_PTR list, type_sized_t oldType, type_sized_t newType)
{
	#warning come back and time these convert functions
	#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	return internal_list_convertTo_cast(list, oldType, newType);
	#else
	return internal_list_convertTo_memory(list, oldType, newType);
	#endif
}

#pragma endregion Unsafe Conversion Methods

#pragma region Helper Methods

static element_size_t __attribute__((__always_inline__)) INTERNAL_LIST_GET_ELEM_SIZE(LIST_PTR list)
{
	return list->elem_size;
}

//static element_size_t* __attribute__((__always_inline__)) INTERNAL_LIST_HEAD(LIST_PTR list)
//{
	//return &list->head;
//}
//
//static element_size_t* __attribute__((__always_inline__)) INTERNAL_LIST_TAIL(LIST_PTR list)
//{
	//return &list->tail;
//}


#pragma endregion Helper Methods

#pragma region DMA Functions
#if GENETICC_USE_DMA == true

LIST_PTR internal_array_dma_copyTo_list(ARRAY_PTR array, size_t size,  element_size_t elem_size, size_t populated_size);

bool internal_array_dma_copy_init_list(ARRAY_PTR array, size_t size,  element_size_t elem_size, size_t populated_size, LIST_PTR out_list);

LIST_PTR internal_list_dma_getRange_memory(LIST_PTR list, byte_offset_t offset, size_t range_size);

bool internal_list_dma_insertRange_memory(LIST_PTR list, const ARRAY_PTR range, byte_offset_t offset, size_t range_size );
inline bool internal_list_dma_insert_memory(LIST_PTR list, const ELEMENT_PTR value, byte_offset_t offset);
inline bool internal_list_dma_insert_generic(LIST_PTR list, generic_union_t value, byte_offset_t offset);

inline bool internal_list_dma_addRange_memory(LIST_PTR list, const ARRAY_PTR range, size_t range_size);
inline bool internal_list_dma_add_memory(LIST_PTR list, const ELEMENT_PTR value );
inline bool internal_list_dma_add_generic(LIST_PTR list, generic_union_t value);

inline bool internal_list_dma_prependRange_memory(LIST_PTR list, const ARRAY_PTR range, size_t range_size );
inline bool internal_list_dma_prepend_memory(LIST_PTR list, const ELEMENT_PTR value);
inline bool internal_list_dma_prepend_generic(LIST_PTR list, generic_union_t value);

bool internal_list_dma_removeRange_memory(LIST_PTR list, byte_offset_t offset, size_t byte_count );
inline bool internal_list_dma_removeAt_memory(LIST_PTR list, byte_offset_t offset );

bool internal_list_dma_remove_memory(LIST_PTR list, const ELEMENT_PTR value, bool* out_removed);
inline bool internal_list_dma_remove_generic(LIST_PTR list, generic_union_t value, bool* out_removed);

bool internal_list_dma_removeAll_memory(LIST_PTR list, const ELEMENT_PTR value, uint32_t* out_remove_count);
inline bool internal_list_dma_removeAll_generic(LIST_PTR list, generic_union_t value, uint32_t* out_remove_count );

bool internal_list_dma_removeMatch_memory(LIST_PTR list, PREDICATE predicate, bool* out_removed );
bool internal_list_dma_removeAllMatching_memory(LIST_PTR list, PREDICATE predicate, uint32_t* out_remove_count);

bool internal_list_dma_removeMatch_vargs_memory(LIST_PTR list, PREDICATE_ARGS predicate, bool* out_removed, int arg_count, va_list ap );
bool internal_list_dma_removeAllMatching_vargs_memory(LIST_PTR list, PREDICATE_ARGS predicate, uint32_t* out_remove_count, int arg_count, va_list ap);

GENOPTI_INLINE bool internal_list_dma_removeMatch_args_memory(LIST_PTR list, PREDICATE_ARGS predicate, bool* out_removed, int arg_count, ... );
GENOPTI_INLINE bool internal_list_dma_removeAllMatching_args_memory(LIST_PTR list, PREDICATE_ARGS predicate, uint32_t* out_remove_count, int arg_count, ...);

#endif
#pragma endregion DMA Functions


#endif /* LIST_GENETICC_INTERNAL_FUNCTIONS_H_ */