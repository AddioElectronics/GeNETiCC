
#ifndef GENETICC_DMA_TRANSFER_H_
#define GENETICC_DMA_TRANSFER_H_

#include "geneticc_dma.h"

#pragma region Functions

bool geneticc_dma_register_transfer(geneticc_dma_transfer_t* transfer);

geneticc_dma_transfer_t* geneticc_dma_create_transfer(void* dest, void* src, size_t size, bool free_source);
geneticc_dma_subtransfer_t* geneticc_dma_create_subtransfer(ARRAY_PTR dest, ARRAY_PTR src, size_t size, geneticc_dma_transfer_t* transfer, geneticc_dma_subtransfer_flags_t* flags);

inline bool geneticc_dma_accepting_transfers();

inline length_t geneticc_dma_get_transfer_count();

inline void geneticc_dma_wait_for_transfer(const void* dest);

//Checks to see if a transfer is complete using the dest and src
inline bool geneticc_dma_is_transfer_complete(const void* dest);


void geneticc_dma_delete_transfer(geneticc_dma_transfer_t* transfer);

#pragma endregion Functions

#pragma region Internal Functions


//Checks to see if a geneticc_dma_transfer_t is complete.
inline bool internal_geneticc_dma_is_transfer_complete(geneticc_dma_transfer_t* transfer);

inline geneticc_dma_transfer_t* internal_geneticc_dma_get_transfer(PREDICATE predicate);

geneticc_dma_transfer_t* internal_geneticc_dma_get_transfer_args(PREDICATE_ARGS predicate, int arg_count, ...);

inline geneticc_dma_transfer_t* internal_geneticc_dma_get_transfer_from_address(ARRAY_PTR* dest, ARRAY_PTR* src);

inline geneticc_dma_subtransfer_t* internal_geneticc_dma_get_subtransfer(geneticc_dma_transfer_t* transfer, PREDICATE predicate);

geneticc_dma_subtransfer_t* internal_geneticc_dma_get_subtransfer_args(geneticc_dma_transfer_t* transfer, PREDICATE_ARGS predicate, int arg_count, ...);

void internal_geneticc_dma_complete_transfer(geneticc_dma_transfer_t* transfer);

bool internal_geneticc_dma_mark_subtransfer_complete(geneticc_dma_subtransfer_t* transfer);

void internal_geneticc_dma_start_next_transfer(geneticc_dma_transfer_t* transfer);




void internal_geneticc_dma_delete_subtransfer(geneticc_dma_subtransfer_t* subtransfer);

#pragma endregion Internal Functions


#endif /* GENETICC_DMA_TRANSFER_H_ */
