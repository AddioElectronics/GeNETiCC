#include "geneticc_memory.h"

//#include "enumerator/enumerator_geneticc.h"
#include "geneticc.h"

#pragma region Variables

extern volatile geneticc_dma_control_reg geneticc_dma_control;

//extern size_t GENETICC_DMA_MIN_SIZE;
//extern size_t GENETICC_DMA_SYNC_MIN_SIZE;

#pragma endregion Variables


#if !defined(memcpyovrlp)

static void memcpyovrlp_32(uint32_t* dest, uint32_t* src, size_t size);
//static void memcpyovrlp_16(uint16_t* dest, uint16_t* src, size_t size);
static void memcpyovrlp_8(uint8_t* dest, uint8_t* src, size_t size);

/*
*	Copies memory from source to destination from back to front.
*/
void memcpyovrlp(uint8_t* dest, uint8_t* src, size_t size)
{
	uint8_t a4 = size % 4;
	uint8_t a2 = a4 % 2;
	
	if(size > 4)
	{
		memcpyovrlp_32(dest, src, size - a4);
		memcpyovrlp_8(dest + (size - a4), src + (size - a4), a4);
	}
	else
	{
		memcpyovrlp_8(dest, src, size);
	}	
}

static void memcpyovrlp_32(uint32_t* dest, uint32_t* src, size_t size)
{
	size /=  4;
	for(int i = size - 1; i >= 0; i--)
	{
		dest[i] = src[i];
	}
	
}

//static void memcpyovrlp_16(uint16_t* dest, uint16_t* src, size_t size)
//{
	//
	//
//}

static void memcpyovrlp_8(uint8_t* dest, uint8_t* src, size_t size)
{
	for(int i = size - 1; i >= 0; i--)
	{
		dest[i] = src[i];
	}
}

#endif


#pragma region DMA Synchronous Functions

void geneticc_memcpy_dma_sync(void* dest, void* src, size_t n)
{
	if(geneticc_dma_get_transaction_count(n, GENETICC_DMA_MAX_BEATSIZE) >= GENETICC_DMA_SYNC_MIN_TRANSACTION_COUNT && geneticc_dma_get_enabled())
	{
		geneticc_dma_subtransfer_t* transfer = geneticc_dma_create_subtransfer(dest, src, n, NULL, false);
		//geneticc_dma_register_subtransfer(dest, src, n, NULL, &transfer, false, BEATSIZE_MAX);
		
		//transfer.dest = dest;
		//transfer.src = src;
		//transfer.size = n;
		//transfer.flags = geneticc_dma_empty_subtransfer_flags;
		//transfer.flags.beatsize = geneticc_dma_control.beatsize;
		
		internal_geneticc_dma_start_transfer_sync(&transfer);
		internal_geneticc_dma_delete_subtransfer(transfer);
	}
	else
	{
		memcpy(dest, src, n);
	}
}

void geneticc_memcpyovrlp_dma_sync(void* dest, void* src, size_t n)
{
	if(geneticc_dma_get_transaction_count(n, GENETICC_DMA_MAX_BEATSIZE) < GENETICC_DMA_SYNC_MIN_TRANSACTION_COUNT || !geneticc_dma_get_enabled())
		goto StandardCopy;
	
	if(dest > src && dest - src > n)
	{
		//No chance of overlap, use dma.
		geneticc_memcpy_dma_sync(dest, src, n);
		return;
	}
	else //Overlapping
	{
	
		size_t non_overlapping = (dest - src - geneticc_dma_control.beatsize);
		size_t overlapping = n - non_overlapping;
		
		//Enough bytes not overlapping to use DMA
		if(geneticc_dma_get_transaction_count(non_overlapping, GENETICC_DMA_MAX_BEATSIZE) >= GENETICC_DMA_SYNC_MIN_TRANSACTION_COUNT)
		{
			//Copy overlapping bytes standard way.
			memcpyovrlp(dest + non_overlapping, src + non_overlapping, overlapping);
			
			//Copy non overlapping with DMA
			geneticc_memcpy_dma_sync(dest, src, non_overlapping);
			
			return;
		}
		else
		{
			goto StandardCopy;
		}
	}
	
	StandardCopy:
	//Not worth using DMA, use regular way.
	memcpyovrlp(dest, src, n);
}

void geneticc_memset_dma_sync(void* dest, generic_union_t c, size_t n)
{
	
	if(geneticc_dma_get_transaction_count(n, GENETICC_DMA_MAX_BEATSIZE) >= GENETICC_DMA_SYNC_MIN_TRANSACTION_COUNT && geneticc_dma_get_enabled())
	{
		generic_union_t* s = malloc(GENETICC_DMA_MAX_BEATSIZE);
		*s = c;
		size_t leftovers = n;
		size_t transfersize = 0;
		transfersize = geneticc_dma_get_transfer_size(0, n, &leftovers);
		
		geneticc_dma_subtransfer_t* transfer = geneticc_dma_create_subtransfer(dest, s, transfersize, NULL, NULL);
		transfer->flags.no_incsrc = true;
		transfer->flags.freesrc = true;
		
		if(leftovers > 0)
		memset(((uint8_t*)dest) + transfersize, c.i, leftovers);
		
		//geneticc_dma_subtransfer_t transfer;
		//
		//
		//transfer.dest = dest;
		//transfer.src = &s;
		//transfer.size = n;
		//transfer.flags.no_incsrc = true;
		
		internal_geneticc_dma_start_transfer_sync(&transfer);
		internal_geneticc_dma_delete_subtransfer(transfer);
	}
	else
	{
		memset(dest, c.i, n);
	}
}


#pragma endregion DMA Synchronous Functions

#pragma region DMA Asynchronous Functions

void geneticc_memcpy_dma_async(void* dest, void* src, size_t n, bool free_source)
{
	if(geneticc_dma_size_ok(n, GENETICC_DMA_MAX_BEATSIZE) && geneticc_dma_get_enabled())
	{
		geneticc_dma_transfer_t* transfer = geneticc_dma_create_transfer(dest, src, n, free_source);
		geneticc_dma_subtransfer_t* subtransfer = geneticc_dma_create_subtransfer(dest, src, n, transfer, false);
		subtransfer->flags.freesrc = free_source;

		if(!geneticc_dma_register_transfer(transfer))
		{
			geneticc_dma_delete_transfer(transfer);
			goto StandardCopy;
		}
	}
	else
	{
		StandardCopy:
		memcpy(dest, src, n);
	}
}

void geneticc_memcpyovrlp_dma_async(void* dest, void* src, size_t n, bool free_source)
{
	
	if(geneticc_dma_size_ok(n, GENETICC_DMA_MAX_BEATSIZE) || !geneticc_dma_get_enabled())
		goto StandardCopy;
	
	if(dest > src && dest - src > n)
	{
		//No chance of overlap, use dma.
		geneticc_memcpy_dma_async(dest, src, n, free_source);
		return;
	}
	else //Overlapping
	{
		
		size_t non_overlapping = (dest - src - geneticc_dma_control.beatsize);
		size_t overlapping = n - non_overlapping;
		
		//Enough bytes not overlapping to use DMA
		if(geneticc_dma_get_transaction_count(non_overlapping, GENETICC_DMA_MAX_BEATSIZE) >= GENETICC_DMA_SYNC_MIN_TRANSACTION_COUNT)
		{
			//Copy overlapping bytes standard way.
			memcpyovrlp(dest + non_overlapping, src + non_overlapping, overlapping);
			
			//Copy non overlapping with DMA
			geneticc_memcpy_dma_async(dest, src, non_overlapping, free_source);
			return;
		}
		else
		{
			goto StandardCopy;
		}
	}
	
	StandardCopy:
	//Not worth using DMA, use regular way.
	memcpyovrlp(dest, src, n);
}

void geneticc_memset_dma_async(void* dest, generic_union_t c, size_t n)
{
	
	if(geneticc_dma_size_ok(n, GENETICC_DMA_MAX_BEATSIZE) && geneticc_dma_get_enabled())
	{
		generic_union_t* s = malloc(GENETICC_DMA_MAX_BEATSIZE);
		*s = c;
		size_t leftovers = n;
		size_t transfersize = 0;
		transfersize = geneticc_dma_get_transfer_size(0, n, &leftovers);
		
		geneticc_dma_transfer_t* transfer = geneticc_dma_create_transfer(dest, s, n, true);
		
		geneticc_dma_subtransfer_flags_t flags = geneticc_dma_empty_subtransfer_flags;
		flags.no_incsrc = true;
		
		geneticc_dma_subtransfer_t* subtransfer = geneticc_dma_create_subtransfer(dest, s, transfersize, transfer, &flags);
		//subtransfer->flags.no_incsrc = true;

		
		if(leftovers > 0)
		{
			memset(((uint8_t*)dest) + transfersize, c.i, leftovers);
		}
		else if(subtransfer->flags.overlap)	//If overlap is true, the internal DMA functions will use memmove.
		{
			geneticc_dma_delete_transfer(transfer);
			free(s);
			goto StandardSet;
		}
		
		//geneticc_dma_subtransfer_t transfer;
		//
		//
		//transfer.dest = dest;
		//transfer.src = &s;
		//transfer.size = n;
		//transfer.flags.no_incsrc = true;
		
		if(!geneticc_dma_register_transfer(transfer))
		{
			geneticc_dma_delete_transfer(transfer);
			free(s);
			goto StandardSet;
		}
	}
	else
	{
		StandardSet:
		memset(dest, c.i, n);
	}
}

#pragma endregion DMA Asynchronous Functions