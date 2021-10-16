#include "generic.h"
#include "generic_array.h"

//The struct def for the example
typedef struct example_struct{
	int a;
	float b;
}example_struct_t;

/* The example arrays */

//Struct array
example_struct_t ts_arr[4];

//Standard Value Types
uint16_t shortarray [10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};	
float  floatarray[3] = {1.0f, 2.5f, -5.25f};	

//Pointer array
int* intPointerArray;
	

int main(void)
{
	/* Integer Types */	
	
	//Macros
	//Array_IndexOf does not require the length, but can also be used.
	//array_indexOf always requires the length.
	
	int shortResult1 =  Array_IndexOf(2, shortarray);						//length is not passed
	int shortResult2 =  Array_IndexOf(2, shortarray, 5);					//length is passed
	int shortResult3 =  array_indexOf(2, shortarray, sizeof(shortarray));	//length must be passed
	
	//Here is the function which the macros call, it always requires the length, as well as the size of the element.
	//It also requires you to cast the value to (generic_union_t).
	int shortResult4 = array_indexOf_generic((generic_union_t)2, shortarray, sizeof(shortarray), sizeof(uint16_t));
	

	/* Floating Point Types*/
	bool containsFloat = Array_Contains(2.5f, floatarray);
	
	/* Struct */
	ts_arr[0] = (example_struct_t){.a = 5, .b = 2.5f};
	ts_arr[1] = (example_struct_t){.a = 5, .b = -2.5f};
	ts_arr[2] = (example_struct_t){.a = 3, .b = 2.5f};
	ts_arr[3] = (example_struct_t){.a = 4, .b = 1.4f};	
	
	example_struct_t ts = (example_struct_t){.a = 3, .b = 2.5f};
		
	//Unfortuantely we need to cast the struct to a char* (All pointers must be char*)
	bool containsStruct = Array_Contains((char*)&ts, ts_arr);
	
	/* Pointer Based Arrays */
	
	//Initialize our "pointer array"
	intPointerArray = malloc(6 * sizeof(int));	
	intPointerArray[0] = 0;
	intPointerArray[1] = 1;
	intPointerArray[2] = 2;
	intPointerArray[3] = 0;
	intPointerArray[4] = 1;
	intPointerArray[5] = 2;
	
	//You probably already know, but pointer arrays can not be used with sizeof. The length needs to be passed manually.
	int pointerArrayResult = Array_LastIndexOf(1, intPointerArray, (6 * sizeof(int)));
}
