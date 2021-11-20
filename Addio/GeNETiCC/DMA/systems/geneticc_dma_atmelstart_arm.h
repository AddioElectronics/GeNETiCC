
#ifndef GENETICC_DMA_ATMELSTART_ARM_H_
#define GENETICC_DMA_ATMELSTART_ARM_H_

//Functions that were not included in the native API, that are required for the system macros.
#pragma region Additional Functions


/*	geneticc_dma_wait
*
*	returns true, if the transfer was complete, and false if the transfer timed out.
*/
bool geneticc_dma_wait();


bool geneticc_dma_busy();

bool geneticc_dma_busy_timeout();

#pragma endregion Functions



#pragma region System Macros

#include <hpl_dma.h>

extern DmacDescriptor _descriptor_section[DMAC_CH_NUM];

int32_t _dma_disable_transaction(const uint8_t channel);

#pragma region Configuration Macros

/*
*	The largest beatsize the system supports. (geneticc_dma_beatsize_t)
*
*	*BEATSIZE_MAX will select the largest beatsize for the current system.
*	*System MAX is set in geneticc_dma.h,
*/
#define GENETICC_DMA_MAX_BEATSIZE		BEATSIZE_DWORD

/*
*	DMA hardware register values for the beatsize.
*/
#define GENETICC_DMA_BEATSIZE_BYTE_MASK		0x00
#define GENETICC_DMA_BEATSIZE_WORD_MASK		0x01
#define GENETICC_DMA_BEATSIZE_DWORD_MASK	0x02
//#define GENETICC_DMA_BEATSIZE_QWORD_MASK	//Unsupported

//#define GENETICC_DMA_BEATSIZE_TO_AMOUNT(beatsize, size)

#define GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_RETURNTYPE			void
#define GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_PARAMS				struct _dma_resource *resource
#define GENETICC_DMA_CALLBACK_TRANSFERDONE__PARAMS						resource

#define GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_RETURNTYPE				void
#define GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_PARAMS					struct _dma_resource *resource
#define GENETICC_DMA_CALLBACK_ERROR_PARAMS								resource

#pragma endregion Configuration Macros

#pragma region Function macros

#define GENETICC_DMA_SET_SRC_ADDRESS(src)			_dma_set_source_address(GENETICC_DMA_CHANNEL, src)
#define GENETICC_DMA_SET_DEST_ADDRESS(dest)			_dma_set_destination_address(GENETICC_DMA_CHANNEL, dest)

#define GENETICC_DMA_SET_ADDRESSES(dest, src)		_dma_set_source_address(GENETICC_DMA_CHANNEL, src), \
_dma_set_destination_address(GENETICC_DMA_CHANNEL, dest)

#define GENETICC_DMA_SET_INC_SRC(enabled)			_dma_srcinc_enable(GENETICC_DMA_CHANNEL, enabled)
#define GENETICC_DMA_SET_INC_DEST(enabled)			_dma_dstinc_enable(GENETICC_DMA_CHANNEL, enabled)

#define GENETICC_DMA_SET_BEAT_SIZE(beatsize)		hri_dmacdescriptor_write_BTCTRL_BEATSIZE_bf(&_descriptor_section[GENETICC_DMA_CHANNEL], beatsize)
#define GENETICC_DMA_SET_TRANSACTION_COUNT(count)	_dma_set_data_amount(GENETICC_DMA_CHANNEL, count)


/*
*
*/
#define GENETICC_DMA_ENABLE_TRANSACTION				_dma_enable_transaction(GENETICC_DMA_CHANNEL, true)

#define GENETICC_DMA_BUSY							geneticc_dma_busy()


#define GENETICC_DMA_BUSY_WAIT						geneticc_dma_busy_timeout()

/*
*	Waits for the transfer to finish, or times out.
*
*	/returns	True when transfer is complete, and false if the timeout is reached.
*/
#define GENETICC_DMA_WAIT							geneticc_dma_wait()



/*
*	Cancels the pending DMA transaction.
*/
#define GENETICC_DMA_CANCEL_TRANSACTION				_dma_disable_transaction(GENETICC_DMA_CHANNEL)

#define GENETICC_DMA_ENABLE_INTERRUPT				_dma_set_irq_state(GENETICC_DMA_CHANNEL, DMA_TRANSFER_COMPLETE_CB, true)//, _dma_set_irq_state(GENETICC_DMA_CHANNEL, DMA_TRANSFER_ERROR_CB, true)

#define GENETICC_DMA_DISABLE_INTERRUPT				_dma_set_irq_state(GENETICC_DMA_CHANNEL, DMA_TRANSFER_COMPLETE_CB, false)//, _dma_set_irq_state(GENETICC_DMA_CHANNEL, DMA_TRANSFER_ERROR_CB, false)

//#define GENETICC_DMA_CLEAR_TRANSFERDONE_INTERRUPT	//hri_dmac_clear_CHINTFLAG_TCMPL_bit(DMAC) Automatically cleared

//#define GENETICC_DMA_CLEAR_ERROR_INTERRUPT		//hri_dmac_clear_CHINTFLAG_TERR_bit(DMAC) Automatically cleared

#pragma endregion Function macros
#pragma endregion System Macros

#endif /* GENETICC_DMA_ATMELSTART_ARM_H_ */