#include "enumerator_geneticc.h"



void enumerator_init(ARRAY_PTR array, size_t size, type_size_t elem_size, enumerator_t* out_enumerator)
{
	out_enumerator->array = array;
	out_enumerator->size = size;
	out_enumerator->elem_size = elem_size;
}


#warning untested
generic_union_t enumerator_get_next(enumerator_t* enumerator, /*bool trim,*/ bool* valid, uint32_t* valuesLeft )
{
	size_t offset = enumerator->index++ * enumerator->elem_size;
	
	if(offset >= enumerator->size)
	{
		*valid = false;
		return (generic_union_t)0;
	}
	
	if(valuesLeft != NULL)
	*valuesLeft = (enumerator->size  - (offset + enumerator->elem_size)) / enumerator->elem_size;
	
	if(valid != NULL)
	*valid = true;
	
	generic_union_t value;// =trim ? (generic_union_t)trim_extra_bytes(((uint64_t*)enumerator->array)[offset], enumerator->elem_size) : (generic_union_t)((uint64_t*)enumerator->array)[offset];
	memcpy(&value, enumerator->array + offset, enumerator->elem_size);
	
	return value;
}

#warning tired, unfinished
#warning untested
bool enumerator_set_next(enumerator_t* enumerator, generic_union_t value, uint32_t* valuesLeft )
{
	size_t offset = enumerator->index++ * enumerator->elem_size;
	
	if(offset >= enumerator->size)
	{
		return false;
	}
	
	*valuesLeft = (enumerator->size  - (offset + enumerator->elem_size)) / enumerator->elem_size;
	
	uint64_t memval = value.u64;
	
	memcpy(enumerator->array + offset, &memval, enumerator->elem_size);
	
	return true;
}