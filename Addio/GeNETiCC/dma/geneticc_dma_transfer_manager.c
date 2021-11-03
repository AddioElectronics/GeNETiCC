#include "geneticc_dma_transfer_manager.h"

#include "../geneticc_memory.h"
#include "../list/geneticc_list.h"

#pragma region Variables

extern volatile geneticc_dma_control_reg geneticc_dma_control;

extern volatile list_t	geneticc_dma_transfer_list;	//list of geneticc_dma_transfer_t

extern struct geneticc_dma_user_callbacks geneticc_dma_user_callbacks;

//The subtransfers that are currently running.
geneticc_dma_subtransfer_t* current_subtransfer;

//list_t active_transfers;

#pragma endregion Variables

#pragma region Functions

void geneticc_dma_register_user_callbacks(enum geneticc_dma_cb_type type, FUNC_PTR cb)
{
	switch(type)
	{
		case DMA_CB_TRANSFER_DONE:
		geneticc_dma_user_callbacks.transfer_done = cb;
		break;
		case DMA_CB_SUBTRANSFER_DONE:
		geneticc_dma_user_callbacks.subtransfer_done = cb;
		break;
		case DMA_CB_ERROR:
		geneticc_dma_user_callbacks.error = cb;
		break;
		//case DMA_CB_CHANNEL_SUSPEND:
		//break;
		
	}
}


#warning when multi-channel support, reminder to add priority, and the ability to skip ahead in line. Should I add standard memcpy if unable to register, or leave that up to the calling function?
bool geneticc_dma_register_transfer(geneticc_dma_transfer_t* transfer)
{
	if(!geneticc_dma_accepting_transfers())
	{
		if(!transfer->flags.no_delete)
		geneticc_dma_delete_transfer(transfer);
		return false;
	}
	
	
	transfer->flags.start = true;	
	internal_list_add_memory(&geneticc_dma_transfer_list, &transfer, sizeof(geneticc_dma_transfer_t*));
	geneticc_dma_control.transfer_count++;
	
	//Try to start the transfer. If there are transfers ahead of it, it will wait for them to finish first.
	internal_geneticc_dma_start_next_transfer(transfer);	
	return true;
}

geneticc_dma_transfer_t* geneticc_dma_create_transfer(void* dest, void* src, size_t size, bool free_source)
{
	geneticc_dma_transfer_t* transfer = malloc(sizeof(geneticc_dma_transfer_t));
	
	transfer->dest = dest;
	transfer->src = src;
	transfer->size = size;
	transfer->complete_count = 0;
	transfer->subtran_count = 0;
	transfer->flags = geneticc_dma_empty_transfer_flags;
	transfer->flags.freesrc = free_source;

	List_Init(&(transfer->subtransfers), sizeof(geneticc_dma_subtransfer_t*));
	

	return transfer;
}

geneticc_dma_subtransfer_t* geneticc_dma_create_subtransfer(ARRAY_PTR dest, ARRAY_PTR src, size_t size, geneticc_dma_transfer_t* transfer, geneticc_dma_subtransfer_flags_t* flags)
{
	geneticc_dma_subtransfer_t* subtransfer = malloc(sizeof(geneticc_dma_subtransfer_t));

	subtransfer->dest = dest;
	subtransfer->src = src;
	subtransfer->size = size;
	subtransfer->parent =  transfer;
	
	if(flags != NULL)
	{
		subtransfer->flags = *flags;
		
		switch(flags->beatsize)
		{
			case BEATSIZE_MAX:
			subtransfer->flags.beatsize = GENETICC_DMA_MAX_BEATSIZE;
			break;
		}
	}	
	else
	{
		subtransfer->flags = geneticc_dma_empty_subtransfer_flags;
		subtransfer->flags.beatsize = GENETICC_DMA_MAX_BEATSIZE;
	}
	
	subtransfer->flags.freesrc = false;
	
	if((dest > src && dest - src <= size && subtransfer->flags.no_incsrc == false) || (dest < src && src - dest  <= size && subtransfer->flags.no_incdest == false))
	subtransfer->flags.overlap = true;
	
	if(transfer != NULL)
	{
		//subtransfer->parent = transfer;
		transfer->subtran_count++;
		internal_list_add_memory(&transfer->subtransfers, &subtransfer, sizeof(geneticc_dma_transfer_t*));
		
		if(flags != NULL )
		{
			if(transfer->flags.freesrc == true || flags->freesrc == true)
			{
				//Make sure the subtransfer source, is not an offset to the original source array.
				if(transfer->src + transfer->size < src /*|| src < transfer->src*/)
				{
					//Not an offset
					subtransfer->flags.freesrc = flags->freesrc;
				}
				else
				{
					//Is an offset. freesrc can not be active as it would try to free the middle of an array.
					subtransfer->flags.freesrc = false;
					//Warning!!! The subtransfer would free memory that either the parent transfer will later free, or is trying to free memory from the inside of an array.
					//while(1){}	
					#warning Should I allow it to be passed and just ignore it, or enter this error state?
					//ASSERT(false);
				}
			}
		}
	}
	else
	{
		subtransfer->flags.freesrc = flags->freesrc;
	}
	
	return subtransfer;
}

bool __attribute__((__always_inline__)) geneticc_dma_accepting_transfers()
{
	return geneticc_dma_get_transfer_count() < GENETICC_DMA_MAX_TRANSFERS;
}

length_t __attribute__((__always_inline__)) geneticc_dma_get_transfer_count()
{
	return internal_list_get_count(&geneticc_dma_transfer_list);
}

void __attribute__((__always_inline__)) geneticc_dma_wait_for_transfer(const void* dest)
{
	while(internal_list_find_args_memory(&geneticc_dma_transfer_list, predicate_transfer_destmatch, 1, dest) != -1);
}

bool __attribute__((__always_inline__)) geneticc_dma_is_transfer_complete(const void* dest)
{
	return internal_list_find_args_memory(&geneticc_dma_transfer_list, predicate_transfer_destmatch, 1, dest) == -1;
}

#pragma endregion Functions


#pragma region Internal Functions


//See if all sub-transfers are complete.
bool __attribute__((__always_inline__)) internal_geneticc_dma_is_transfer_complete(geneticc_dma_transfer_t* transfer)
{
	return transfer->subtran_count == transfer->complete_count;
	//int subcount = internal_list_get_count(&(transfer->sub_transfers));
	//
	//for(int i = 0; i < subcount; i++)
	//{
		//if((geneticc_dma_subtransfer_t*)(transfer->sub_transfers)[i])->flags.complete == false)
		//return false;
	//}
	//
	//return true;
}


geneticc_dma_transfer_t* __attribute__((__always_inline__)) internal_geneticc_dma_get_transfer(PREDICATE predicate)
{
/*
	int subcount = internal_list_get_count(&transfer_list);	
	geneticc_dma_transfer_t* array = internal_list_get_aligned_array(&transfer_list);
	
	for(int i = 0; i < subcount; i++)
	{
		if(array[i].dest == dest)
		return transfer_list[i];
	}
	
	return NULL;*/
	
	//return *(geneticc_dma_transfer_t**)internal_list_find_memory(&geneticc_dma_transfer_list, predicate);
	//return internal_list_get(&geneticc_dma_transfer_list, internal_list_find_memory(&geneticc_dma_transfer_list, predicate));
	return *(geneticc_dma_transfer_t**)List_FindPointers(&geneticc_dma_transfer_list, predicate);
}

geneticc_dma_transfer_t* internal_geneticc_dma_get_transfer_args(PREDICATE_ARGS predicate, int arg_count, ...)
{
	va_list ap;
	va_start(ap, arg_count);
	
	//geneticc_dma_transfer_t* transfer = *(geneticc_dma_transfer_t**)internal_list_find_vargs_memory(&geneticc_dma_transfer_list, predicate, arg_count, ap);	//Calls va_end
	//geneticc_dma_transfer_t* transfer =  internal_list_get(&geneticc_dma_transfer_list, internal_list_find_vargs_memory(&geneticc_dma_transfer_list, predicate, arg_count, ap));	//Calls va_end
	geneticc_dma_transfer_t* transfer = *(geneticc_dma_transfer_t**)List_FindPointersVargs(&geneticc_dma_transfer_list, predicate, arg_count, ap);	//Calls va_end
	return transfer;
}

geneticc_dma_transfer_t* __attribute__((__always_inline__)) internal_geneticc_dma_get_transfer_from_address(ARRAY_PTR* dest, ARRAY_PTR* src)
{
/*
	int subcount = internal_list_get_count(&transfer_list);	
	geneticc_dma_transfer_t* array = internal_list_get_aligned_array(&transfer_list);
	
	for(int i = 0; i < subcount; i++)
	{
		if(array[i].dest == dest)
		return transfer_list[i];
	}
	
	return NULL;*/
	
	//return *(geneticc_dma_transfer_t**)internal_list_find_args_memory(&geneticc_dma_transfer_list, predicate_transfer_destsrcmatch, 2, dest, src);
	return *(geneticc_dma_transfer_t**)List_FindPointersArgs(&geneticc_dma_transfer_list, predicate_transfer_destsrcmatch, 2, dest, src);
}

geneticc_dma_subtransfer_t* __attribute__((__always_inline__)) internal_geneticc_dma_get_subtransfer(geneticc_dma_transfer_t* transfer, PREDICATE predicate)
{
	//return *(geneticc_dma_subtransfer_t**)internal_list_find_memory(&(transfer->subtransfers), predicate);
	return *(geneticc_dma_subtransfer_t**)List_FindPointers(&(transfer->subtransfers), predicate);
}

geneticc_dma_subtransfer_t* internal_geneticc_dma_get_subtransfer_args(geneticc_dma_transfer_t* transfer, PREDICATE_ARGS predicate, int arg_count, ...)
{
	va_list ap;
	va_start(ap, arg_count);
	
	//return *(geneticc_dma_subtransfer_t**)internal_list_find_vargs_memory(&(transfer->subtransfers), predicate, arg_count, ap);	//Calls va_end
	return *(geneticc_dma_subtransfer_t**)List_FindPointersVargs(&(transfer->subtransfers), predicate, arg_count, ap);	//Calls va_end
}

void internal_geneticc_dma_complete_transfer(geneticc_dma_transfer_t* transfer)
{
	if(transfer->flags.freesrc)
	free(transfer->src);
	
	int index = List_Indexof(&geneticc_dma_transfer_list, CAST_STRUCT &transfer);
	if(index != -1)
	{
		internal_list_removeAt_memory(&geneticc_dma_transfer_list, index);
	}

	//Let the user know this transfer has finished.
	if(geneticc_dma_user_callbacks.transfer_done != NULL)
	{
		(*geneticc_dma_user_callbacks.transfer_done)(transfer);
	}
	#if GENETICC_DMA_AUTOFREE_TRANSFER == false
	else
	{
		//Only free transfer if transfer_done callback is not registered.
		if(!transfer->flags.no_delete)
		geneticc_dma_delete_transfer(transfer);
	}
	#else
	//Always free transfer after callback.
	if(!transfer->flags.no_delete)
	geneticc_dma_delete_transfer(transfer);
	#endif
}

bool internal_geneticc_dma_mark_subtransfer_complete(geneticc_dma_subtransfer_t* subtransfer)
{
	bool parent_transfer_complete = false;
	
	if(subtransfer->flags.freesrc)
	free(subtransfer->src);
	
	subtransfer->flags.complete = true;
	
	
	if(subtransfer->parent != NULL)
	{
		subtransfer->parent->complete_count++;		
		parent_transfer_complete = subtransfer->parent->flags.complete = internal_geneticc_dma_is_transfer_complete(subtransfer->parent);
	}
	
	//Let the user know the subtransfer has finished.
	if(geneticc_dma_user_callbacks.subtransfer_done != NULL)
	{
		(*geneticc_dma_user_callbacks.subtransfer_done)(subtransfer);
	}
	
	//I dont like this.
	
	if(subtransfer->parent != NULL)
	{
		if(subtransfer->parent->flags.complete)
			internal_geneticc_dma_complete_transfer(subtransfer->parent);
	}
	
	return parent_transfer_complete;
}


/*
void internal_geneticc_dma_mark_subtransfer_complete(geneticc_dma_transfer_t* transfer)
{
	geneticc_dma_subtransfer_t* sub_transfers = internal_list_get_aligned_array(&transfer->sub_transfers);
	
	size_t array_size = internal_list_get_size(&(transfer->sub_transfers));
	
	//Get first transfer where running flag = true, and complete flag = false, and mark it complete.
	//This transfer has finished, we know because there was a DMA callback, and this is the oldest transfer.
	geneticc_dma_subtransfer_t* marking = internal_array_select_memory(sub_transfers, array_size, sizeof(geneticc_dma_subtransfer_t*), predicate_subtransfer_transferring);
	
	ASSERT(marking);	//Should not be null. The last transfer should be in this array.
	
	if(marking->flags.freesrc)
	free(marking->src);
	
	marking->flags.complete = true;
	transfer->complete_count++;

}*/


void internal_geneticc_dma_start_next_transfer(geneticc_dma_transfer_t* transfer)
{
	//Already a transfer running.
	//if(geneticc_dma_control.transferring)
	//return;

	if(transfer == NULL)
	{
		if(geneticc_dma_control.transfer_count == 0)
		{
			return;
		}
		#warning may not work
		//transfer = *(geneticc_dma_transfer_t**)internal_list_find_memory(&geneticc_dma_transfer_list, predicate_transfer_ready);  
		transfer =  *(geneticc_dma_transfer_t**)List_FindPointers(&geneticc_dma_transfer_list, predicate_transfer_start_triggered);
	}
	
	if(transfer == NULL)	//No transfers ready
		return;
	
	//Get next transfer where running flag = false, and return a pointer to it.
	//If this value is null it means all the transfers are complete.
	//geneticc_dma_subtransfer_t* next =  *(geneticc_dma_subtransfer_t**)internal_list_find_memory(&transfer->subtransfers, predicate_subtransfer_waiting);
	geneticc_dma_subtransfer_t* next =  *(geneticc_dma_subtransfer_t**)List_FindPointers(&transfer->subtransfers, predicate_subtransfer_waiting);

	ASSERT(next);
	
	internal_geneticc_dma_start_transfer_async(next);

}

void internal_geneticc_dma_start_transfer_sync(geneticc_dma_subtransfer_t* subtransfer)
{

	//This subtransfer will overlap.
	//It must be transferred without DMA.
	if(subtransfer->flags.overlap == true)
	{
		subtransfer->flags.running = true;
		geneticc_memcpyovrlp(subtransfer->dest, subtransfer->src, subtransfer->size);
		subtransfer->flags.complete = true;
		return;
	}
	
	if(geneticc_dma_size_ok(subtransfer->size, subtransfer->flags.beatsize) && geneticc_dma_get_enabled())
	{
		geneticc_dma_control.sync_waiting = true;
		
		//Wait for other transfer to finish.
		if(!GENETICC_DMA_BUSY_WAIT)
		{
			//Timed out waiting, use standard method.
			geneticc_dma_control.sync_waiting = false;
			goto StandardCopy;
		}
		
		uint8_t beatsize = subtransfer->flags.beatsize;//geneticc_dma_get_beatsize();
		uint8_t standard_size = subtransfer->size % beatsize;
		size_t dma_size = subtransfer->size - standard_size;
		size_t transaction_count = geneticc_dma_get_transaction_count(dma_size, beatsize);
		
		//Must be set before SET_TRANSACTION_COUNT
		if(beatsize != geneticc_dma_control.beatsize)
			geneticc_dma_set_beatsize(beatsize);
		
		GENETICC_DMA_SET_INC_SRC(!subtransfer->flags.no_incsrc);
		GENETICC_DMA_SET_INC_DEST(!subtransfer->flags.no_incdest);
		GENETICC_DMA_SET_ADDRESSES(subtransfer->dest, subtransfer->src);
		GENETICC_DMA_SET_TRANSACTION_COUNT(transaction_count);
		
		geneticc_dma_control.sync_trans = true;
		geneticc_dma_control.transferring = true;
		
		GENETICC_DMA_ENABLE_TRANSACTION;
		
		subtransfer->flags.running = true;
		geneticc_dma_control.sync_waiting = false;
		
		//Transfer stragglers while waiting for transfer to start.
		if(standard_size > 0)
		{
			memcpy((subtransfer->dest) + dma_size, (subtransfer->src) + dma_size, standard_size);
		}
		
		
		
		//Wait for DMA to finish.
		if(!GENETICC_DMA_WAIT)
		{
			//Timed out(didnt start), cancel and then transfer with memcpy.
			GENETICC_DMA_CANCEL_TRANSACTION;
			subtransfer->flags.running = false;
			geneticc_dma_control.transferring = false;
			geneticc_dma_control.sync_trans = false;
			//geneticc_dma_synchronous_holding = false;
			//geneticc_dma_current_transfer_synchronous = false;
			goto StandardCopy;
		}

		geneticc_dma_control.sync_trans = false;
		subtransfer->flags.complete = true;
		
	}
	else
	{
		StandardCopy:
		subtransfer->flags.running = true;
		memcpy(subtransfer->dest, subtransfer->src, subtransfer->size);
		subtransfer->flags.complete = true;
	}
}


void internal_geneticc_dma_start_transfer_async(geneticc_dma_subtransfer_t* subtransfer)
{
	//This subtransfer will overlap.
	//It must be transferred without DMA.
	if(subtransfer->flags.overlap == true)
	{
		subtransfer->flags.running = true;
		memcpyovrlp(subtransfer->dest, subtransfer->src, subtransfer->size);
		
		
		//internal_geneticc_dma_mark_subtransfer_complete(subtransfer);
		current_subtransfer = subtransfer;
		geneticc_dma_transfer_done_callback();	//Call the callback just in case the transfer_done user callback is relying on it.
		//subtransfer->flags.complete = true;
		return;
	}

	if(geneticc_dma_size_ok(subtransfer->size, subtransfer->flags.beatsize) && geneticc_dma_get_enabled())
	{
		//Wait for other transfer to finish.
		if(!GENETICC_DMA_BUSY_WAIT)
		{
			//Timed out waiting, use standard method.
			goto StandardCopy;
		}
		
		uint8_t beatsize = subtransfer->flags.beatsize;//geneticc_dma_get_beatsize();
		uint8_t standard_size = subtransfer->size % beatsize;
		size_t dma_size = subtransfer->size - standard_size;
		size_t transaction_count = geneticc_dma_get_transaction_count(dma_size, subtransfer->flags.beatsize);
		
		//Must be set before SET_TRANSACTION_COUNT
		if(subtransfer->flags.beatsize != geneticc_dma_control.beatsize)
			geneticc_dma_set_beatsize(subtransfer->flags.beatsize);
			
		GENETICC_DMA_SET_INC_SRC(!subtransfer->flags.no_incsrc);
		GENETICC_DMA_SET_INC_DEST(!subtransfer->flags.no_incdest);
		GENETICC_DMA_SET_ADDRESSES(subtransfer->dest, subtransfer->src);
		GENETICC_DMA_SET_TRANSACTION_COUNT(transaction_count);
		
		if(standard_size > 0)
		{
			//A few stragglers, send them first.
			//We do this because we do not want DMA to finish, trigger a callback, and mark it complete before everything is transferred.
			//This could only happen if min DMA transfer size is too low, and probably wouldn't anyways, but as of now it has not been tested.
			memcpy((subtransfer->dest) + dma_size, (subtransfer->src) + dma_size, standard_size);
		}
		
		subtransfer->flags.running = true;
		geneticc_dma_control.transferring = true;
		current_subtransfer = subtransfer;
		GENETICC_DMA_ENABLE_TRANSACTION;		
	}
	else
	{
		StandardCopy:
		subtransfer->flags.running = true;
		memcpy(subtransfer->dest, subtransfer->src, subtransfer->size);
		current_subtransfer = subtransfer;
		geneticc_dma_transfer_done_callback();
		//subtransfer->flags.complete;
	}
}


void geneticc_dma_delete_transfer(geneticc_dma_transfer_t* transfer)
{
	internal_list_forEach(&(transfer->subtransfers), internal_geneticc_dma_delete_subtransfer, 0, 0);
	List_Delete(&(transfer->subtransfers));
	free(transfer);
}

void internal_geneticc_dma_delete_subtransfer(geneticc_dma_subtransfer_t* subtransfer)
{
	free(subtransfer);
}


#pragma endregion Internal Functions

#pragma region Callback Functions


void geneticc_dma_transfer_done_callback ()
{
	geneticc_dma_control.transferring = false;
	
	//Transfer was synchronous. It was not in the transfer list.
	if(geneticc_dma_control.sync_trans == true)
	{
		geneticc_dma_control.sync_trans = false;
		
		if(geneticc_dma_control.transfer_count == 0) return;
		
		//Start next transfer in list.
		goto GetNextTransfer;
	}
	
	if(geneticc_dma_control.transfer_count == 0) return;
	
	geneticc_dma_transfer_t* transfer =  current_subtransfer->parent;//*(geneticc_dma_transfer_t**)internal_list_get(&geneticc_dma_transfer_list, 0); //internal_geneticc_dma_get_transfer(predicate_transfer_transferring);  Not needed until multi-channel support.
	geneticc_dma_subtransfer_t* subtransfer = current_subtransfer;//internal_geneticc_dma_get_subtransfer(transfer, predicate_subtransfer_transferring);	
	
	current_subtransfer = NULL;
	
	//Is the transfer finished? (Not the subtransfer)
	if(internal_geneticc_dma_mark_subtransfer_complete(subtransfer))
	{		
		GetNextTransfer:
		
		if(--geneticc_dma_control.transfer_count == 0) 
		{
			return;	//No more transfers waiting.
		}
		
		//Get the next transfer in the list.
		//Will return NULL if no more transfers exist.
		transfer = internal_list_get(&geneticc_dma_transfer_list, 0);

	}

	//Synchronous code is trying to start a transfer, let it start.
	//It will start the next transfer when its finished.
	if(geneticc_dma_control.sync_waiting == true)
	{
		return;
	}

	if(transfer != NULL)
	internal_geneticc_dma_start_next_transfer(transfer);
	
}

void geneticc_dma_error_callback ()
{
	
	geneticc_dma_transfer_t* transfer = internal_list_get(&geneticc_dma_transfer_list, 0);
	
	//Let the user know this transfer has had an error.
	if(geneticc_dma_user_callbacks.error != NULL)
		(*geneticc_dma_user_callbacks.error)(transfer);
	
	while(1)
	{
		#warning reminder to finish
		//Need to add functionality that will cancel the transfer, and send it with standard memory functions.
	}
};

GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_RETURNTYPE geneticc_dma_transfer_done_system_callback (GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_PARAMS)
{
	#if defined(GENETICC_DMA_TRANSFERDONE_SECONDARY_CALLBACK)
		GENETICC_DMA_TRANSFERDONE_SECONDARY_CALLBACK(GENETICC_DMA_CALLBACK_TRANSFERDONE__PARAMS);
	#endif
	
	#if defined(GENETICC_DMA_CLEAR_TRANSFERDONE_INTERRUPT)
	GENETICC_DMA_CLEAR_TRANSFERDONE_INTERRUPT();
	#endif
		
	
	//Callback fired from non GeNETiCC transfer.
	if(geneticc_dma_control.transferring = false) return;
	geneticc_dma_transfer_done_callback();
}

GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_RETURNTYPE geneticc_dma_error_system_callback (GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_PARAMS)
{
	#if defined(GENETICC_DMA_ERROR_SECONDARY_CALLBACK)
		GENETICC_DMA_ERROR_SECONDARY_CALLBACK(GENETICC_DMA_CALLBACK_ERROR_PARAMS);
	#endif
	
	#if defined(GENETICC_DMA_CLEAR_ERROR_INTERRUPT)
	GENETICC_DMA_CLEAR_ERROR_INTERRUPT();
	#endif
	
	//Callback fired from non GeNETiCC transfer.
	if(geneticc_dma_control.transferring = false) return;
	geneticc_dma_error_callback();
}


#pragma endregion Callback Functions

