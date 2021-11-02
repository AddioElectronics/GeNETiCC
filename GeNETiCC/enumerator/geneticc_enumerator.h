
#ifndef GENETICC_ENUMERATOR_H_
#define GENETICC_ENUMERATOR_H_

#include "../geneticc.h"
#include "../array/geneticc_array_defs.h"


//////////////////////////////////////////////////////////////////////////
//	This is a WIP, and not recommended to use at this time.
//////////////////////////////////////////////////////////////////////////
typedef struct{
	
	ARRAY_PTR array;
	size_t size;
	type_size_t elem_size;
	uint32_t index;
	
}enumerator_t;


void enumerator_init(ARRAY_PTR array, size_t size, type_size_t elem_size, enumerator_t* out_enumerator);


generic_union_t enumerator_get_next(enumerator_t* enumerator,/* bool trim,*/ bool* valid, uint32_t* valuesLeft );

bool enumerator_set_next(enumerator_t* enumerator, generic_union_t value, uint32_t* valuesLeft );



#endif /* GENETICC_ENUMERATOR_H_ */
