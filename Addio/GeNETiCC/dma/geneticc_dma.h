#ifndef GENETICC_DMA_H_
#define GENETICC_DMA_H_
#if GENETICC_USE_DMA == true

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <utils_assert.h>

#include "geneticc_dma_config.h"
#include "geneticc_dma_defs.h"




#pragma region Functions

inline void  geneticc_dma_init ();

inline void  geneticc_dma_system_init ();

inline void __attribute__((weak)) geneticc_dma_enable();
inline void __attribute__((weak)) geneticc_dma_disable();

inline bool geneticc_dma_get_enabled();
inline size_t geneticc_dma_get_min_size();

GENOPTI_INLINE uint32_t geneticc_dma_get_transaction_count(size_t size, geneticc_dma_beatsize_t beatsize);

GENOPTI_INLINE bool geneticc_dma_size_ok(size_t size, geneticc_dma_beatsize_t beatsize);

/*
	Warning, if you register the transfer done callback, you must delete the transfer when finished!
*/

void geneticc_dma_register_user_callbacks(enum geneticc_dma_cb_type type, /*DMA_USER_CALLBACK*/ FUNC_PTR cb);

uint8_t geneticc_dma_beatsize_enum_to_mask(geneticc_dma_beatsize_t bs);

void geneticc_dma_set_beatsize  (geneticc_dma_beatsize_t beatsize);
inline geneticc_dma_beatsize_t geneticc_dma_get_beatsize();

size_t geneticc_dma_get_transfer_size(uint8_t beatsize, size_t size, uint8_t* leftovers);

#pragma endregion Functions

#pragma region Internal Functions

void internal_geneticc_dma_start_transfer_async(geneticc_dma_subtransfer_t* subtransfer);
void internal_geneticc_dma_start_transfer_sync(geneticc_dma_subtransfer_t* subtransfer);

#pragma endregion Internal Functions

#pragma region Shared System Functions
/*
	If there are no functions that can be called from the macros,
	add them to here for future systems to use.

	*In your C file, remember to wrap them in #if GENETICC_DMA_SYSTEM == THE_SYSTEM.
*/


/*	geneticc_dma_wait
*
*	returns true, if the transfer was complete, and false if the transfer timed out.
*/
bool geneticc_dma_wait();


bool geneticc_dma_busy();

bool geneticc_dma_busy_timeout();


void geneticc_dma_transfer_done_callback();
void geneticc_dma_error_callback();


#pragma endregion Shared System Functions

#pragma region System Definitions


//////////////////////////////////////////////////////////////////////////
//					Atmel Start Definitions
//////////////////////////////////////////////////////////////////////////
#if GENETICC_DMA_SYSTEM == GENETICC_DMA_SYSTEM_ATMELSTART_ARM
#pragma region Atmel Start

#include <hpl_dma.h>

extern DmacDescriptor _descriptor_section[DMAC_CH_NUM];

int32_t _dma_disable_transaction(const uint8_t channel);

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

#define GENETICC_DMA_BEATSIZE_TO_AMOUNT(beatsize, size)

#define GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE					void geneticc_dma_transfer_done_system_callback(struct _dma_resource *resource)
#define GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE							void geneticc_dma_error_system_callback(struct _dma_resource *resource)

#define GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_RETURNTYPE			void
#define GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_PARAMS				struct _dma_resource *resource
#define GENETICC_DMA_CALLBACK_TRANSFERDONE__PARAMS						resource

#define GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_RETURNTYPE				void
#define GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_PARAMS					struct _dma_resource *resource
#define GENETICC_DMA_CALLBACK_ERROR_PARAMS								resource



#define GENETICC_DMA_SET_SRC_ADDRESS(src)			_dma_set_source_address(GENETICC_DMA_CHANNEL, src)
#define GENETICC_DMA_SET_DEST_ADDRESS(dest)			_dma_set_destination_address(GENETICC_DMA_CHANNEL, dest)

#define GENETICC_DMA_SET_ADDRESSES(dest, src)		_dma_set_source_address(GENETICC_DMA_CHANNEL, src), \
													_dma_set_destination_address(GENETICC_DMA_CHANNEL, dest)
													
#define GENETICC_DMA_SET_INC_SRC(enabled)			_dma_srcinc_enable(GENETICC_DMA_CHANNEL, enabled)	
#define GENETICC_DMA_SET_INC_DEST(enabled)			_dma_dstinc_enable(GENETICC_DMA_CHANNEL, enabled)	

#define GENETICC_DMA_SET_BEAT_SIZE(beatsize)			hri_dmacdescriptor_write_BTCTRL_BEATSIZE_bf(&_descriptor_section[GENETICC_DMA_CHANNEL], beatsize)
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
#define GENETICC_DMA_WAIT				geneticc_dma_wait()



/*
*	Cancels the pending DMA transaction.
*/
#define GENETICC_DMA_CANCEL_TRANSACTION				_dma_disable_transaction(GENETICC_DMA_CHANNEL)

#define GENETICC_DMA_ENABLE_INTERRUPT				_dma_set_irq_state(GENETICC_DMA_CHANNEL, DMA_TRANSFER_COMPLETE_CB, true)//, _dma_set_irq_state(GENETICC_DMA_CHANNEL, DMA_TRANSFER_ERROR_CB, true);

#define GENETICC_DMA_DISABLE_INTERRUPT				_dma_set_irq_state(GENETICC_DMA_CHANNEL, DMA_TRANSFER_COMPLETE_CB, false)//, _dma_set_irq_state(GENETICC_DMA_CHANNEL, DMA_TRANSFER_ERROR_CB, false)

//#define GENETICC_DMA_CLEAR_TRANSFERDONE_INTERRUPT	//hri_dmac_clear_CHINTFLAG_TCMPL_bit(DMAC) Automatically cleared

//#define GENETICC_DMA_CLEAR_ERROR_INTERRUPT		//hri_dmac_clear_CHINTFLAG_TERR_bit(DMAC) Automatically cleared


#pragma endregion Atmel Start
//#elif GENETICC_DMA_SYSTEM ==
#endif
#pragma endregion System Definitions

#pragma region Callback Macros

/*
	Prototypes
*/
GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_RETURNTYPE			geneticc_dma_transfer_done_system_callback (GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_PARAMS);
GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_RETURNTYPE				geneticc_dma_error_system_callback (GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_PARAMS);

#define GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_NAME		geneticc_dma_transfer_done_system_callback
#define GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_NAME				geneticc_dma_error_system_callback

/*
*	Secondary Callbacks
*
*	*Allows the native callbacks to be used outside of GeNETiCC.
*	*When defined, the GeNETiCC callback will call this callback, passing the parameters from the native callback.
*/
//#define GENETICC_DMA_TRANSFERDONE_SECONDARY_CALLBACK
//#define GENETICC_DMA_ERROR_SECONDARY_CALLBACK			


#pragma endregion Callback Macros

#pragma region Macro Check
#warning reminder to add new macros
//
//#if !defined(GENETICC_DMA_MAX_BEATSIZE)
//#error Must be defined
//#endif
//
//#if !defined(GENETICC_DMA_BEATSIZE_BYTE_MASK)
//#error Must be defined
//#endif
//
//#if !defined(GENETICC_DMA_BEATSIZE_WORD_MASK)
//#error Must be defined
//#endif
//
//#if !defined(GENETICC_DMA_BEATSIZE_DWORD_MASK)
//#error Must be defined
//#endif
//
//#if !defined(GENETICC_DMA_BEATSIZE_QWORD_MASK)
//#error Must be defined
//#endif
//
//#if !defined(GENETICC_DMA_BEATSIZE_TO_AMOUNT)
//#error Must be defined
//#endif
//
////#if !defined(GENETICC_DMA_SET_SRC_ADDRESS)
////#error Must be defined
////#endif
////
////#if !defined(GENETICC_DMA_SET_DEST_ADDRESS)
////#error Must be defined
////#endif
//
//#if !defined(GENETICC_DMA_SET_ADDRESSES)
//#error Must be defined
//#endif
//
//#if !defined(GENETICC_DMA_SET_BEAT_SIZE)
//#error Must be defined
//#endif
//
//#if !defined(GENETICC_DMA_SET_TRANSACTION_COUNT)
//#error Must be defined
//#endif
//
//#if !defined(GENETICC_DMA_ENABLE_TRANSACTION)
//#error Must be defined
//#endif 
//
//#if !defined(GENETICC_DMA_WAIT_FOR_TRANSFER)
//#error Must be defined
//#endif
//
//#if !defined(GENETICC_DMA_CANCEL_TRANSACTION)
//#error Must be defined
//#endif

#pragma endregion Macro Check

#endif /*GENETICC_USE_DMA == true*/
#endif /* GENETICC_DMA_H_ */
