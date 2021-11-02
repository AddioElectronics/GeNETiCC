#ifndef GENETICC_DMA_CONFIG_H_
#define GENETICC_DMA_CONFIG_H_

#include "geneticc_dma_defs.h"


/*
*	The systems/environments that the library currently supports.
*/
#define GENETICC_DMA_SYSTEM_NONE -1
#define GENETICC_DMA_SYSTEM_ATMELSTART_ARM 0

//The current system the library is compiling for.
#define GENETICC_DMA_SYSTEM	GENETICC_DMA_SYSTEM_ATMELSTART_ARM

/*
*	The DMA channel to use for memory transfers.
*
*	*In the future the channel
*/
#define GENETICC_DMA_CHANNEL		11

/*Multi-channel configuration not implemented yet*/
/*
*	Should each channel be configured to meet the transfer's flags every time, or are channels configured before hand?
*
*	*If true, you must be careful about what channel the transfers use.
*/
//#define GENETICC_DMA_CHANNEL_PRECONFIGURED	false
//#define GENETICC_DMA_TX_RX_USE_ANY_CHANNEL	true		//If true, TX and RX to use the channel set in the transfer flag. If false, they will use a specific channel that was preconfigured.
//#define GENETICC_DMA_CHANNEL_TX			11			//(src inc, dest !inc)
//#define GENETICC_DMA_CHANNEL_RX			11			//(src !inc, dest inc)

/*
*	The minimum transaction count needed to use DMA over memcpy.
*/
#warning reminder to set transaction count to a reasonable value before publishing
#define GENETICC_DMA_MIN_TRANSACTION_COUNT	100
#define GENETICC_DMA_SYNC_MIN_TRANSACTION_COUNT 2048		//For synchronous methods (inside geneticc_memory.h)


/*
*	The minimum transfer size needed to use DMA over memcpy.
*
*	*(GENETICC_DMA_MIN_TRANSACTION_COUNT * beatsize)
*/
//extern size_t GENETICC_DMA_MIN_SIZE;
//extern size_t GENETICC_DMA_SYNC_MIN_SIZE;		//For synchronous methods (inside geneticc_memory.h)

/*
*	How many transfers can be in the list at once?
*/
#define GENETICC_DMA_MAX_TRANSFERS				100	//Absolute max is 255	(To increase, change the bit count for the transfer_count bit field in the geneticc_dma_control_reg definition)

/*
*	How many subtransfers can fit in a transfer.
*/
#define GENETICC_DMA_MAX_SUBTRANSFERS			100	//Absolute max is 255	(To increase, change the bit count for the subtran_count and complete_count bit fields in the geneticc_dma_transfer_t definition)

/*
*	How many loop cycles in the geneticc_dma_wait() equals a timeout.
*/
#define GENETICC_DMA_TIMEOUT		10000


/*
*	If the transfer_done user callback is registered,
*	the transfer will be freed after the callback executes.
*	This allows the user to use the transfer in the callback and not have to free itself.
*/
#define GENETICC_DMA_AUTOFREE_TRANSFER	false


#endif /* GENETICC_DMA_CONFIG_H_ */
