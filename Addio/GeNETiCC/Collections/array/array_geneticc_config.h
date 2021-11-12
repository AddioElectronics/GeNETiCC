
#ifndef ARRAY_GENETICC_CONFIG_H_
#define ARRAY_GENETICC_CONFIG_H_

#include "../../config/addio_multisystem_config.h"

#if SYSTEM_TYPE == SYSTEM_TYPE_PC
#define ARRAY_MAX_SIZE					0x7FFFFFC7		//The max size of the array (in bytes).
#define ARRAY_MAX_LENGTH				0X7FEFFFFF		//The max length of the array (element count)
#elif SYSTEM_TYPE == SYSTEM_TYPE_EMBEDDED
#define ARRAY_MAX_SIZE					0xF4240			//The max size of the array (in bytes).
#define ARRAY_MAX_LENGTH				0xF4240			//The max length of the array (element count)
#endif

/*
*	How large can a single item in an array be?
*
*	*Won't stop you from using larger values, but internal functions will only be able to deal with elements up to this size.
*/
#define ARRAY_ELEMENT_SIZE_TYPE			uint8_t


#endif /* ARRAY_GENETICC_CONFIG_H_ */
