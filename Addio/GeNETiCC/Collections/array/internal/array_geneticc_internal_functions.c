#include "array_geneticc_internal_functions.h"

#include <utils_assert.h>
#include <stddef.h>

#include "Addio/GeNETiCC/standard/memory/geneticc_memory.h"


/*
*	The assert calculation for determining an array will not grow larger than the max length.
*
*	Example: ASSERT(ARRAY_ASSERT_LENGTH(length + 100))
*
*	*I have set them both to true, as I am using an embedded system, and wi
*/
#define ARRAY_ASSERT_SIZE(newSize, elem_size)	true //ARRAY_MAX_LENGTH - (new_size / elem_size)
#define ARRAY_ASSERT_LENGTH(newLength)			true //ARRAY_MAX_LENGTH - newLength

#pragma region Generic Methods





/*
*	Search an array for a specific value, and returns the first index.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		Any value or struct that is in memory.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The index of the value in the array, or -1 if it does not exist.
*/
int internal_array_indexOf_memory(const ARRAY_PTR array, const ELEMENT_PTR value, size_t size,  element_size_t elem_size)
{
	for(int i = 0; i < size; i += elem_size)
	{
		uint8_t b = 0;
		for(b; b < elem_size; b++)
		{
			if(array[i + b] != value[b])
			{
				break;
			}
		}
		
		if(b == elem_size)
		{
			return i / elem_size;
		}
	}
	return -1;
}

/*
*	Search an array for a specific value, and returns the first index.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to find in the array. Can be any basic value type.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The index of the value in the array, or -1 if it does not exist.
*/
int __attribute__((__always_inline__)) internal_array_indexOf_generic(const ARRAY_PTR array, const generic_union_t value, size_t size,  element_size_t elem_size)
{
	return internal_array_indexOf_memory(array, &value, size, elem_size);
}


/*
*	Searches an array for a matching segment of memory, one byte at a time.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		Any value or struct that is in memory.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			If the array contains the value.
*/
bool __attribute__((__always_inline__)) internal_array_contains_memory(const ARRAY_PTR array, const ELEMENT_PTR value, size_t size,  element_size_t elem_size )
{
	return internal_array_indexOf_memory(array, value, size, elem_size) != -1;
}

/*
*	Searches an array for a value, and confirms its existence.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to find in the array. Can be any basic value type.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			If the array contains the value.
*/
bool __attribute__((__always_inline__)) internal_array_contains_generic(const ARRAY_PTR array, const generic_union_t value, size_t size,  element_size_t elem_size)
{
	return internal_array_contains_memory(array, &value, size, elem_size);
}

/*
*	Enumerates through an array checking the elements to the predicate, and returns true or false if an element satisfied the condition.
*
*	/param	array		Pointer to the start of the array.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	predicate	A function which checks each value to its conditions.
*
*	/returns			True if an element in the array satisfied the predicate's condition, false if the predicate was unsatisfied.
*/
bool __attribute__((__always_inline__)) internal_array_exists(const ARRAY_PTR array, size_t size,  element_size_t elem_size, PREDICATE predicate)
{
	return internal_array_select_memory(array, size, elem_size, predicate) != -1;
	//for(int i = 0; i < size; i += elem_size)
	//{
		//if((*predicate)(array + i))
		//{
			//return true;
		//}
	//}
//
	//return false;
}

/*
*	Searches an array from back to front for a value, and returns the index of the first match (latest index).
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to find in the array. Can be any basic value type.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The last index of the value in the array, or -1 if it does not exist.
*/
int internal_array_lastIndexOf_memory(const ARRAY_PTR array, const ELEMENT_PTR value, size_t size,  element_size_t elem_size)
{
	for(int i = size - elem_size; i >= 0; i -= elem_size)
	{
		uint8_t b = 0;
		for(b; b < elem_size; b++)
		{
			if(array[i + b] != value[b])
			{
				break;
			}
		}
		
		if(b == elem_size)
		{
			return i / elem_size;
		}
	}
	return -1;
}

/*
*	Searches an array for a value, and returns the last index.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to find in the array. Can be any basic value type.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The index of the value in the array, or -1 if it does not exist.
*/
int __attribute__((__always_inline__)) internal_array_lastIndexOf_generic(const ARRAY_PTR array, const generic_union_t value, size_t size,  element_size_t elem_size)
{
	return internal_array_lastIndexOf_memory(array, &value, size, elem_size);
}


/*
*	Searches an array for the largest value, and returns its index.
*
*	/param	array		Pointer to the start of the array.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	isSigned	Are the values signed?
*
*	/returns			The index of the largest value in the array.
*/
index_t internal_array_max_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, bool isSigned)
{
	int valueIndex = 0;		//The index of the largest value.
	int64_t value = 0;		//The value at valueIndex.
	int64_t compare = 0;	//The current value to compare.
	uint64_t uvalue = 0;	//The value at valueIndex.
	uint64_t ucompare = 0;	//The current value to compare.

	if(isSigned)
	{
		for(int i = 0; i < size; i += elem_size)
		{
			compare = 0;	//Reset compare
			switch(elem_size)
			{
				case 1:
				compare = array[i];
				break;
				case 2:
				compare = ((int16_t*)array)[i / 2];
				break;
				case 4:
				compare = ((int32_t*)array)[i / 4];
				break;
				case 8:
				compare = ((int64_t*)array)[i / 8];
				break;
			}
			
			if(compare > value)
			{
				value = compare;
				valueIndex = i;
			}
		}
	}
	else
	{
		for(int i = 0; i < size; i += elem_size)
		{
			compare = 0;	//Reset compare
			switch(elem_size)
			{
				case 1:
				ucompare = array[i];
				break;
				case 2:
				ucompare = ((uint16_t*)array)[i / 2];
				break;
				case 4:
				ucompare = ((uint32_t*)array)[i / 4];
				break;
				case 8:
				ucompare = ((uint64_t*)array)[i / 8];
				break;
			}
			
			if(ucompare > uvalue)
			{
				uvalue = ucompare;
				valueIndex = i;
			}
		}
	}

	
	return valueIndex / elem_size;
}


/*
*	Searches an array of floating points for the largest value.
*
*	/param	array		Pointer to the start of the array.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The index of the largest value in the array.
*/
index_t internal_array_maxf_memory(const double* array, size_t size,  element_size_t elem_size)
{
	int valueIndex = 0;		//The index of the largest value.
	double value = 0;		//The value at valueIndex.
	double compare = 0;	//The current value to compare.
	
	for(int i = 0; i < size; i += elem_size)
	{
		compare = 0;	//Reset compare
		
		switch(elem_size)
		{
			case 4:
			compare = (double)((float*)array)[i / 4];
			break;
			case 8:
			compare = (double)((double*)array)[i / 8];
			break;
		}
		
		if(compare > value)
		{
			value = compare;
			valueIndex = i;
		}
	}
	
	return valueIndex / elem_size;
}


/*
*	Searches an array for the smallest value.
*
*	/param	array		Pointer to the start of the array.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	isSigned	Are the values signed?
*
*	/returns			The index of the largest value in the array.
*/
index_t internal_array_min_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, bool isSigned)
{
	int valueIndex = 0;		//The index of the largest value.
	int64_t value = 0;		//The value at valueIndex.
	int64_t compare = 0;	//The current value to compare.
	uint64_t uvalue = 0;		//The value at valueIndex.
	uint64_t ucompare = 0;	//The current value to compare.
	

		
	if(isSigned)
	{
		for(int i = 0; i < size; i += elem_size)
		{
			compare = 0;	//Reset compare
			switch(elem_size)
			{
				case 1:
				compare = array[i];
				break;
				case 2:
				compare = ((int16_t*)array)[i / 2];
				break;
				case 4:
				compare = ((int32_t*)array)[i / 4];
				break;
				case 8:
				compare = ((int64_t*)array)[i / 8];
				break;
			}
			
			if(compare < value || i == 0)
			{
				value = compare;
				valueIndex = i;
			}
		}
	}
	else
	{
		for(int i = 0; i < size; i += elem_size)
		{
			compare = 0;	//Reset compare
			switch(elem_size)
			{
				case 1:
				ucompare = array[i];
				break;
				case 2:
				ucompare = ((uint16_t*)array)[i / 2];
				break;
				case 4:
				ucompare = ((uint32_t*)array)[i / 4];
				break;
				case 8:
				ucompare = ((uint64_t*)array)[i / 8];
				break;
			}
			
			if(ucompare < uvalue || i == 0)
			{
				uvalue = ucompare;
				valueIndex = i;
			}
		}
	}
	
	return valueIndex / elem_size;
}


/*
*	Searches an array of floating points for the smallest value.
*
*	/param	array		Pointer to the start of the array.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The index of the largest value in the array.
*/
index_t internal_array_minf_memory(const double* array, size_t size,  element_size_t elem_size)
{
	int valueIndex = 0;		//The index of the largest value.
	double value = 0;		//The value at valueIndex.
	double compare = 0;	//The current value to compare.
	
	for(int i = 0; i < size; i += elem_size)
	{
		compare = 0;	//Reset compare
		
		switch(elem_size)
		{
			case 4:
			compare = (double)((float*)array)[i / 4];
			break;
			case 8:
			compare = (double)((double*)array)[i / 8];
			break;
		}
		
		if(compare < value)
		{
			value = compare;
			valueIndex = i;
		}
	}
	
	return valueIndex / elem_size;
}

/*
*	Gets the sum of all values in the array.
*
*	/param	array		Pointer to the start of the array.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	isSigned	Are the values signed?
*
*	/returns			The average value of the array's contents.
*/
generic_union_t internal_array_sum_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, bool isSigned)
{
	int64_t value = 0;		//The sum of all contents
	uint64_t uvalue = 0;	//The sum of all contents (unsigned)

	
	
	if(isSigned)
	{
		for(int i = 0; i < size; i += elem_size)
		{
			switch(elem_size)
			{
				case 1:
				value = array[i];
				break;
				case 2:
				value += ((int16_t*)array)[i / 2];
				break;
				case 4:
				value += ((int32_t*)array)[i / 4];
				break;
				case 8:
				value += ((int64_t*)array)[i / 8];
				break;
			}
		}
	}
	else
	{
		for(int i = 0; i < size; i += elem_size)
		{
			switch(elem_size)
			{
				case 1:
				uvalue += array[i];
				break;
				case 2:
				uvalue += ((uint16_t*)array)[i / 2];
				break;
				case 4:
				uvalue += ((uint32_t*)array)[i / 4];
				break;
				case 8:
				uvalue += ((uint64_t*)array)[i / 8];
				break;
			}
		}
	}
	
	
	if(isSigned)
	return (generic_union_t)value;
	else
	return (generic_union_t)uvalue;
}


/*
*	Gets the sum of all values in the array.
*
*	/param	array		Pointer to the start of the array.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The average value of the array's contents.
*/
double internal_array_sumf_memory(const double* array, size_t size,  element_size_t elem_size)
{
	double value = 0;		//The sum of all contents
	
	for(int i = 0; i < size; i += elem_size)
	{
		switch(elem_size)
		{
			case 4:
			value += (double)((float*)array)[i / 4];
			break;
			case 8:
			value += (double)((double*)array)[i / 8];
			break;
		}
	}
	
	return value;
}

/*
*	Gets the average of all values in the array.
*
*	/param	array		Pointer to the start of the array.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	isSigned	Are the values signed?
*
*	/returns			The average value of the array's contents.
*/
generic_union_t internal_array_average_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, bool isSigned)
{
	if(isSigned)
	return (generic_union_t)(internal_array_sum_memory(array, size, elem_size, isSigned).s64 / (size / elem_size));
	else
	return (generic_union_t)(internal_array_sum_memory(array, size, elem_size, isSigned).u64 / (size / elem_size));
}


/*
*	Gets the average of all values in the array.
*
*	/param	array		Pointer to the start of the array.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The average value of the array's contents.
*/
double GENOPTI_ATT_FORCE_INLINE internal_array_averagef_memory(const double* array, size_t size,  element_size_t elem_size)
{
	return internal_array_sumf_memory(array, size, elem_size) / (size / elem_size);
}

/*
*	Counts how many times "value" is seen in the array.
*
*	/param	array		Pointer to the start of the array.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The amount of times "value" is in the array.
*/
unsigned int internal_array_valueCount_memory(const ELEMENT_PTR value, const ARRAY_PTR array, size_t size,  element_size_t elem_size)
{
	int offset = 0;
	int count = 0;
	
	while(offset < size)
	{
		int index = internal_array_indexOf_memory((array + offset), value, size - offset, elem_size);
		
		if(index == -1)
		break;
		
		count++;
		offset += (index == 0 ? elem_size : ((index + 1) * elem_size));
	}

	return count;
}

/*
*	Counts how many times "value" is seen in the array.
*
*	/param	array		Pointer to the start of the array.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The amount of times "value" is in the array.
*/
unsigned int __attribute__((__always_inline__)) internal_array_valueCount_generic(const ARRAY_PTR array, const generic_union_t value, size_t size,  element_size_t elem_size)
{
	return internal_array_valueCount_memory(array, &value, size, elem_size);
}

/*
*	Retrieves a pointer to the first value in the array that matches the predicate.
*
*	/param	array		Pointer to the start of the array.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	predicate	A function which checks each value to its conditions.
*
*	/returns			Zero based index of the first value in the array that matches the predicate, or -1 if the value does not exist.
*/
int internal_array_select_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, PREDICATE predicate)
{
	for(int i = 0; i < size; i += elem_size)
	{
		if((*predicate)(array + i))
		{
			return i = 0 ? 0 : i / elem_size;
		}
	}

	return -1;
}

/*
*	Retrieves a pointer to the first value in the array that matches the predicate.
*
*	/param	array		Pointer to the start of the array.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	predicate	A function which checks each value to its conditions, while accepting arguments in the form of a va_list.
*	/param	arg_count	How many arguments have been passed.
*	/param	...			The variadic arguments. *Will call va_end before returning.
*
*	/returns			Zero based index of the first value in the array that matches the predicate, or -1 if the value does not exist.
*/
int internal_array_select_args_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, PREDICATE_ARGS predicate, int arg_count, ...)
{
	va_list ap;
	va_start(ap, arg_count);
	return internal_array_select_vargs_memory(array, size, elem_size, predicate, arg_count, ap);
}


/*
*	Retrieves a pointer to the first value in the array that matches the predicate.
*
*	/param	array		Pointer to the start of the array.
*	/param	predicate	A function which checks each value to its conditions, while accepting arguments in the form of a va_list.
*	/param	arg_count	How many arguments have been passed.
*	/param	ap			The variadic arguments. *Will call va_end before returning.
*
*	/returns			Zero based index of the first value in the array that matches the predicate, or -1 if the value does not exist.
*/
int internal_array_select_vargs_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, PREDICATE_ARGS predicate, int arg_count, va_list ap)
{
	for(int i = 0; i < size; i += elem_size)
	{
		if((*predicate)(array + i, arg_count, ap ))
		{
			va_end(ap);
			return i = 0 ? 0 : i / elem_size;
		}
	}

	va_end(ap);
	
	return -1;
}



#pragma endregion Generic Methods

#pragma region Type Specific Methods


bool __attribute__((__always_inline__)) internal_array_contains_float(const float value, const float* array, length_t length)
{
	return internal_array_indexOf_float(value, array, length) != -1;
}

bool __attribute__((__always_inline__)) internal_array_contains_double(const double value, const double* array, length_t length)
{
	return internal_array_indexOf_double(value, array, length) != -1;
}

int internal_array_indexOf_float(const float value, const float* array, length_t length)
{
	for(int i = 0; i < length; i++)
	{
		if(array[i] == value)
		return i;
	}
	return -1;
}

int internal_array_indexOf_double(const double value, const double* array, length_t length)
{
	for(int i = 0; i < length; i++)
	{
		if(array[i] == value)
		return i;
	}
	return -1;
}

int internal_array_lastIndexOf_float(const float value, const float* array, length_t length)
{
	for(int i = length - 1; i >= 0; i--)
	{
		if(array[i] == value)
		return i;
	}
	return -1;
}

int internal_array_lastIndexOf_double(const double value, const double* array, length_t length)
{
	for(int i = length - 1;  i >= 0; i--)
	{
		if(array[i] == value)
		return i;
	}
	return -1;
}

#pragma endregion Type Specific Methods

#pragma region Unsafe Generic Methods

/*
*	Retrieves an array of pointers to the elements that match the conditions defined by the specified predicate.
*
*	/param	array		Pointer to the start of the array.
*	/param	length		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	predicate	A function which checks each value to its conditions.
*	/param	out_length	The amount of matches in the array.
*
*	/returns			An array of pointers to each value in the array that matched the predicate's conditions
*/
uint8_t** internal_array_selectMany_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, PREDICATE predicate, length_t* out_length)
{
	size_t capacity = 10;
	length_t length = 0;
	
	uint8_t** values = malloc(capacity * sizeof(uint8_t*) );
	
	for(int i = 0; i < size; i += elem_size)
	{
		if((*predicate)(array + i))
		{
			values[length] = (array + i);
			(length)++;
			
			if(length == capacity)
			{
				capacity *= 2;
				values = (values, capacity * sizeof(uint8_t*) );
			}
		}
	}
	
	values = realloc(values, length * sizeof(uint8_t*) );
	
	if(out_length != NULL)
	*out_length = length;
	
	return values;
}


/*
*	Retrieves an array of pointers to the elements that match the conditions defined by the specified predicate.
*
*	/param	array		Pointer to the start of the array.
*	/param	length		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	predicate	A function which checks each value to its conditions.
*	/param	arg_count	How many arguments have been passed.
*	/param	ap			The variadic arguments. *Will call va_end before returning.
*	/param	out_length	The amount of matches in the array.
*
*	/returns			An array of pointers to each value in the array that matched the predicate's conditions
*/
uint8_t** internal_array_selectMany_vargs_memory(const ARRAY_PTR array, size_t size,  element_size_t elem_size, PREDICATE_ARGS predicate, int arg_count, va_list ap, length_t* out_length)
{
	size_t capacity = 10;
	length_t length = 0;
	
	uint8_t** values = malloc(capacity * sizeof(uint8_t*) );
	
	for(int i = 0; i < size; i += elem_size)
	{
		if((*predicate)(array + i, arg_count, ap))
		{
			values[length] = (array + i);
			(length)++;
			
			if(length == capacity)
			{
				capacity *= 2;
				values = (values, capacity * sizeof(uint8_t*) );
			}
		}
	}
	
	values = realloc(values, length * sizeof(uint8_t*) );
	
	if(out_length != NULL)
	*out_length = length;
	
	return values;
}

/*
*	Inserts a range of values into an array.
*	Works by allocating enough memory to fit the original array, plus the new values.
*	It then copies the array and the values into the new array, and returns a pointer to where the new array was allocated.
*
*	/param	array		Pointer to the start of the array.
*	/param	range		Pointer to where the values to insert starts.
*	/param	offset		The byte offset where the new values should be inserted at.
*	/param	size		The size of the array (in bytes).
*	/param	byte_count	The amount of bytes being inserted (element count x elem_size)
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	free_old*	If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*
*	/returns			A pointer to the start of the new array.
*/
ARRAY_PTR internal_array_insertRange_memory(ARRAY_PTR array, const ELEMENT_PTR range, int offset, size_t byte_count, size_t size,  element_size_t elem_size, bool free_old )
{
	ASSERT(byte_count && ARRAY_MAX_SIZE - (size + byte_count) );

	//Allocate new array.
	uint8_t* newArray = malloc(size + byte_count);
	
	//Copy data before the inserted range into our new array.
	if(offset > 0)
		geneticc_memcpy(newArray, array, offset);
		
		
	//Place our range into the new array.
	geneticc_memcpy( (newArray + offset), range, byte_count);
	
	
	//Copy data after the inserted range into our new array.
	if(offset < size)
		geneticc_memcpy(newArray + offset + byte_count, array + offset, size - offset);
	
	if(free_old)
		free(array);
	
	return newArray;
}

/*
*	Inserts a value into an array.
*	Works by allocating enough memory to fit the original array, plus the new value.
*	It then copies the array and the value into the new array, and returns a pointer to where the new array was allocated.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		Pointer to where the value to insert starts.
*	/param	offset		The byte offset where the new values should be inserted at.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	free_old*	If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*
*	/returns			A pointer to the start of the new array.
*/
ARRAY_PTR __attribute__((__always_inline__)) internal_array_insert_memory(ARRAY_PTR array, const ELEMENT_PTR value, int offset,  size_t size,  element_size_t elem_size, bool free_old )
{
	return internal_array_insertRange_memory(array, value, offset,  (1 * elem_size), size, elem_size, free_old);
}

/*
*	Inserts a value into an array.
*	Works by allocating enough memory to fit the original array, plus the new value.
*	It then copies the array and the value into the new array, and returns a pointer to where the new array was allocated.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to insert.
*	/param	offset		The byte offset where the new values should be inserted at.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	free_old*	If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*
*	/returns			A pointer to the start of the new array.
*/
ARRAY_PTR __attribute__((__always_inline__)) internal_array_insert_generic(ARRAY_PTR array, const generic_union_t value, int offset,  size_t size,  element_size_t elem_size, bool free_old)
{
	return internal_array_insertRange_memory(array, &value, offset,  (1 * elem_size), size, elem_size, free_old);
}

///*
//*	Adds a range of values into the end of an array.
//*	*Calculates the offset and then calls array_insertRange_memory.
//*
//*/param	array		Pointer to the start of the array.
//*	/param	value		Pointer to where the values to add starts.
//*	/param	byte_count	The amount of bytes being inserted (element count x elem_size)
//*	/param	size		The size of the array (in bytes).
//*	/param	elem_size	The size of the value's type (in bytes).
//*	/param	free_old*	If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
//*
//*	/returns			A pointer to the start of the new array.
//*/
//ARRAY_PTR array_addRange_memory(const ELEMENT_PTR value, ARRAY_PTR array, size_t byte_count, size_t size,  element_size_t elem_size, bool free_old )
//{
	//return array_insertRange_memory(value, (size / elem_size), array, byte_count, size, elem_size, free_old); 
//}

/*
*	Adds a value to the end of an array.
*	*Calculates the offset and then calls array_insertRange_memory with a count of 1.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		Pointer to the value or struct to add.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	free_old*	If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*
*	/returns			A pointer to the start of the new array.
*/
ARRAY_PTR __attribute__((__always_inline__)) internal_array_add_memory(ARRAY_PTR array, const ELEMENT_PTR value, size_t size,  element_size_t elem_size, bool free_old )
{
	return internal_array_insertRange_memory(array, value, size, (1 * elem_size), size, elem_size, free_old); 
}

/*
*	Adds a value to the end of an array.
*	*Calculates the offset and then calls array_insertRange_memory with a count of 1.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to add.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	free_old*	If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*
*	/returns			A pointer to the start of the new array.
*/
ARRAY_PTR __attribute__((__always_inline__)) internal_array_add_generic(ARRAY_PTR array, const generic_union_t value, size_t size,  element_size_t elem_size, bool free_old)
{
	return internal_array_insertRange_memory(array, &value, size, (1 * elem_size), size, elem_size, free_old);
}

///*
//*	Adds a range of values into the end of an array.
//*	*Calculates the offset and then calls array_insertRange_memory.
//*
//*	/param	value		Pointer to where the values to prepend starts.
//*	/param	array		Pointer to the start of the array.
//*	/param	byte_count	The amount of bytes being inserted (element count x elem_size)
//*	/param	size		The size of the array (in bytes).
//*	/param	elem_size	The size of the value's type (in bytes).
//*	/param	free_old*	If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
//*
//*	/returns			A pointer to the start of the new array.
//*/
//ARRAY_PTR array_prependRange_memory(const ELEMENT_PTR value,  ARRAY_PTR array, size_t byte_count, size_t size,  element_size_t elem_size, bool free_old )
//{
	//return array_insertRange_memory(value, 0, array, byte_count, size,  elem_size, free_old);
//}

/*
*	Adds a value to the end of an array.
*	*Calculates the offset and then calls array_insertRange_memory with a count of 1.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		Pointer to the value or struct to prepend.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	free_old*	If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*
*	/returns			A pointer to the start of the new array.
*/
ARRAY_PTR __attribute__((__always_inline__)) internal_array_prepend_memory(ARRAY_PTR array, const ELEMENT_PTR value,  size_t size,  element_size_t elem_size, bool free_old )
{
	return internal_array_insertRange_memory(array, value, 0, (1 * elem_size), size,elem_size, free_old);
}

/*
*	Adds a value to the end of an array.
*	*Calculates the offset and then calls array_insertRange_memory with a count of 1.
*
*	/param	array		Pointer to the start of the array.
*	/param	value		The value to prepend.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	free_old*	If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*
*	/returns			A pointer to the start of the new array.
*/
ARRAY_PTR __attribute__((__always_inline__)) internal_array_prepend_generic(ARRAY_PTR array, const generic_union_t value, size_t size,  element_size_t elem_size, bool free_old)
{
	return internal_array_insertRange_memory(array, &value, 0,  (1 * elem_size), size, elem_size, free_old);
}

/*
*	Removes a range of values from an array.
*	Works by allocating memory to fit the original array, minus the count.
*	It then copies the data surrounding offset + count from the old array to the new, and returns a pointer to where the new array was allocated.
*
*	/param	array		Pointer to the start of the array.
*	/param	offset		The byte offset to start removing at.
*	/param	byte_count	The amount of bytes being removed (element count x elem_size)
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	free_old*	If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*
*	/returns			A pointer to the start of the new array.
*/
ARRAY_PTR internal_array_removeRange_memory(ARRAY_PTR array, int offset, size_t byte_count, size_t size,  element_size_t elem_size, bool free_old )
{
	ASSERT(byte_count);

	//Allocate new array.
	uint8_t* newArray = malloc(size - byte_count);
	
	//Copy data before the removed values into our new array.
	if(offset > 0)
		geneticc_memcpy(newArray, array, offset);
	
	
	//Copy data after the removed values into our new array.
	if(offset < size)
	geneticc_memcpy(newArray + offset, array + offset + byte_count, size - offset - byte_count);
	
	if(free_old)
	free(array);
	
	return newArray;
	
}

/*
*	Removes a values from an array by its byte offset.
*
*	/param	array		Pointer to the start of the array.
*	/param	offset		The byte offset to start removing at.
*	/param	size		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*	/param	free_old*	If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*
*	/returns			A pointer to the start of the new array.
*/
ARRAY_PTR __attribute__((__always_inline__)) internal_array_removeAt_memory(ARRAY_PTR array, int offset, size_t size,  element_size_t elem_size, bool free_old )
{
	internal_array_removeRange_memory(array, offset, elem_size, size, elem_size, free_old);
}

/*
*	Removes the first item from an array which equals "value."
*
*	/param	array		Pointer to the start of the array.
*	/param	value			A pointer to the value to remove
*	/param	size			The size of the array (in bytes).
*	/param	elem_size		The size of the value's type (in bytes).
*	/param	free_old*		If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*	/param	out_removed*	(Out)Returns true or false depending if a value was found and removed.
*
*	/returns				A pointer to the start of the new array.
*/
ARRAY_PTR internal_array_remove_memory(ARRAY_PTR array, const ELEMENT_PTR value, size_t size,  element_size_t elem_size, bool free_old, bool* out_removed )
{
	//Element index
	int index = internal_array_indexOf_memory(array, value, size, elem_size);
	
	//Value not in array, nothing to remove.
	if(index == -1)
	{
		//Mark that nothing was removed
		//Return original array.
		if(out_removed != NULL)
		*out_removed = false;
		return array;
	}
	
	//Mark that a value was removed
	if(out_removed != NULL)
	*out_removed = true;
	
	return internal_array_removeAt_memory(array, (index * elem_size), size, elem_size, free_old);
}


/*
*	Removes the first item from an array which equals "value."
*
*	/param	array		Pointer to the start of the array.
*	/param	value			The value to remove
*	/param	size			The size of the array (in bytes).
*	/param	elem_size		The size of the value's type (in bytes).
*	/param	free_old*		If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*	/param	out_removed*	(Out)Returns true or false depending if a value was found and removed.
*
*	/returns				A pointer to the start of the new array.
*/
ARRAY_PTR __attribute__((__always_inline__)) internal_array_remove_generic(ARRAY_PTR array, const generic_union_t value, size_t size,  element_size_t elem_size, bool free_old, bool* out_removed)
{
	return internal_array_remove_memory(array, &value, size, elem_size, free_old, out_removed);
}

/*
*	Removes all items from an array which equals "value."
*
*	/param	array		Pointer to the start of the array.
*	/param	value			A pointer to the value which is used to determine which elements to remove.
*	/param	size			The size of the array (in bytes).
*	/param	elem_size		The size of the value's type (in bytes).
*	/param	free_old*		If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*	/param	out_removed*	(Out)Returns amount of values removed from the array.
*
*	/returns				A pointer to the start of the new array.
*/
ARRAY_PTR internal_array_removeAll_memory(ARRAY_PTR array, const ELEMENT_PTR value, size_t size,  element_size_t elem_size, bool free_old, unsigned int* out_count )
{
	uint8_t* newArray = malloc(size);
	
	size_t newSize = 0;

	for(int i = 0; i < size; i += elem_size)	
	{
		uint8_t b = 0;
		
		for(b; b < elem_size; b++)
		{
			if(array[i + b] != value[b])
			{
				break;			
			}
			else
			{
				//Copy into memory while we are here, if we end up getting rid of the value we will overwrite.
				newArray[newSize + b] = array[i + b];
			}
		}
		
		if(b == elem_size)
			(*out_count)++;			//Matching value, overwrite, and increase our remove counter.
		else
			newSize += elem_size;	//Not a matching value, don't overwrite
			
	}
	
	newArray = (newArray, (newSize * elem_size));
	
	if(free_old)
		free(array);
	
	return newArray;
}

/*
*	Removes all items from an array which equals "value."
*
*	/param	array		Pointer to the start of the array.
*	/param	value			The value which is used to determine which elements to remove.
*	/param	size			The size of the array (in bytes).
*	/param	elem_size		The size of the value's type (in bytes).
*	/param	free_old*		If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*	/param	out_removed*	(Out)Returns amount of values removed from the array.
*
*	/returns				A pointer to the start of the new array.
*/
ARRAY_PTR __attribute__((__always_inline__)) internal_array_removeAll_generic(ARRAY_PTR array, const generic_union_t value, size_t size,  element_size_t elem_size, bool free_old, unsigned int* out_count )
{
	return internal_array_removeAll_memory(array, &value, size, elem_size, free_old, out_count);
}

/*
*	Reverse the array in memory.
*
*	/param	array			Pointer to the start of the array.
*	/param	size			The size of the array (in bytes).
*	/param	elem_size		The size of the value's type (in bytes).
*	/param	free_old*		If true, the array passed to "array" will be freed after the copy. (ARRAY_UNSAFE_REALLOC must be undefined)
*
*	/returns				A pointer to the start of the new array.
*/
ARRAY_PTR internal_array_reverse_memory(ARRAY_PTR array, size_t size,  element_size_t elem_size, bool free_old)
{
	uint8_t* newArray = malloc(size);
	
	for(int i = 0, b = size - elem_size; i < size; i += elem_size, b -= elem_size)
	{
		for(uint8_t c = 0; c < elem_size; c++)
		{
			newArray[i + c] = array[b + c];
		}		
	}

	if(free_old)
	free(array);
	
	return newArray;
}



//ARRAY_PTR internal_array_sort_memory(ARRAY_PTR array, size_t size,  element_size_t elem_size, COMPARISON comparison, bool free_old)
//{
	//
//
	//if(free_old)
	//free(array);
	//
	//return newArray;
//}


/*
*	Assigns the given value to the elements of the specified array.
*
*	/param	array			Pointer to the start of the array.
*	/param	value			Pointer to the value which will be assigned to each element of the array.
*	/param	size			The size of the array (in bytes), or how many bytes to set.
*	/param	elem_size		The size of the value's type (in bytes).
*/
void internal_array_fill_memory(ARRAY_PTR array, ELEMENT_PTR value, size_t size, element_size_t elem_size)
{
	for(int i = 0; i < size; i += elem_size)
	{
		memcpy(array + i, value + i, elem_size);
	}
}

/*
*	Assigns the given value to the elements of the specified array.
*
*	/param	array			Pointer to the start of the array.
*	/param	value			Assigns the given value to each element of the specified array.
*	/param	size			The size of the array (in bytes), or how many bytes to set.
*	/param	elem_size		The size of the value's type (in bytes).
*/
void __attribute__((__always_inline__)) internal_array_fill_generic(ARRAY_PTR array, generic_union_t value, size_t size, element_size_t elem_size)
{
	return internal_array_fill_memory(array, &value, size, elem_size);
}

#pragma endregion Unsafe Generic Methods

#pragma region Unsafe Conversion Methods

#warning tired
#warning untested
#warning reminder to time each function, and add if GENETICC_OPTIMIZATION_LEVEL



/*
*	Array ConvertTo(Cast)
*
*	*Very slow!
*
*	/param		name	desc
*
*	/returns	type	desc
*/
uint8_t* internal_array_convertTo_cast(ARRAY_PTR array, size_t length, type_sized_t oldType, type_sized_t newType, bool free_old)
{
	int8_t old_elem_size = geneticc_get_sized_type_abs(oldType);
	int8_t new_elem_size = geneticc_get_sized_type_abs(newType);
	
	uint32_t count = (length / old_elem_size);
	size_t new_size = count * new_elem_size;	
		
	uint8_t* newArray = malloc(new_size);
	
	for(int i = 0, offset = 0; i < length; i += old_elem_size, offset += new_elem_size)
	{
		generic_union_t value = (generic_union_t)(*((uint64_t*)(array + i)));		
		value = geneticc_cast(value, oldType, newType, old_elem_size, new_elem_size);
		geneticc_memcpy(newArray + offset, &value, new_elem_size);
		//newArray[offset] = value;
	}
	
	if(free_old)
	free(array);
	
	return newArray;
}

/*
*	Nasty
*/
#warning untested
uint8_t* internal_array_convertTo_memory(ARRAY_PTR array, size_t length, type_sized_t oldType, type_sized_t newType, bool free_old)
{
	signed_type_size_t old_elem_size = geneticc_get_sized_type_abs(oldType);
	signed_type_size_t new_elem_size = geneticc_get_sized_type_abs(newType);
	
	switch(oldType)
	{
		case TYPE_SIZED_I8:
		case TYPE_SIZED_U8:
		case TYPE_SIZED_I16:
		case TYPE_SIZED_U16:
		case TYPE_SIZED_I32:
		case TYPE_SIZED_U32:
		case TYPE_SIZED_I64:
		case TYPE_SIZED_U64:
		switch(newType)
		{
			case TYPE_SIZED_I8:
			case TYPE_SIZED_U8:
			case TYPE_SIZED_I16:
			case TYPE_SIZED_U16:
			case TYPE_SIZED_I32:
			case TYPE_SIZED_U32:
			case TYPE_SIZED_I64:
			case TYPE_SIZED_U64:
			case TYPE_SIZED_BOOL:
			return internal_array_convertToInteger_memory(array, length, old_elem_size, new_elem_size, free_old);
			case TYPE_SIZED_FLOAT:
			return internal_array_convertToFloatFromInteger_memory(array, length, oldType, free_old);
			case TYPE_SIZED_DOUBLE:
			return internal_array_convertToDoubleFromInteger_memory(array, length, old_elem_size, free_old);
			default:
			ASSERT(false);
			break;
			
		}
		break;
		
		case TYPE_SIZED_FLOAT:
		switch(newType)
		{
			case TYPE_SIZED_I8:
			case TYPE_SIZED_U8:
			case TYPE_SIZED_I16:
			case TYPE_SIZED_U16:
			case TYPE_SIZED_I32:
			case TYPE_SIZED_U32:
			case TYPE_SIZED_I64:
			case TYPE_SIZED_U64:
			case TYPE_SIZED_BOOL:
			return internal_array_convertToIntegerFromFloat_memory(array, length / old_elem_size, new_elem_size, free_old);
			case TYPE_SIZED_FLOAT:
			return array;
			case TYPE_SIZED_DOUBLE:
			return internal_array_convertToDoubleFromFloat_memory(array, length / old_elem_size, free_old);
			default:
			ASSERT(false);
			break;
		}
		break;
		
		case TYPE_SIZED_DOUBLE:
		switch(newType)
		{
			case TYPE_SIZED_I8:
			case TYPE_SIZED_U8:
			case TYPE_SIZED_I16:
			case TYPE_SIZED_U16:
			case TYPE_SIZED_I32:
			case TYPE_SIZED_U32:
			case TYPE_SIZED_I64:
			case TYPE_SIZED_U64:
			case TYPE_SIZED_BOOL:
			return internal_array_convertToIntegerFromDouble_memory(array, length / old_elem_size, new_elem_size, free_old);
			case TYPE_SIZED_FLOAT:
			return internal_array_convertToFloatFromDouble_memory(array, length / old_elem_size, free_old);
			case TYPE_SIZED_DOUBLE:
			return array;
			default:
			ASSERT(false);
			break;
		}
		break;
	}
}


#warning finish rest of methods to be like float
//#error
uint8_t* internal_array_convertToInteger_memory(ARRAY_PTR array, size_t length, signed_type_size_t old_elem_size, signed_type_size_t new_elem_size, bool free_old )
{
	size_t new_size = (length / old_elem_size) * new_elem_size;
	
	uint8_t* newArray = malloc(new_size);
	
	for(int i = 0; i < (new_size / old_elem_size); i++)
	{
		if(old_elem_size < 0)
		{
			int64_t saint = 0;
			
			old_elem_size *= -1;
			
			if(old_elem_size == 4)
			{
				saint = (int64_t)((int32_t*)array)[i];
			}
			else if(old_elem_size == 2)
			{
				saint = (int64_t)((int16_t*)array)[i];
			}
			else if(old_elem_size == 1)
			{
				saint = (int64_t)((int8_t*)array)[i];
			}
			else if(old_elem_size == 8)
			{
				saint = (int64_t)((int64_t*)array)[i];
			}
			
			if(new_elem_size < 0)
			{
				if(new_elem_size == 4)
				{
					((int32_t*)newArray)[i] = (int32_t)saint;
				}
				else if(new_elem_size == 2)
				{
					((int16_t*)newArray)[i] = (int16_t)saint;
				}
				else if(new_elem_size == 1)
				{
					((int8_t*)newArray)[i] = (int8_t)saint;
				}
				else if(new_elem_size == 8)
				{
					((int64_t*)newArray)[i] = (int64_t)saint;
				}
			}
			else
			{
				if(new_elem_size == 4)
				{
					((uint32_t*)newArray)[i] = (uint32_t)saint;
				}
				else if(new_elem_size == 2)
				{
					((uint16_t*)newArray)[i] = (uint16_t)saint;
				}
				else if(new_elem_size == 1)
				{
					((uint8_t*)newArray)[i] = (uint8_t)saint;
				}
				else if(new_elem_size == 8)
				{
					((uint64_t*)newArray)[i] = (uint64_t)saint;
				}
			}
		}
		else
		{
			uint64_t aint = 0;
			
			if(old_elem_size == 4)
			{
				aint = (uint64_t)((uint32_t*)array)[i];
			}
			else if(old_elem_size == 2)
			{
				aint = (uint64_t)((uint16_t*)array)[i];
			}
			else if(old_elem_size == 1)
			{
				aint = (uint64_t)((uint8_t*)array)[i];
			}
			else if(old_elem_size == 8)
			{
				aint = (uint64_t)((uint64_t*)array)[i];
			}
			
			if(new_elem_size < 0)
			{
				if(new_elem_size == 4)
				{
					((int32_t*)newArray)[i] = (int32_t)aint;
				}
				else if(new_elem_size == 2)
				{
					((int16_t*)newArray)[i] = (int16_t)aint;
				}
				else if(new_elem_size == 1)
				{
					((int8_t*)newArray)[i] = (int8_t)aint;
				}
				else if(new_elem_size == 8)
				{
					((int64_t*)newArray)[i] = (int64_t)aint;
				}
			}
			else
			{
				if(new_elem_size == 4)
				{
					((uint32_t*)newArray)[i] = (uint32_t)aint;
				}
				else if(new_elem_size == 2)
				{
					((uint16_t*)newArray)[i] = (uint16_t)aint;
				}
				else if(new_elem_size == 1)
				{
					((uint8_t*)newArray)[i] = (uint8_t)aint;
				}
				else if(new_elem_size == 8)
				{
					((uint64_t*)newArray)[i] = (uint64_t)aint;
				}
			}
		}
	}
	
	if(free_old)
	free(array);
	
	return newArray;
}

uint8_t* internal_array_convertToIntegerFromFloat_memory(float* array, length_t length, int8_t new_elem_size, bool free_old )
{
	size_t new_size = (length / new_elem_size);
	
	uint8_t* newArray = malloc(new_size);
	
	for(int i = 0; i < length; i++)
	{
		float val = array[i];
		
		if(new_elem_size < 0)
		{
			if(new_elem_size == 4)
			{
				((int32_t*)newArray)[i] = (int32_t)val;
			}
			else if(new_elem_size == 2)
			{
				((int16_t*)newArray)[i] = (int16_t)val;
			}
			else if(new_elem_size == 1)
			{
				((int8_t*)newArray)[i] = (int8_t)val;
			}
			else if(new_elem_size == 8)
			{
				((int64_t*)newArray)[i] = (int64_t)val;
			}
		}
		else
		{
			if(new_elem_size == 4)
			{
				((uint32_t*)newArray)[i] = (uint32_t)val;
			}
			else if(new_elem_size == 2)
			{
				((uint16_t*)newArray)[i] = (uint16_t)val;
			}
			else if(new_elem_size == 1)
			{
				((uint8_t*)newArray)[i] = (uint8_t)val;
			}
			else if(new_elem_size == 8)
			{
				((uint64_t*)newArray)[i] = (uint64_t)val;
			}
		}
	}
	
	if(free_old)
	free(array);
	
	return newArray;
}

uint8_t* internal_array_convertToIntegerFromDouble_memory(double* array, length_t length, int8_t new_elem_size, bool free_old )
{
	size_t new_size = (length / new_elem_size);
	
	uint8_t* newArray = malloc(new_size);
	
	for(int i = 0; i < length; i++)
	{
		double val = array[i];
		
		if(new_elem_size < 0)
		{
			if(new_elem_size == 4)
			{
				((int32_t*)newArray)[i] = (int32_t)val;
			}
			else if(new_elem_size == 2)
			{
				((int16_t*)newArray)[i] = (int16_t)val;
			}
			else if(new_elem_size == 1)
			{
				((int8_t*)newArray)[i] = (int8_t)val;
			}
			else if(new_elem_size == 8)
			{
				((int64_t*)newArray)[i] = (int64_t)val;
			}
		}
		else
		{
			if(new_elem_size == 4)
			{
				((uint32_t*)newArray)[i] = (uint32_t)val;
			}
			else if(new_elem_size == 2)
			{
				((uint16_t*)newArray)[i] = (uint16_t)val;
			}
			else if(new_elem_size == 1)
			{
				((uint8_t*)newArray)[i] = (uint8_t)val;
			}
			else if(new_elem_size == 8)
			{
				((uint64_t*)newArray)[i] = (uint64_t)val;
			}
		}
	}
	
	if(free_old)
	free(array);
	
	return newArray;
}

#warning come back here to change other methods to be like this
float* internal_array_convertToFloatFromInteger_memory(int8_t* array, size_t length, type_sized_t old_elem_type, bool free_old )
{
	signed_type_size_t old_elem_size = geneticc_get_sized_type_abs(old_elem_type);//old_elem_type == 0 ? sizeof(bool) : old_elem_type;
	size_t new_size = (length / old_elem_size) * sizeof(float);
	
	if(old_elem_type == TYPE_SIZED_BOOL)
		old_elem_type = old_elem_size;
	
	float* newArray = malloc(new_size);
	
	#if GENETICC_OPTIMIZATION_LEVEL == GENETICC_OPTIMIZATION_LEVEL_SIZE
	
	for(int i = 0; i < (new_size / old_elem_size); i++)
	{
		int64_t saint = 0;
		uint64_t aint = 0;
		switch(old_elem_type)
		{
			case TYPE_SIZED_I8:
				saint = ((int64_t)((int8_t*)array)[i]);
				goto isSigned;
			case TYPE_SIZED_U8:
				aint = ((uint64_t)((uint8_t*)array)[i]);
				goto isUnSigned;
			case TYPE_SIZED_I16:
				saint = ((int64_t)((int16_t*)array)[i]);
				goto isSigned;
			case TYPE_SIZED_U16:
				aint = ((uint64_t)((uint16_t*)array)[i]);
				goto isUnSigned;
			case TYPE_SIZED_I32:
				saint = ((int64_t)((int32_t*)array)[i]);
				goto isSigned;
			case TYPE_SIZED_U32:
				aint = ((uint64_t)((uint32_t*)array)[i]);
				goto isUnSigned;
			case TYPE_SIZED_I64:
				saint = ((int64_t)((int64_t*)array)[i]);
				goto isSigned;
			case TYPE_SIZED_U64:
				aint = ((uint64_t)((uint64_t*)array)[i]);
				goto isUnSigned;
			default:
				ASSERT(false);
				break;
			
			isSigned:
				newArray[i] = (float)saint;
				break;
			isUnSigned:
				newArray[i] = (float)aint;
				break;
			
		}
	}
	
	#else
		
	int64_t saint = 0;
	uint64_t aint = 0;
	switch(old_elem_type)
	{
		case TYPE_SIZED_I8:
			for(int i = 0; i < (new_size / old_elem_size); i++)
			{
				saint = ((int64_t)((int8_t*)array)[i]);
				newArray[i] = (float)saint;
			}
			break;
		case TYPE_SIZED_U8:
			for(int i = 0; i < (new_size / old_elem_size); i++)
			{
				aint = ((uint64_t)((uint8_t*)array)[i]);
				newArray[i] = (float)aint;
			}
			break;
		case TYPE_SIZED_I16:
			for(int i = 0; i < (new_size / old_elem_size); i++)
			{
				saint = ((int64_t)((int16_t*)array)[i]);
				newArray[i] = (float)saint;
			}
			break;
		case TYPE_SIZED_U16:
			for(int i = 0; i < (new_size / old_elem_size); i++)
			{
				aint = ((uint64_t)((uint16_t*)array)[i]);
				newArray[i] = (float)aint;
			}
			break;
		case TYPE_SIZED_I32:
			for(int i = 0; i < (new_size / old_elem_size); i++)
			{
				saint = ((int64_t)((int32_t*)array)[i]);
				newArray[i] = (float)saint;
			}
			break;
		case TYPE_SIZED_U32:
			for(int i = 0; i < (new_size / old_elem_size); i++)
			{
				aint = ((uint64_t)((uint32_t*)array)[i]);
				newArray[i] = (float)aint;
			}
			break;
		case TYPE_SIZED_I64:
			for(int i = 0; i < (new_size / old_elem_size); i++)
			{
				saint = ((int64_t)((int64_t*)array)[i]);
				newArray[i] = (float)saint;
			}
			break;
		case TYPE_SIZED_U64:
			for(int i = 0; i < (new_size / old_elem_size); i++)
			{
				aint = ((uint64_t)((uint64_t*)array)[i]);
				newArray[i] = (float)aint;
			}
			break;
		default:
			ASSERT(false);
			break;				
	}
	
	#endif
	
	if(free_old)
	free(array);
	
	return newArray;
}

double* internal_array_convertToDoubleFromInteger_memory(int8_t* array, size_t length, signed_type_size_t old_elem_size, bool free_old )
{
	size_t new_size = (length / old_elem_size) * sizeof(double);
	
	double* newArray = malloc(new_size);
	
	for(int i = 0; i < (new_size / old_elem_size); i++)
	{
		if(old_elem_size < 0)
		{
			int64_t saint = 0;
			if(old_elem_size == 4)
			{
				saint = ((int64_t)((int32_t*)array)[i]);
			}
			else if(old_elem_size == 2)
			{
				saint = ((int64_t)((int16_t*)array)[i]);
			}
			else if(old_elem_size == 1)
			{
				saint = ((int64_t)((int8_t*)array)[i]);
			}
			else if(old_elem_size == 8)
			{
				saint = ((int64_t)((int64_t*)array)[i]);
			}
			
			newArray[i] = (double)saint;
		}
		else
		{
			uint64_t aint = 0;
			if(old_elem_size == 4)
			{
				aint = ((uint64_t)((uint32_t*)array)[i]);
			}
			else if(old_elem_size == 2)
			{
				aint = ((uint64_t)((uint16_t*)array)[i]);
			}
			else if(old_elem_size == 1)
			{
				aint = ((uint64_t)((uint8_t*)array)[i]);
			}
			else if(old_elem_size == 8)
			{
				aint = ((uint64_t)((uint64_t*)array)[i]);
			}
			
			newArray[i] = (double)aint;
		}
	}
	
	if(free_old)
	free(array);
	
	return newArray;
}

double* internal_array_convertToDoubleFromFloat_memory(float* array, length_t length, bool free_old )
{
	double* newArray = malloc(length * (sizeof(double) ));
	
	for(int i = 0; i < length; i++)
	{
		newArray[i] = (double)array[i];
	}
	
	if(free_old)
	free(array);
	
	return newArray;
}

float* internal_array_convertToFloatFromDouble_memory(double* array, length_t length, bool free_old )
{
	float* newArray = malloc(length * (sizeof(float) ));
	
	for(int i = 0; i < length; i++)
	{
		newArray[i] = (float)array[i];
	}
	
	if(free_old)
	free(array);
	
	return newArray;
}

#pragma endregion Unsafe Conversion Methods

#pragma region DMA Functions
#if GENETICC_USE_DMA == true
#include "Addio/GeNETiCC/dma/geneticc_dma.h"

bool internal_array_copy_dma(ARRAY_PTR array, ARRAY_PTR dest, size_t size, bool free_source)
{
	if(geneticc_dma_size_ok(size, BEATSIZE_MAX))
	{
		geneticc_dma_transfer_t* transfer = geneticc_dma_create_transfer(dest, array, size, free_source);
		
		size_t transfersize = size;
		size_t leftovers = 0;
	
		transfersize = geneticc_dma_get_transfer_size(0, size, &leftovers);	
		geneticc_dma_subtransfer_t* subtransfer = geneticc_dma_create_subtransfer(dest, array, transfersize, transfer, NULL);	
		//geneticc_dma_subtransfer_init(dest, array, transfersize, &transfer, &subtransfer, false, BEATSIZE_MAX);
		
		geneticc_dma_register_transfer(transfer);
		
		memcpy(dest + transfersize, array + transfersize, leftovers);	//Copy the bytes that were unable to transferred with the beatsize.
		return true;
	}
	else
	{
		memcpy(dest, array, size);
		return true;
	}
	
	
	return false;
}



#endif
#pragma endregion DMA Functions


