#ifndef GENETICC_ARRAY_DEFS_H_
#define GENETICC_ARRAY_DEFS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

#include "geneticc_array_config.h"


#define ARRAY_PTR		uint8_t*

/*
*	Used to describe the the size of an element in an array, in bytes.
*	*For standard value types and structs.
*/
typedef ARRAY_ELEMENT_SIZE_TYPE element_size_t;





#endif /* GENETICC_ARRAY_DEFS_H_ */
