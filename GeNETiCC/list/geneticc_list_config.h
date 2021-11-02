#ifndef GENETICC_LIST_CONFIG_H_
#define GENETICC_LIST_CONFIG_H_

#include "../config/multisystem_config.h"

#include "../array/geneticc_array_config.h"

/*
*	How many elements can a freshly initialized list store, before having to allocate more memory.
*/
#define LIST_DEFAULT_CAPACITY	4



/*
*	When adding items to a list, and the capacity has ran out,
*	how should we determine the amount of extra memory to allocate?
*/
#define LIST_CAPACITY_INCREASE_METHOD_ADD		0
#define LIST_CAPACITY_INCREASE_METHOD_MULTIPLY	1

#if SYSTEM_TYPE == SYSTEM_TYPE_PC
#define LIST_CAPACITY_INCREASE_METHOD LIST_CAPACITY_INCREASE_METHOD_MULTIPLY
#elif SYSTEM_TYPE == SYSTEM_TYPE_EMBEDDED
#define LIST_CAPACITY_INCREASE_METHOD LIST_CAPACITY_INCREASE_METHOD_ADD
#endif


/*
*	When adding items to a list, and the capacity has ran out,
*	how much extra memory should be allocated?
*
*	*Depends on LIST_CAPACITY_INCREASE_METHOD
*
*	The higher the value will of course use more memory,
*	but the next time you add/insert values, 
*	there may very well be a performance boost.
*/
#if SYSTEM_TYPE == SYSTEM_TYPE_PC
#define LIST_CAPICTY_INCREASE_COUNT			10		//The array will increase by x elements.
#define LIST_CAPICTY_INCREASE_MULTIPLIER	2		//The array will increase by x * size.
#elif SYSTEM_TYPE == SYSTEM_TYPE_EMBEDDED
#define LIST_CAPICTY_INCREASE_COUNT			10		//The array will increase by x elements.
#define LIST_CAPICTY_INCREASE_MULTIPLIER	1.5f	//The array will increase by x * size.
#endif


#if		LIST_CAPACITY_INCREASE_METHOD == LIST_CAPACITY_INCREASE_METHOD_ADD
#define LIST_INCREASE_CAPACITY(capacitySize, elem_size)		capacitySize + (LIST_CAPICTY_INCREASE_COUNT * elem_size)
#elif	LIST_CAPACITY_INCREASE_METHOD == LIST_CAPACITY_INCREASE_METHOD_MULTIPLY
#define LIST_INCREASE_CAPACITY(capacitySize, elem_size)		capacitySize * LIST_CAPICTY_INCREASE_MULTIPLIER
#endif

/*
*	How large can the tail grow before the internal functions realign the array?
*/
#define  LIST_MAX_TAIL_BEFORE_REALIGN		10

#endif /* GENETICC_LIST_CONFIG_H_ */
