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

#pragma region System Functions
/*
*	Functions must added to "geneticc_dma_system_%YOURSYSTEM%"
*/

inline void  geneticc_dma_system_init(); //Optional

void internal_geneticc_dma_register_system_callbacks();

#pragma endregion System Functions

#pragma region System Definitions
//////////////////////////////////////////////////////////////////////////
//					Include your system here.
//////////////////////////////////////////////////////////////////////////
#if GENETICC_DMA_SYSTEM == GENETICC_DMA_SYSTEM_ATMELSTART_ARM
#include "systems/geneticc_dma_atmelstart_arm.h"

//#elif GENETICC_DMA_SYSTEM == GENETICC_DMA_SYSTEM_%SYSTEM%
//#include "systems/geneticc_dma_%SYSTEM%.h"
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


#if !defined(GENETICC_DMA_MAX_BEATSIZE)
#error Must be defined
#endif

//#if !defined(GENETICC_DMA_SET_SRC_ADDRESS)
//#error Must be defined
//#endif
//
//#if !defined(GENETICC_DMA_SET_DEST_ADDRESS)
//#error Must be defined
//#endif

#if !defined(GENETICC_DMA_SET_ADDRESSES)
#error Must be defined
#endif

#if !defined(GENETICC_DMA_SET_BEAT_SIZE)
#error Must be defined
#endif

#if !defined(GENETICC_DMA_SET_TRANSACTION_COUNT)
#error Must be defined
#endif

#if !defined(GENETICC_DMA_ENABLE_TRANSACTION)
#error Must be defined
#endif 

#if !defined(GENETICC_DMA_CANCEL_TRANSACTION)
#error Must be defined
#endif

#if !defined(GENETICC_DMA_BUSY)
#error Must be defined
#endif

#if !defined(GENETICC_DMA_BUSY_WAIT)
#error Must be defined
#endif

#if !defined(GENETICC_DMA_WAIT)
#error Must be defined
#endif

#if !defined(GENETICC_DMA_ENABLE_INTERRUPT)
#error Must be defined
#endif

#if !defined(GENETICC_DMA_DISABLE_INTERRUPT)
#error Must be defined
#endif



#if !defined(GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_RETURNTYPE)
#error Must be defined
#endif

#if !defined(GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_PARAMS)
#error Must be defined
#endif

#if !defined(GENETICC_DMA_CALLBACK_TRANSFERDONE__PARAMS)
#error Must be defined
#endif

#if !defined(GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_RETURNTYPE)
#error Must be defined
#endif

#if !defined(GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_PARAMS)
#error Must be defined
#endif

#if !defined(GENETICC_DMA_CALLBACK_ERROR_PARAMS)
#error Must be defined
#endif

#pragma endregion Macro Check

#endif /*GENETICC_USE_DMA == true*/
#endif /* GENETICC_DMA_H_ */
