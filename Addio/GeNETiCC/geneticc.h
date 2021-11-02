#ifndef GENETICC_H_
#define GENETICC_H_

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#include "config/geneticc_config.h"

#define ELEMENT_PTR		uint8_t*	//A pointer to a generic element

/*
*	Generic Pointer Cast
*
*	For casting non standard type pointers, so they are able to be passed to the generic macros.
*
*	*If you ever see the error "cast to union type from type not present in union," 
*	*you either passed a struct pointer or other value not included in "geneticc_union."
*	*If it was not a struct, you can add the value to the union(as long as it is under 64 bits),
*	*or simply cast to a value that is in the union(Which what this macro does).
*	*Unfortunately your IDE will not display the line which calls the macro, but the macro itself,
*	*which can be difficult to track down.
*
*	ex.
*
*	struct example* x;
*	struct example a[10];
*
*	Array_IndexOf(a, CAST_STRUCT x);
*	or
*	Array_IndexOf(a, (char*) x);
*/
#define CAST_STRUCT		(char*)

enum type{
	TYPE_I8,
	TYPE_U8,
	TYPE_I16,
	TYPE_U16,
	TYPE_I32,
	TYPE_U32,
	TYPE_I64,
	TYPE_U64,
	TYPE_FLOAT,
	TYPE_DOUBLE,
	TYPE_BOOL,
	TYPE_ENUM
};
typedef uint16_t type_t;

enum type_sized{
	TYPE_SIZED_I8 = -1,
	TYPE_SIZED_U8 = 1,
	TYPE_SIZED_I16 = -2,
	TYPE_SIZED_U16 = 2,
	TYPE_SIZED_I32 = -4,
	TYPE_SIZED_U32 = 4,
	TYPE_SIZED_I64 = -8,
	TYPE_SIZED_U64 = 8,
	TYPE_SIZED_FLOAT = 32,	//geneticc_get_sized_type_abs internally divides by 8 (anything over 16 is divided)
	TYPE_SIZED_DOUBLE = 64,	//geneticc_get_sized_type_abs internally divides by 8 (anything over 16 is divided)
	TYPE_SIZED_BOOL = 0
	//TYPE_SIZED_ENUM = 4
};
typedef int16_t type_sized_t;

/*
*	Used to describe a standard value's/type's size in bytes.
*	*For standard value types only.
*/
typedef uint8_t type_size_t;



/*
*	Used to describe the value/type size in bytes.
*
*	*If you see negative numbers, it indicates the type is signed.
*	*Positive value indicates the type is unsided.
*/
typedef int8_t signed_type_size_t;


/*
*	Used to describe the size of something in bytes.
*
*	Ex.	int[2]'s index_t = 8
*/
//typedef size_t byte_size_t;	Using size_t, array size represented as length_t

/*
*	Used to describe the size of an array.
*
*	Ex.	int[2]'s length_t = 2;
*/
typedef uint32_t length_t;

/*
*	Used to describe an offset(byte index) from an array, using bytes.
*
*	Ex.	int[2]'s byte_offset_t = 8
*/
typedef uint32_t byte_offset_t;

/*
*	Used to describe an index in an array.
*
*	Ex.	int[2]'s index_t = 2
*/
typedef uint32_t index_t;


/*
*	Union which contains all value types.
*	Useful for converting from int types to and from floating types, without casting the bits.
*	Also makes _Generic macros which call functions to all value types possible.
*
*	*Not all values are included at the moment.
*/
typedef union generic_union
{
	const char* ccp;
	char*cp;
	signed char* scp;
	unsigned char* uscp;
	int* ip;
	unsigned int* uip;
	short* sp;
	unsigned short* usp;
	long* lp;
	unsigned long* ulp;
	long long* llp;
	unsigned long long* ullp;
	_Bool* bp;	
	float* fp;
	double* dp;
	uint8_t* u8p;
	int8_t* s8p;
	uint16_t* u16p;
	int16_t* s16p;
	uint32_t* u32p;
	int32_t* s32p;
	uint64_t* u64p;
	int64_t* s64p;
	void* vp;
	
	char c;
	signed char sc;
	unsigned char usc;
	int i;
	unsigned int ui;
	short s;
	unsigned short us;
	long l;
	unsigned long ul;	
	long long ll;
	unsigned long long ull;
	float f;
	double d;
	_Bool b;
	enum e;
	uint8_t u8;
	int8_t s8;
	uint16_t u16;
	int16_t s16;
	uint32_t u32;
	int32_t s32;
	uint64_t u64;
	int64_t s64;
	
}generic_union_t;

typedef struct
{
	generic_union_t value;
	type_t type;
}geneticc_t;


//typedef struct  
//{
	//generic_union_t value;
	//bool hasValue;
//}nullable_t;


//static uint64_t /*__attribute__(__always_inline)*/ trim_extra_bytes(uint64_t value, size_t elem_size)
//{
	//switch(elem_size)
	//{
		//case 1:
		//return (uint8_t)value;	// & 0xff;
		//case 2:
		//return (uint16_t)value;	// & 0xffff;
		//case 4:
		//return (uint32_t)value;	// & 0xffffffff;
		//case 8:
		//return value;
		//default:				//bool
		//return value & 0x01;
	//}
//}

type_size_t geneticc_get_sized_type_abs(type_sized_t type);

signed_type_size_t geneticc_get_signed_sized_type_abs(type_sized_t type);

generic_union_t geneticc_cast_pointer(uint8_t* value, index_t index, type_sized_t type, type_sized_t cast_to);
generic_union_t geneticc_cast_b(generic_union_t value, type_sized_t type, type_sized_t cast_to);
generic_union_t geneticc_cast(generic_union_t value, type_sized_t type, type_sized_t cast_to, int8_t old_elem_size, int8_t new_elem_size);


#pragma region Macros

/*
*	Macros used in ARRAY_..._MACRO_CHOOSER macros, used to change parameter counts.
*/
#define GET_ARGUMENT_2(varg, arg1, arg2, ...) arg2

/*
*	Macros used in ARRAY_..._MACRO_CHOOSER macros, used to change parameter counts.
*/
#define GET_ARGUMENT_3(varg, arg1, arg2, arg3, ...) arg3

/*
*	Macros used in ARRAY_..._MACRO_CHOOSER macros, used to change parameter counts.
*/
#define GET_ARGUMENT_4(varg, arg1, arg2, arg3, arg4, ...) arg4

/*
*	Macros used in ARRAY_..._MACRO_CHOOSER macros, used to change parameter counts.
*/
#define GET_ARGUMENT_5(varg, arg1, arg2, arg3, arg4, arg5, ...) arg5

/*
*	Macros used in ARRAY_..._MACRO_CHOOSER macros, used to change parameter counts.
*/
#define GET_ARGUMENT_6(varg, arg1, arg2, arg3, arg4, arg5, arg6, ...) arg6

/*
*	Macros used in ARRAY_..._MACRO_CHOOSER macros, used to change parameter counts.
*/
#define GET_ARGUMENT_7(varg, arg1, arg2, arg3, arg4, arg5, arg6, arg7, ...) arg7

/*
*	/param	pointer		The pointer value to dereference
*	/param	type		The value used to get the type.
*
*	/returns			The index of the smallest value in the array.
*/
#define GENETICC_DEREFERENCE(pointer, type) _Generic((type),			\
const void*:				*(const void**)pointer,						\
void*:						*(void**)pointer,							\
const char*:				*(const char**)pointer,						\
char*:						*(char**)pointer,							\
const unsigned char*:		*(const unsigned char**)pointer,			\
unsigned char*:				*(unsigned char**)pointer,					\
const signed char*:			*(const signed char**)pointer,				\
signed char*:				*(signed char**)pointer,					\
const int*:					*(const int**)pointer,						\
int*:						*(int**)pointer,							\
const unsigned int*:		*(const unsigned int**)pointer,				\
unsigned int*:				*(unsigned int**)pointer,					\
const short*:				*(const short**)pointer,					\
short*:						*(short**)pointer,							\
const unsigned short*:		*(const unsigned short**)pointer,			\
unsigned short*:			*(unsigned short**)pointer,					\
const long*:				*(const long**)pointer,						\
long*:						*(long**)pointer,							\
const unsigned long*:		*(const unsigned long**)pointer,			\
unsigned long*:				*(unsigned long**)pointer,					\
const long long*:			*(const long long**)pointer,				\
long long*:					*(long long**)pointer,						\
const unsigned long long*:	*(const unsigned long long**)pointer,		\
unsigned long long*:		*(unsigned long long**)pointer,				\
_Bool*:						*(_Bool**)pointer,							\
double*:					*(double**)pointer,							\
float *:					*(float**)pointer,							\
const char:					*(const char*)pointer,						\
char:						*(char*)pointer,							\
const unsigned char:		*(const unsigned char*)pointer,				\
unsigned char:				*(unsigned char*)pointer,					\
const signed char:			*(const signed char*)pointer,				\
signed char:				*(signed char*)pointer,						\
const int:					*(const int*)pointer,						\
int:						*(int*)pointer,								\
const unsigned int:			*(const unsigned int*)pointer,				\
unsigned int:				*(unsigned int*)pointer,					\
const short:				*(const short*)pointer,						\
short:						*(short*)pointer,							\
const unsigned short:		*(const unsigned short*)pointer,			\
unsigned short:				*(unsigned short*)pointer,					\
const long:					*(const long*)pointer,						\
long:						*(long*)pointer,							\
const unsigned long:		*(const unsigned long*)pointer,				\
unsigned long:				*(unsigned long*)pointer,					\
const long long:			*(const long long*)pointer,					\
long long:					*(long long*)pointer,						\
const unsigned long long:	*(const unsigned long long*)pointer,		\
unsigned long long:			*(unsigned long long*)pointer,				\
_Bool:						*(_Bool*)pointer,							\
double:						*(double*)pointer,							\
float :						*(float*)pointer)



#pragma endregion Macros

#endif /* GENETICC_H_ */