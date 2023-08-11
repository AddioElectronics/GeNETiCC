
#ifndef GENETICC_MEMORY_H_
#define GENETICC_MEMORY_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "Addio/GeNETiCC/geneticc.h"
#include "Addio/GeNETiCC/dma/geneticc_dma.h"
#include "Addio/GeNETiCC/dma/geneticc_dma_transfer_manager.h"

//static int zero = 0;

/*
*	Should standard memory functions be replaced with DMA?
*
*	*They will run synchronously,
*	*if DMA is slower than the CPU for your system, this is not recommended.
*/
#define GENETICC_FORCE_SYNCHRONOUS_DMA	(GENETICC_USE_DMA && false /* <- Set this bool*/)

#define memcpyovrlp(dest, src, size) memmove(dest, src, size)

#warning reminder to compare memmove to memcpyovrlp
#if !defined(memcpyovrlp)
void memcpyovrlp(uint8_t* dest, uint8_t* src, size_t size);
#endif

//#if GENMEM_USE_DMA == true Allow other functions to use these instead of removing them.

#pragma region DMA Synchronous Functions

void geneticc_memcpy_dma_sync(void* dest, void* src, size_t n);

void geneticc_memcpyovrlp_dma_sync(void* dest, void* src, size_t n);

void geneticc_memset_dma_sync(void* dest, generic_union_t c, size_t n);


#pragma endregion DMA Synchronous Functions

#pragma region DMA Asynchronous Functions

void geneticc_memcpy_dma_async(void* dest, void* src, size_t n, bool free_source);

void geneticc_memcpyovrlp_dma_async(void* dest, void* src, size_t n, bool free_source);

void geneticc_memset_dma_async(void* dest, generic_union_t c, size_t n);

#pragma endregion DMA Asynchronous Functions
//#endif

#if GENETICC_FORCE_SYNCHRONOUS_DMA == true
#define  geneticc_memcpy(dest, src, size)		geneticc_memcpy_dma_sync(dest, src, size, false)
#define  geneticc_memcpyovrlp(dest, src, size)	geneticc_memcpyovrlp_dma_sync(dest, src, size, false)
#define  geneticc_memset(dest, c, size)			geneticc_memset_dma_sync(dest, c, size)
#else
#define  geneticc_memcpy(dest, src, size)		memcpy(dest, src, size)
#define  geneticc_memcpyovrlp(dest, src, size)	memcpyovrlp(dest, src, size)
#define  geneticc_memset(dest, c, size)			memset(dest, c, size)
#endif


#warning untested
/*
*	geneticc_set_memory
*
*	Sets a range(array) of values into a potentially different sized array, while keeping the same values.
*
*	Ex. if array = short {0x11aa, 0x22bb, 0x33cc}, and range = char {0x11, 0x22, 0x33}, then result = {0x0011, 0x0022, 0x0033}
*
*	/param		array				Pointer to the first value to start setting.
*	/param		array_elem_size		How many bytes is 1 element in array?
*	/param		range				Pointer to eh first value which will be used to set.
*	/param		range_elem_size		How many bytes is 1 element in range?
*	/param		count				How many values are in range?
*/
static void geneticc_set_values_memory(ARRAY_PTR array, type_size_t array_elem_size, ARRAY_PTR range, type_size_t range_elem_size, length_t count)
{
	uint32_t byte_count = count * array_elem_size;
	uint8_t copysize = array_elem_size > range_elem_size ? range_elem_size : array_elem_size;
	uint8_t setsize = array_elem_size > range_elem_size ? array_elem_size - range_elem_size : 0;
	
	if(array_elem_size != range_elem_size)
	{
		//enumerator_t enumerator;
		
		//enumerator_init(range, count * range_elem_size, range_elem_size, &enumerator);
		
		for(int i = 0, r = 0 ; i < byte_count; i+= array_elem_size, r += range_elem_size)
		{
			geneticc_memcpyovrlp(array + i, range + r, copysize);
			
			if(setsize)
			geneticc_memset(array + i + range_elem_size, 0x00, setsize);
			//generic_union_t val = enumerator_get_next(&enumerator, false, 0, 0);
			//memcpy( (array + i), &val, range_elem_size);
		}
	}
	else
	{
		//Elem size the same, we can do a regular copy.
		geneticc_memcpyovrlp(array, range, byte_count);
	}
}




#endif /* GENETICC_MEMORY_H_ */
