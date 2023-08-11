
#ifndef GENETICC_ENUMERABLE_CONFIG_H_
#define GENETICC_ENUMERABLE_CONFIG_H_

#include "Addio/config/addio_multisystem_config.h"

#if SYSTEM_TYPE == SYSTEM_TYPE_PC
#define ENUMERABLE_MAX_SIZE					0x7FFFFFC7		//The max size of the enumerable (in bytes).
#define ENUMERABLE_MAX_LENGTH				0X7FEFFFFF		//The max length of the enumerable (element count)
#elif SYSTEM_TYPE == SYSTEM_TYPE_EMBEDDED
#define ENUMERABLE_MAX_SIZE					0xF4240			//The max size of the enumerable (in bytes).
#define ENUMERABLE_MAX_LENGTH				0xF4240			//The max length of the enumerable (element count)
#endif

/*
*	How large can a single item in an enumerable be?
*
*	*Won't stop you from using larger values, but internal functions will only be able to deal with elements up to this size.
*/
#define ENUMERABLE_ELEMENT_SIZE_TYPE			uint8_t


#endif /* GENETICC_ENUMERABLE_CONFIG_H_ */
