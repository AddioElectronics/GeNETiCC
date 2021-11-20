///*
//*	Macros required for each system.
//*/
//
//#pragma region Configuration Macros
//
//
///*
//*	The largest beatsize the system supports. (geneticc_dma_beatsize_t)
//*
//*	*Cannot be BEATSIZE_MAX
//*/
//#define GENETICC_DMA_MAX_BEATSIZE		BEATSIZE_DWORD
//
///*
//*	DMA hardware register values for the beatsize.
//*
//*	*For unsupported beat sizes, comment them out.
//*/
//#define GENETICC_DMA_BEATSIZE_BYTE_MASK		
//#define GENETICC_DMA_BEATSIZE_WORD_MASK		
//#define GENETICC_DMA_BEATSIZE_DWORD_MASK	
//#define GENETICC_DMA_BEATSIZE_QWORD_MASK
//
///*Transfer done callback*/
//
////The return type of your systems transfer done callback.
//#define GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_RETURNTYPE		
//
////The parameters of your systems transfer done callback, types included.	
//#define GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_PARAMS	
//
////The parameters of your systems transfer done callback, types not included.				
//#define GENETICC_DMA_CALLBACK_TRANSFERDONE__PARAMS						
//
///*Error callback*/
//
////The return type of your systems transfer done callback.
//#define GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_RETURNTYPE	
//
////The parameters of your systems transfer done callback, types included.			
//#define GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_PARAMS	
//
////The parameters of your systems transfer done callback, types not included.					
//#define GENETICC_DMA_CALLBACK_ERROR_PARAMS		
//
//
//
//#pragma endregion Configuration Macros						
//
//#pragma region Function macros
//
//
////Point to a function which sets the source address.
//#define GENETICC_DMA_SET_SRC_ADDRESS(src)	
//
////Point to a function which sets the destination address.		
//#define GENETICC_DMA_SET_DEST_ADDRESS(dest)			
//
////Point to a function which sets the destination and source address.
//#define GENETICC_DMA_SET_ADDRESSES(dest, src)		GENETICC_DMA_SET_DEST_ADDRESS(dest), GENETICC_DMA_SET_SRC_ADDRESS(src)
//
////Point to a function which will set or disable the incrementing of the source address during transfers.	
//#define GENETICC_DMA_SET_INC_SRC(enabled)	
//
////Point to a function which will set or disable the incrementing of the destination address during transfers.		
//#define GENETICC_DMA_SET_INC_DEST(enabled)		
//
////Point to a function which will set how many beats will be transferred.
//#define GENETICC_DMA_SET_BEAT_SIZE(beatsize)
//
////Point to a function which will set how many beats will be transferred.
//#define GENETICC_DMA_SET_TRANSACTION_COUNT(count)	
//
////Point to a function which will initiate the transfer.
//#define GENETICC_DMA_ENABLE_TRANSACTION		
//
////Point to a function that will cancel a transfer that has been set up.
//#define GENETICC_DMA_CANCEL_TRANSACTION
//
///*
//*	Point to a function which returns if the DMA engine is in a busy state.
//*
//*	/returns	True if it is busy, false if it is idle.
//*/
//#define GENETICC_DMA_BUSY							
//
///*
//*	Point to a function which waits for the DMA to exit its busy state, or times out.
//*
//*	/returns	True when the DMA engine is free, and false if the timeout is reached.
//*/
//#define GENETICC_DMA_BUSY_WAIT	
					//
///*
//*	Points to a function that waits for the transfer to finish, or times out.
//*
//*	/returns	True when transfer is complete, and false if the timeout is reached.
//*/
//#define GENETICC_DMA_WAIT
//
//
//
		//
//
////Point to a function that will enable DMA interrupts.
//#define GENETICC_DMA_ENABLE_INTERRUPT				_dma_set_irq_state(GENETICC_DMA_CHANNEL, DMA_TRANSFER_COMPLETE_CB, true)//, _dma_set_irq_state(GENETICC_DMA_CHANNEL, DMA_TRANSFER_ERROR_CB, true);
//
////Point to a function that will disable DMA interrupts.
//#define GENETICC_DMA_DISABLE_INTERRUPT				_dma_set_irq_state(GENETICC_DMA_CHANNEL, DMA_TRANSFER_COMPLETE_CB, false)//, _dma_set_irq_state(GENETICC_DMA_CHANNEL, DMA_TRANSFER_ERROR_CB, false)
//
////If DMA interrupt flags are not automatically cleared, point to a function which will clear the transfer done interrupt.
////If they are automatically cleared, comment it out.
////#define GENETICC_DMA_CLEAR_TRANSFERDONE_INTERRUPT	
//
////If DMA interrupt flags are not automatically cleared, point to a function which will clear the error interrupt.
////If they are automatically cleared, comment it out.
////#define GENETICC_DMA_CLEAR_ERROR_INTERRUPT		
//
//#pragma endregion Function macros
