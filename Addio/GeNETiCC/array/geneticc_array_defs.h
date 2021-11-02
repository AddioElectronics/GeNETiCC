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

/*
*	Function pointer used for selecting values in an array with a custom set of conditions.
*
*	*Value must be cast to the array's values types.
*/
typedef bool (*PREDICATE)(const ELEMENT_PTR value);

/*
*	Function pointer used for selecting values in an array with a custom set of conditions.
*
*	*Value must be cast to the array's values types.
*/
typedef bool (*PREDICATE_ARGS)(const ELEMENT_PTR value, int arg_count, va_list ap);



#endif /* GENETICC_ARRAY_DEFS_H_ */
