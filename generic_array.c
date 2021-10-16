#include "generic_array.h"

#pragma region Generic Methods

/*
*	Searches an array for a matching segment of memory, one byte at a time.
*
*	/param	value		Any value or struct that is in memory.
*	/param	array		Pointer to the start of the array.
*	/param	length		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			If the array contains the value.
*/
bool array_contains_memory(const uint8_t* value, const uint8_t* array, size_t length, uint8_t elem_size )
{
	return array_indexOf_memory(value, array, length, elem_size) != -1;
}

/*
*	Search an array for a specific value, and returns the first index.
*
*	/param	value		Any value or struct that is in memory.
*	/param	array		Pointer to the start of the array.
*	/param	length		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The index of the value in the array, or -1 if it does not exist.
*/
int array_indexOf_memory(const uint8_t* value, const uint8_t* array, size_t length, uint8_t elem_size)
{
	uint8_t matching = 0;		//How many bytes have matched, also used to point at a byte in the value.
	
	int i = 0;
	while(i < length)
	{
		//The current byte is matching.
		if(array[i] == value[matching])
		{
			//All bytes are matching.
			if(++matching == elem_size)
			{
				if(i < elem_size)
					return 0;
				else
					return (i + 1) / elem_size;
			}
			
			//Increase index to check next byte.
			i++;			
		}
		else //First byte did not match.
		{
			//Reset
			matching = 0;
			
			//Go to the first byte of the next value.
			//Staying aligned with the value types size (elem_size)
			//This allows us to check less bytes, and stops the ability to get a false positive.
			i += elem_size - ((i != 0 ? i : elem_size) % elem_size);
		}
	}
	return -1;
}

/*
*	Searches an array for a value, and returns the last index.
*
*	/param	value		The value to find in the array. Can be any basic value type.
*	/param	array		Pointer to the start of the array.
*	/param	length		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The last index of the value in the array, or -1 if it does not exist.
*/
int array_lastIndexOf_memory(const uint8_t* value, const uint8_t* array, size_t length, uint8_t elem_size)
{
	int8_t matching = elem_size - 1;	//How many bytes have matched, also used to point at a byte in the value.
	
	int i = length - 1;
	while(i >= 0)
	{
		//The current byte is matching.
		if(array[i] == value[matching])
		{
			//All bytes are matching.
			if(--matching < 0)
			{
				if(i == 0) 
					return i;
				else
					return i / elem_size;
			}
			
			//Decrease index to check next byte.
			i--;
		}
		else //First byte did not match.
		{
			
			
			//Reset
			matching = elem_size - 1;
			
			
			//Go to the first byte of the next value.
			//Staying aligned with the value types size (elem_size)
			//This allows us to check less bytes, and stops the ability to get a false positive.
			i -= elem_size - ((i + 1) % elem_size);
		}
	}
	return -1;
}

/*
*	Searches an array for a value, and confirms its existence.
*
*	/param	value		The value to find in the array. Can be any basic value type.
*	/param	array		Pointer to the start of the array.
*	/param	length		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			If the array contains the value.
*/
bool array_contains_generic(const generic_union_t value, const char* array, size_t length, uint8_t elem_size)
{
	uint64_t v = value.u64;	//Store value so we can get pointer from.
	
	return array_contains_memory(&v, array, length, elem_size);
}

/*
*	Search an array for a specific value, and returns the first index.
*
*	/param	value		The value to find in the array. Can be any basic value type.
*	/param	array		Pointer to the start of the array.
*	/param	length		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The index of the value in the array, or -1 if it does not exist.
*/
int array_indexOf_generic(const generic_union_t value, const char* array, size_t length, uint8_t elem_size)
{
	uint64_t v = value.u64;		//Store value so we can get pointer from.
	
	return array_indexOf_memory(&v, array, length, elem_size);
}

/*
*	Searches an array for a value, and returns the last index.
*
*	/param	value		The value to find in the array. Can be any basic value type.
*	/param	array		Pointer to the start of the array.
*	/param	length		The size of the array (in bytes).
*	/param	elem_size	The size of the value's type (in bytes).
*
*	/returns			The index of the value in the array, or -1 if it does not exist.
*/
int array_lastIndexOf_generic(const generic_union_t value, const char* array, size_t length, uint8_t elem_size)
{
	uint64_t v = value.u64;		//Store value so we can get pointer from.
	
	return array_lastIndexOf_memory(&v, array, length, elem_size);
}

#pragma endregion Generic Methods

#pragma region Fast Type Specific Methods


bool array_contains_float(const float value, const float* array, size_t length)
{
	return array_indexOf_float(value, array, length) != -1;
	//for(int i = 0; i < length; i++)
	//{
		//if(array[i] == value)
		//return true;
	//}
	//return false;
}

bool array_contains_double(const double value, const double* array, size_t length)
{
	return array_indexOf_double(value, array, length) != -1;
	//for(int i = 0; i < length; i++)
	//{
		//if(array[i] == value)
		//return true;
	//}
	//return false;
}

int array_indexOf_float(const float value, const float* array, size_t length)
{
	for(int i = 0; i < length; i++)
	{
		if(array[i] == value)
		return i;
	}
	return -1;
}

int array_indexOf_double(const double value, const double* array, size_t length)
{
	for(int i = 0; i < length; i++)
	{
		if(array[i] == value)
		return i;
	}
	return -1;
}

int array_lastIndexOf_float(const float value, const float* array, size_t length)
{
	for(int i = length - 1; i >= 0; i--)
	{
		if(array[i] == value)
		return i;
	}
	return -1;
}

int array_lastIndexOf_double(const double value, const double* array, size_t length)
{
	for(int i = length - 1;  i >= 0; i--)
	{
		if(array[i] == value)
		return i;
	}
	return -1;
}

#pragma endregion Fast Type Specific Methods