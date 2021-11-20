#include "geneticc_dma.h"

#include "../standard/memory/geneticc_memory.h"
#include "../collections/list/list_geneticc.h"

#pragma region Variables

volatile geneticc_dma_control_reg geneticc_dma_control;

volatile list_t	geneticc_dma_transfer_list;	//list of geneticc_dma_transfer_t

struct geneticc_dma_user_callbacks geneticc_dma_user_callbacks;

//extern list_t active_transfers;

#pragma endregion Variables


#pragma region Functions
void geneticc_dma_init()
{	
	geneticc_dma_control.beatsize = 0;
	geneticc_dma_control.enabled = 0;
	geneticc_dma_control.initialized = 0;
	geneticc_dma_control.sync_trans = 0;
	geneticc_dma_control.sync_waiting = 0;
	geneticc_dma_control.transfer_count = 0;
	geneticc_dma_control.transferring = 0;
	geneticc_dma_set_beatsize(BEATSIZE_MAX);
	
	List_Init(&geneticc_dma_transfer_list, sizeof(geneticc_dma_transfer_t*));
	
	//List_Init(&active_transfers, sizeof(geneticc_dma_active_transfer_t*));	//Will be used when multi-channel support is added.
	
	geneticc_dma_user_callbacks.transfer_done		= NULL;
	geneticc_dma_user_callbacks.subtransfer_done	= NULL;
	geneticc_dma_user_callbacks.error				= NULL;
	
	geneticc_dma_system_init();	
	
	internal_geneticc_dma_register_system_callbacks();
	
	GENETICC_DMA_ENABLE_INTERRUPT;
	
	geneticc_dma_control.initialized = true;
	
	geneticc_dma_enable();
}

void  __attribute__((weak, __always_inline__)) geneticc_dma_enable() 
{
	geneticc_dma_control.enabled = true;
	
	//geneticc_dma_enabled = true;
	GENETICC_DMA_ENABLE_INTERRUPT;
}

void  __attribute__((weak, __always_inline__)) geneticc_dma_disable()
{
	geneticc_dma_control.enabled = false;
	//geneticc_dma_enabled = false;
	GENETICC_DMA_DISABLE_INTERRUPT;
}


bool __attribute__((__always_inline__)) geneticc_dma_get_enabled()
{
	return geneticc_dma_control.enabled == true;
}

size_t __attribute__((__always_inline__)) geneticc_dma_get_min_size()
{
	return geneticc_dma_control.beatsize * GENETICC_DMA_MIN_TRANSACTION_COUNT;
}

/*
*	Returns how many DMA transactions it will take to send a number of bytes using a specific beatsize.
*
*	/param	size		How many bytes are being sent.
*	/param	beatsize	How many bytes per transaction. 0 = max for the supported system.
*/
uint32_t GENOPTI_ATT_FORCE_INLINE geneticc_dma_get_transaction_count(size_t size, geneticc_dma_beatsize_t beatsize)
{
	if(beatsize == 0)
	beatsize = GENETICC_DMA_MAX_BEATSIZE;
	
	return size / beatsize;
}



/*
*
*/
bool GENOPTI_ATT_FORCE_INLINE geneticc_dma_size_ok(size_t size, geneticc_dma_beatsize_t beatsize)
{
	return geneticc_dma_get_transaction_count(size, beatsize) > GENETICC_DMA_MIN_TRANSACTION_COUNT;
}

uint8_t geneticc_dma_beatsize_enum_to_mask(geneticc_dma_beatsize_t beatsize)
{
	switch(beatsize)
	{
		#if defined(GENETICC_DMA_BEATSIZE_BYTE_MASK)
		case BEATSIZE_BYTE:		return GENETICC_DMA_BEATSIZE_BYTE_MASK;
		#endif
		#if defined(GENETICC_DMA_BEATSIZE_WORD_MASK)
		case BEATSIZE_WORD:		return GENETICC_DMA_BEATSIZE_WORD_MASK;
		#endif
		#if defined(GENETICC_DMA_BEATSIZE_DWORD_MASK)
		case BEATSIZE_DWORD:	return GENETICC_DMA_BEATSIZE_DWORD_MASK;
		#endif
		#if defined(GENETICC_DMA_BEATSIZE_QWORD_MASK)
		case BEATSIZE_QWORD:	return GENETICC_DMA_BEATSIZE_QWORD_MASK;
		#endif
		default: ASSERT(false); break;
	}
}


/*
*	set_beatsize
*
*	Reconfigures the library to use a new beatsize, as well as calls a macro which will reconfigures the DMA hardware registers.
*
*	/param	beatsize	The new beatsize to use.
*/
void __attribute__((__always_inline__)) geneticc_dma_set_beatsize (geneticc_dma_beatsize_t beatsize)
{
	#warning For some reason this isn't getting compiled, I'll look into it later
	//if(beatsize == BEATSIZE_MAX)
	//beatsize == GENETICC_DMA_MAX_BEATSIZE;
	
	switch(beatsize)
	{
		case BEATSIZE_MAX:
		beatsize = GENETICC_DMA_MAX_BEATSIZE;
		break;
	}
	
	if(beatsize <= GENETICC_DMA_MAX_BEATSIZE && geneticc_dma_control.beatsize != beatsize)
	{
		geneticc_dma_control.beatsize = beatsize;
		//GENETICC_DMA_MIN_SIZE = beatsize * GENETICC_DMA_MIN_TRANSACTION_COUNT;
		//GENETICC_DMA_SYNC_MIN_SIZE = beatsize * GENETICC_DMA_SYNC_MIN_TRANSACTION_COUNT;
		GENETICC_DMA_SET_BEAT_SIZE(geneticc_dma_beatsize_enum_to_mask(beatsize));
	}
}

geneticc_dma_beatsize_t __attribute__((__always_inline__)) geneticc_dma_get_beatsize()
{
	return geneticc_dma_control.beatsize;
}

size_t geneticc_dma_get_transfer_size(geneticc_dma_beatsize_t beatsize, size_t size, uint8_t* leftovers)
{
	//For some reason the if statement was not included in the disassembly.
	//if(beatsize == BEATSIZE_MAX)
	//{
		//beatsize == GENETICC_DMA_MAX_BEATSIZE;
	//}
	
	switch(beatsize)
	{
		case BEATSIZE_MAX:
		beatsize = GENETICC_DMA_MAX_BEATSIZE;
		break;
	}
	
	
	*leftovers = size % beatsize;
	
	//*size -= *leftovers;
	return size - *leftovers;
}

#pragma endregion Functions




