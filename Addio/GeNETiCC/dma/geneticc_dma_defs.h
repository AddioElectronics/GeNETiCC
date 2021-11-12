#ifndef GENETICC_DMA_DEFS_H_
#define GENETICC_DMA_DEFS_H_

#include <utils.h>

#include "../geneticc.h"
//#include "../array_geneticc_defs.h"
#include "../collections/list/list_geneticc.h"

#warning change Geneticc DMA to use a ring buffer instead of a list.

enum geneticc_dma_beatsize
{
	BEATSIZE_MAX = 0,	//The maximum beatsize for the system.
	BEATSIZE_BYTE = 1,
	BEATSIZE_WORD = 2,
	BEATSIZE_DWORD = 4,
	BEATSIZE_QWORD = 8
};
typedef uint8_t geneticc_dma_beatsize_t;


typedef struct
{
	#if GENETICC_OPTIMIZATION_LEVEL <= GENETICC_OPTIMIZATION_LEVEL_SPEED_1
	bool initialized					: 1;	//Initialization routine has been run.
	bool enabled						: 1;	//DMA use is enabled.
	bool transferring					: 1;	//A transfer is currently active.
	bool sync_trans						: 1;	//The current running transfer is synchronous.
	bool sync_waiting					: 1;	//A synchronous transfer is waiting to start.
	//unsigned int async_waiting		: 1;	//An async transfer is in the list.
	unsigned int 						: 2;	/*Reserved*/
	//geneticc_dma_beatsize_t beatsize		: 2;
	#else
	bool initialized;							
	bool enabled;								
	bool transferring;						
	bool sync_trans;							
	bool sync_waiting;							
	//unsigned int async_waiting;
	#endif
	
	geneticc_dma_beatsize_t beatsize;
	unsigned int transfer_count			: 8;
}geneticc_dma_control_reg;


typedef struct
{
	#if GENETICC_OPTIMIZATION_LEVEL <= GENETICC_OPTIMIZATION_LEVEL_SPEED_1
	bool start				: 1;	//Is the transfer waiting to start.
	bool ready				: 1;	//Is the transfer initialized?
	bool running			: 1;	//Have we started to send its sub-transfer to the DMA engine?
	bool complete			: 1;	//Have all the sub-transfers finished?
	bool freesrc			: 1;	//Should the source be freed upon completion?
	bool no_delete			: 1;	//Should the transfer not be deleted upon completion, or if failed to start?
	unsigned int : 3;
	#else
	bool start;
	bool running;
	bool complete;
	bool freesrc;
	bool no_delete;
	#endif
}geneticc_dma_transfer_flags_t;	//Control/Status flags

static const geneticc_dma_transfer_flags_t geneticc_dma_empty_transfer_flags;

typedef struct
{
	list_t	subtransfers;					//list of geneticc_dma_subtransfer_t
	ARRAY_PTR src;							//Pointer to the source array
	ARRAY_PTR dest;							//Pointer to the destination array			
	size_t size;							//The total size of the src array, NOT how many bytes are being transferred
	unsigned int subtran_count		: 8;	//Sub-transfer count
	unsigned int complete_count		: 8;	//How many sub-transfers have finished?
	geneticc_dma_transfer_flags_t flags;
}geneticc_dma_transfer_t;

typedef struct  
{
	//uint8_t channel;							//Currently only 1 channel supported.	Need to add more so interacting with peripherals and events are a possiblity. (It is technically possible now, but having to reconfigure the DMA engine between transfers should be avoided
	//int8_t index;								//Use to make sure transfers run in order. *Currently subtransfers run in order. When multiple channels are supported, this will come into play.
	
	#if GENETICC_OPTIMIZATION_LEVEL <= GENETICC_OPTIMIZATION_LEVEL_SPEED_1	
	bool running					: 1;		//Has the sub-transfer been sent to the DMA engine?
	bool complete					: 1;		//Has the sub-transfer finished?
	bool overlap					: 1;		//This src and dest overlap. (This will send without DMA, used for when transfers must be sent in order.)
	bool freesrc					: 1;		//Should the source be freed upon completion? (Warning! Make sure other transfers do not share the source)
	bool no_incsrc					: 1;		//When true, DMA will not increment the source address.
	bool no_incdest					: 1;		//When true, DMA will not increment the destination address.
	unsigned int					: 2;
	#else
	bool running;								
	bool complete;								
	bool overlap;								
	bool freesrc;								
	bool no_incsrc;								
	bool no_incdest;							
	#endif
	
	geneticc_dma_beatsize_t beatsize;
}geneticc_dma_subtransfer_flags_t;	//Control flags

static const geneticc_dma_subtransfer_flags_t geneticc_dma_empty_subtransfer_flags;

typedef struct
{
	geneticc_dma_transfer_t* parent;		//Parent transfer
	ARRAY_PTR src;							//Source address
	ARRAY_PTR dest;							//Destination address
	size_t size;							//Transfer size
	geneticc_dma_subtransfer_flags_t flags;	//Status/Control Flags
}geneticc_dma_subtransfer_t;

/*
*	Struct used to hold the currently running transfers.
*/
//typedef struct 
//{
	//int8_t channel;
	////geneticc_dma_transfer_t* transfer;		Transfer can be retrieved from subtransfer->parent.
	//geneticc_dma_subtransfer_t* subtransfer;
//}geneticc_dma_active_transfer_t;


enum geneticc_dma_cb_type
{
	#warning reminder to implement error callback.
	DMA_CB_TRANSFER_DONE,
	DMA_CB_SUBTRANSFER_DONE,
	DMA_CB_ERROR		//Not implemented yet
	//DMA_CB_CHANNEL_SUSPEND	//Unsupported
};

/*
*	Function pointer used for letting the user know the last DMA transfer has finished.
*
*	*Value must be cast to the array's values types.
*/
//typedef void (*DMA_USER_CALLBACK)(const geneticc_dma_transfer_t* value);

/**
 * \brief DMA interrupt callbacks
 */
struct geneticc_dma_user_callbacks {
	void (*transfer_done)(const geneticc_dma_transfer_t* value);
	void (*subtransfer_done)(const geneticc_dma_subtransfer_t* value);
	void (*error)(const geneticc_dma_transfer_t* value);
};

/*
*	Used to find a transfer from the destination and source.
*/
static bool predicate_transfer_destsrcmatch(const geneticc_dma_transfer_t** transfer, int arg_count, va_list ap)
{
	return (*transfer)->dest == va_arg(ap, ARRAY_PTR) && (*transfer)->src == va_arg(ap, ARRAY_PTR);
}

/*
*	Used to find a transfer from the destination.
*/
static bool predicate_transfer_destmatch(const geneticc_dma_transfer_t** transfer, int arg_count, va_list ap)
{
	return (*transfer)->dest == va_arg(ap, ARRAY_PTR);
}

static bool predicate_transfer_start_triggered(const geneticc_dma_transfer_t** transfer)
{
	return (*transfer)->flags.start == true && (*transfer)->flags.ready == true;
}

static bool predicate_transfer_transferring(const geneticc_dma_transfer_t** transfer)
{
	return (*transfer)->flags.running == true;
}

static bool predicate_transfer_freesrc(const geneticc_dma_transfer_t** transfer)
{
	return (*transfer)->flags.freesrc == true;
}


static bool predicate_subtransfer_transferring(const geneticc_dma_subtransfer_t** subtransfer)
{
	return (*subtransfer)->flags.running == true && (*subtransfer)->flags.complete == false;
}

static bool predicate_subtransfer_running(const geneticc_dma_subtransfer_t** subtransfer)
{
	return (*subtransfer)->flags.running == true;
}

static bool predicate_subtransfer_waiting(const geneticc_dma_subtransfer_t** subtransfer)
{
	return (*subtransfer)->flags.running == false;
}

static bool predicate_subtransfer_complete(const geneticc_dma_subtransfer_t** subtransfer)
{
	return (*subtransfer)->flags.complete == true;
}

static bool predicate_subtransfer_incomplete(const geneticc_dma_subtransfer_t** subtransfer)
{
	return (*subtransfer)->flags.complete == false;
}

static bool predicate_subtransfer_freesrc(const geneticc_dma_subtransfer_t** subtransfer)
{
	return (*subtransfer)->flags.freesrc == true;
}


#endif /* GENETICC_DMA_DEFS_H_ */
