#ifndef GENERIC_H_
#define GENERIC_H_

#include <stdint.h>
#include <stdbool.h>

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
	 bool b;
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


#endif /* GENERIC_H_ */