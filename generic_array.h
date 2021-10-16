#ifndef GENERIC_ARRAY_H_
#define GENERIC_ARRAY_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <utils.h>

#include "generic.h"


#pragma region Generic Methods


bool array_contains_memory(const uint8_t* value, const uint8_t* array, size_t length, uint8_t elem_size );
bool array_contains_generic(const generic_union_t value, const char* array, size_t length, uint8_t elem_size);

int array_indexOf_memory(const uint8_t* value, const uint8_t* array, size_t length, uint8_t elem_size);
int array_indexOf_generic(const generic_union_t value, const char* array, size_t length, uint8_t elem_size);

int array_lastIndexOf_memory(const uint8_t* value, const uint8_t* array, size_t length, uint8_t elem_size);
int array_lastIndexOf_generic(const generic_union_t value, const char* array, size_t length, uint8_t elem_size);

//Not implemented yet
//int array_max_memory(const uint8_t* value, const uint8_t* array, size_t length, uint8_t elem_size);
//int array_max_generic(const generic_union_t value, const char* array, size_t length, uint8_t elem_size);
//
//int array_min_memory(const uint8_t* value, const uint8_t* array, size_t length, uint8_t elem_size);
//int array_min_generic(const generic_union_t value, const char* array, size_t length, uint8_t elem_size);
//
//int array_average_memory(const uint8_t* value, const uint8_t* array, size_t length, uint8_t elem_size);
//int array_average_generic(const generic_union_t value, const char* array, size_t length, uint8_t elem_size);


#pragma endregion Generic Methods

#pragma region Fast Type Specific Methods




/*
*	Enum for differentiating types to calls the proper function in the "internal_call_array_...._special" functions.
*/
enum array_specialized_types{
	//ARR_SPEC_TYPE_INT,
	//.
	//.
	//.
	//ARR_SPEC_TYPE_SHORT,
	ARR_SPEC_TYPE_FLOAT = 4,
	ARR_SPEC_TYPE_DOUBLE = 8
	
};
typedef uint8_t array_specialized_types_t;

bool array_contains_float(const float value, const float* array, size_t length);
bool array_contains_double(const double value, const double* array, size_t length);

int array_indexOf_float(const float value, const float* array, size_t length);
int array_indexOf_double(const double value, const double* array, size_t length);

int array_lastIndexOf_float(const float value, const float* array, size_t length);
int array_lastIndexOf_double(const double value, const double* array, size_t length);

/*
*	To include these type specific functions in the macros, we need an intermediate function as pointers complain when they try to be cast to a floating point value.
*	Even though in macros, the pointers will never call the floating point functions, anything in _Generic has to be compatible with the value that is calling.
*	That is what the internal_call_array_..._special methods are for.
*/

/*
*	Internal function used to call type specific array_contains functions.
*/
static int internal_call_array_contains_special(const generic_union_t value, const char* array, size_t length, array_specialized_types_t elem_type)
{
	switch(elem_type)
	{
		case ARR_SPEC_TYPE_FLOAT:
			return array_contains_float(value.f, array, length);
		case ARR_SPEC_TYPE_DOUBLE:
			return array_contains_double(value.d, array, length);
	}
}

/*
*	Internal function used to call type specific array_indexOf functions.
*/
static int internal_call_array_indexOf_special(const generic_union_t value, const char* array, size_t length, array_specialized_types_t elem_type)
{
	switch(elem_type)
	{
		case ARR_SPEC_TYPE_FLOAT:
			return array_indexOf_float(value.f, array, length);
		case ARR_SPEC_TYPE_DOUBLE:
			return array_indexOf_double(value.d, array, length);
	}
}

/*
*	Internal function used to call type specific array_lastIndexOf functions.
*/
static int internal_call_array_lastIndexOf_special(const generic_union_t value, const char* array, size_t length, array_specialized_types_t elem_type)
{
	switch(elem_type)
	{
		case ARR_SPEC_TYPE_FLOAT:
			return array_lastIndexOf_float(value.f, array, length);
		case ARR_SPEC_TYPE_DOUBLE:
			return array_lastIndexOf_double(value.d, array, length);
	}
}


#pragma endregion Fast Type Specific Methods

#pragma region Macros


/*
*	Macros used in ARRAY_..._MACRO_CHOOSER macros, used to change parameter counts.
*/
#define GET_LENGTH_ARG(varg, value, array, length, ...) length


/*
*	Macro used to "overload" array_indexOf functions.
*	Can be used with arrays or pointers.
*
*	/param	value		The value to search for.
*	/param	array		Pointer to the start of the array.
*	/param	length		The size of the array (in bytes).
*
*	/returns			If the array contains the value.
*/
#define Array_Contains(value, array, length) _Generic((value),																	\
const char*: array_contains_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),				\
char*: array_contains_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),					\
const unsigned char*: array_contains_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length,sizeof(array[0])),		\
unsigned char*: array_contains_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),			\
const signed char*: array_contains_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),		\
signed char*: array_contains_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),				\
float :  internal_call_array_contains_special((generic_union_t)value, array, length, ARR_SPEC_TYPE_FLOAT),						\
double :  internal_call_array_contains_special((generic_union_t)value, array, length,  ARR_SPEC_TYPE_DOUBLE),					\
default: array_contains_generic((generic_union_t)value, array, length, sizeof(array[0])))

/*
*	Adds the ability to call one macro with different amounts of parameters
*/
#define PARRAY_CONTAINS_GENERIC(value, array, length) Array_Contains(value, array, length)
#define ARRAY_CONTAINS_GENERIC(value, array) Array_Contains(value, array, sizeof(array))
#define ARRAY_CONTAINS_MACRO_CHOOSER(...)				\
GET_LENGTH_ARG(__VA_ARGS__, PARRAY_CONTAINS_GENERIC,	\
ARRAY_CONTAINS_GENERIC, )

/*
*	Calls the "Array_Contains" with a different amount of parameters.
*
*	/param	value		The value to search for.
*	/param	array		Pointer to the start of the array.
*	/param	*length		(Optional)The size of the array (in bytes). *Must be called with pointer arrays!
*
*	/returns			If the array contains the value.
*/
#define array_contains(...) ARRAY_CONTAINS_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


/*
*	Macro used to "overload" array_indexOf functions.
*
*	/param	value		The value to search for.
*	/param	array		Pointer to the start of the array.
*	/param	length		The size of the array (in bytes).
*
*	/returns			The index of the value in the array, or -1 if it does not exist.
*/
#define Array_IndexOf(value, array, length) _Generic((value),																	\
const char*: array_indexOf_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),				\
char*: array_indexOf_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),						\
const unsigned char*: array_indexOf_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),		\
unsigned char*: array_indexOf_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),			\
const signed char*: array_indexOf_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),		\
signed char*: array_indexOf_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),				\
float :  internal_call_array_indexOf_special((generic_union_t)value, array, length, ARR_SPEC_TYPE_FLOAT),						\
double :  internal_call_array_indexOf_special((generic_union_t)value, array, length, ARR_SPEC_TYPE_DOUBLE),						\
default: array_indexOf_generic((generic_union_t)value, array, length, sizeof(array[0])))

/*
*	Adds the ability to call one macro with different amounts of parameters
*/
#define PARRAY_INDEXOF_GENERIC(value, array, length) Array_IndexOf(value, array, length)
#define ARRAY_INDEXOF_GENERIC(value, array) Array_IndexOf(value, array, sizeof(array))
#define ARRAY_INDEXOF_MACRO_CHOOSER(...)			\
GET_LENGTH_ARG(__VA_ARGS__, PARRAY_INDEXOF_GENERIC, \
ARRAY_INDEXOF_GENERIC, )

/*
*	Calls the "Array_IndexOf" with a different amount of parameters.
*
*	/param	value		The value to search for.
*	/param	array		Pointer to the start of the array.
*	/param	*length		(Optional)The size of the array (in bytes). *Must be called with pointer arrays!
*
*	/returns			If the array contains the value.
*/
#define array_indexOf(...) ARRAY_INDEXOF_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/*
*	Macro used to "overload" array_indexOf functions.
*
*	/param	value		The value to search for.
*	/param	array		Pointer to the start of the array.
*	/param	length		The size of the array (in bytes).
*
*	/returns			The last index of the value in the array, or -1 if it does not exist.
*/
#define Array_LastIndexOf(value, array, length) _Generic((value),																	\
const char*: array_lastIndexOf_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),				\
char*: array_lastIndexOf_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),						\
const unsigned char*: array_lastIndexOf_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),		\
unsigned char*: array_lastIndexOf_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),			\
const signed char*: array_lastIndexOf_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),		\
signed char*: array_lastIndexOf_memory((generic_union_t){.u64 = (uint32_t)value}.cp, array, length, sizeof(array[0])),				\
float :  internal_call_array_indexOf_special((generic_union_t)value, array, length, ARR_SPEC_TYPE_FLOAT),							\
double :  internal_call_array_indexOf_special((generic_union_t)value, array, length, ARR_SPEC_TYPE_DOUBLE),							\
default: array_lastIndexOf_generic((generic_union_t)value, array, length, sizeof(array[0])))

/*
*	Adds the ability to call one macro with different amounts of parameters
*/
#define PARRAY_LASTINDEXOF_GENERIC(value, array, length) Array_LastIndexOf(value, array, length)
#define ARRAY_LASTINDEXOF_GENERIC(value, array) Array_LastIndexOf(value, array, sizeof(array))
#define ARRAY_LASTINDEXOF_MACRO_CHOOSER(...)			\
GET_LENGTH_ARG(__VA_ARGS__, PARRAY_LASTINDEXOF_GENERIC, \
ARRAY_LASTINDEXOF_GENERIC, )

/*
*	Calls the "Array_LastIndexOf" with a different amount of parameters.
*
*	/param	value		The value to search for.
*	/param	array		Pointer to the start of the array.
*	/param	*length		(Optional)The size of the array (in bytes). *Must be called with pointer arrays!
*
*	/returns			If the array contains the value.
*/
#define array_lastIndexOf(...) ARRAY_LASTINDEXOF_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#pragma endregion Macros

#endif /* GENERIC_ARRAY_H_ */