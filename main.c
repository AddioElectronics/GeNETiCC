#include <atmel_start.h>

#include "addio/using/using_Addio.GeNETiCC.h"

/*
*	If "addio/using" is added to your project includes.
*	#include "using_Addio.GeNETiCC.h"	
*/

//The struct def for the example
typedef struct example_struct{
	int a;
	float b;
}example_struct_t;

//The arrays to search
example_struct_t ts_arr[4];
uint16_t shortarray [4] = {1, 2, 3, 4};
uint16_t* pointerArray;
uint32_t uintarray[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int intarray[10] = {-1, 2, -3, 4, 5, 6, -7, 8, -9, 10};
//int intarray[6] = {-1, 0x0101, 0x5432, 0x6789, 0xFF11, 0xef01};
float  floatarray[3] = {1.0f, 2.5f, -5.25f};
double dubarray[10] = {1.0d, 2.0d, -5.25d, 4.0d, 5.0d, 6.0d, 7.0d, 8.0d, 9.0d, 10.0d};

char charray[4] = {'0', '1', '2', '3'};

int dmaArray[32];


void initialize_example_values()
{
	//Initialize pointer array
	pointerArray = malloc(16);
	pointerArray[0] = 0;
	pointerArray[1] = 1;
	pointerArray[2] = 2;
	pointerArray[3] = 3;
	pointerArray[4] = 0;
	pointerArray[5] = 1;
	pointerArray[6] = 2;
	pointerArray[7] = 3;
	
	//Initialize struct
	ts_arr[0] = (example_struct_t){.a = 5, .b = 5};
	ts_arr[1] = (example_struct_t){.a = 5, .b = -2.5f};
	ts_arr[2] = (example_struct_t){.a = 3, .b = 2.5f};
	ts_arr[3] = (example_struct_t){.a = 4, .b = 1.4f};

	
	for(int i = 0; i < 32; i++)
	{
		dmaArray[i] = 0x11223344 * (i + 1);
	}
	
}



void array_contains_example()
{
	/* Int */
	bool containsInt;
	containsInt = Array_Contains(uintarray, 5);										//Returns true		length not needed as "intarray" is a static array.
	containsInt = Array_Contains(uintarray, 5, sizeof(intarray) / sizeof(int));		//Returns true		length is optional
	containsInt = Array_Contains(uintarray, 5, 4);									//Returns false		only checks the first 4 items.
	containsInt = Array_Contains(uintarray, 5, 4, 4);								//Returns true		checks from index 4, to index 8
	

	/* Pointer array (short) */
	bool containsPointShort;
	containsPointShort = Array_Contains(pointerArray, 3, 8);						//Returns True
	containsPointShort = Array_Contains(pointerArray, 3);							//Returns False, length must be passed for non-static arrays!
	

	/* Floating Points */
	bool containsDouble;
	containsDouble = Array_Contains(dubarray, -5.25d);								//Returns true
	
	//Floats and doubles by default call specific functions for floating point values. This increases performance, but will produce a larger program.
	//If you want them to use the generic method like all the other value types, go to the "Array_Contains" macro, and remove the float: and double: lines, which will end up using "default:"
	//Doing so will trade performance for a smaller code size.
	//If you do not care about code size, specialty functions can be added for any value type, by doing the reverse(also need to create the specialty function and add it in the "internal_call_array_contains_special's" switch statement.).
	
	/* Struct */
	
	//The struct to search for.
	example_struct_t ts = (example_struct_t){.a = 3, .b = 2.5f};
	
	//Unfortuantely we need to cast the struct to a char*
	//(Any pointers must be cast to char*, until they are added to the macro's _Generic)
	bool containsStruct = Array_Contains(ts_arr, CAST_STRUCT &ts);		//Returns true

}


void array_indexOf_example()
{
	/* Int */
	int indexInt;
	indexInt = Array_IndexOf(uintarray, 1);										//Returns 0		length not needed as "intarray" is a static array.
	indexInt = Array_IndexOf(uintarray, 6, sizeof(intarray) / sizeof(int));		//Returns 5		length is optional
	indexInt = Array_IndexOf(uintarray, 7, 4);									//Returns -1	only checks the first 4 items.
	indexInt = Array_IndexOf(uintarray, 8, 4, 4);								//Returns 7		checks from index 4, to index 8
	
	/* Pointer array (short) */
	//length required as it is not a static array
	int indexPointer;
	indexPointer = Array_IndexOf(pointerArray, 2, 8);							//Returns 2
	indexPointer = Array_LastIndexOf(pointerArray, 2, 8);						//Returns 6
	
	/* Floating Points */
	int indexDouble;
	indexDouble = Array_IndexOf(dubarray, -5.25d);								//Returns 2
	

	/* Struct */
	
	//The struct to search for.
	example_struct_t ts = (example_struct_t){.a = 5, .b = -2.5f};
	
	//Unfortuantely we need to cast the struct to a char*
	//(Any pointers must be cast to char*, until they are added to the macro's _Generic)
	int indexStruct = Array_IndexOf(ts_arr, (char*)&ts);						//Returns 1
	
}


void array_insert_example()
{
	char range[5] = {'r', 'a', 'n', 'g', 'e'};
	
	char* insertCharArray	= Array_Insert(charray, 'a', 2);							//Result = {'0', '1', 'a', '2', '3'}
	insertCharArray			= Array_Insert(insertCharArray, 'b', 3, 5, true);			//Result = {'0', '1', 'a', 'b', '2', '3'}
	
	
	char* insertRangeCharArray	= Array_InsertRange(charray, range, 2);									//Result = {'0', '1', 'r', 'a', 'n', 'g', 'e', '2', '3'}				Do not need to pass count or length because range and charray are static arrays.
	insertRangeCharArray		= Array_InsertRange(insertRangeCharArray, range, 2, 2, 9, true);		//Result = {'0', '1', 'r', 'a', 'r', 'a', 'n', 'g', 'e', '2', '3'}		Need to pass length because insertRangeCharArray is not a static array. Only passing the first 2 chars of range.
	
	
	char* addCharArray		= Array_Add(charray,'a');								//Result = {'0', '1', '2', '3', 'a'}
	char* addRangeCharArray = Array_AddRange(charray, range);						//Result = {'0', '1', '2', '3',  'r', 'a', 'n', 'g', 'e'}
	
	
	
	char* prependCharArray		= Array_Prepend(charray, 'a');						//Result = {'a', '0', '1', 'a', '2', '3'}
	char* prependRangeCharArray = Array_PrependRange(charray, range);				//Result = { 'r', 'a', 'n', 'g', 'e', '0', '1', '2', '3'}
	
	
	//Remember to free when no longer used
	free(insertCharArray);
	free(insertRangeCharArray);	//Remember to free when no longer used
	free(addCharArray);	//Remember to free when no longer used
	free(addRangeCharArray);
	free(prependCharArray);	//Remember to free when no longer used
	free(prependRangeCharArray);
}


void array_minmaxaverage_example()
{
	//Gets the index
	int maxInt = Array_Max(uintarray);				//Result = 9
	int minInt = Array_Min(uintarray);				//Result = 0
	
	maxInt = Array_Max(intarray);				//Result = 9
	minInt = Array_Min(intarray);				//Result = 0
	
	
	int maxDouble = Array_Max(dubarray);			//Result = 9
	int minDouble = Array_Min(dubarray);			//Result = 2
	
	int maxFloat = Array_Max(floatarray);
	int minFloat = Array_Min(floatarray);
	
	
	//Gets the value
	int maxIntVal = Array_Max_Val(uintarray);		//Result = 10
	int minIntVal = Array_Min_Val(uintarray);		//Result = 1
	
	
	int sumInt;
	sumInt = Array_Sum(uintarray);					//Result = 55
	sumInt = Array_Sum(uintarray, 5, 5);				//Result = 40
	
	
	int averageInt;
	averageInt = Array_Average(uintarray);				//Result = 5
	averageInt = Array_Average(uintarray, 5, 5);		//Result = 8
	
	
	float array[8] = {1, 2, 3, 4, 1, 2, 3, 4};
	float average;
	average= Array_Average(array);				//Result = 2.5f
	average= Array_Average(array, 3, 5);		//Result = 8
	
	
}

bool predicate_example(const ELEMENT_PTR value)
{
	return *((int*)value) > 5;
}

bool predicate_example2( const ELEMENT_PTR value, int arg_count, va_list ap)
{
	return *((int*)value) == va_arg(ap, int) + va_arg(ap, int);
}


void find_example()
{
	int index = Array_FindIndex(uintarray, predicate_example);								//Result = 5
	uint32_t* pointer = Array_Find(uintarray, predicate_example);							//Result = pointer to uintarray[5]
	
	size_t matchCount = 0;																	//Result = 5
	uint8_t** matches = Array_FindAll(uintarray, predicate_example, &matchCount);			//Result = array holding 5 pointers from uintarray[5] to uintarray[9]
	
	index = Array_FindIndexArgs(uintarray, predicate_example2, 10, 2, 3, 3);				//Result = 5
	
	for(int i = 0; i < matchCount; i++)
	{
		//Will equal 6 to 10 depending on i
		int value = *((int*)(matches[i]));
	}
	
	//Remember to free matches when you are done.
	free(matches);
	
	int count = Array_ValueCount(pointerArray,2,  8);		//Returns 2
	
}


void vargs_example(int arg_count, ...)
{
	va_list ap;
	va_start(ap, arg_count);
	
	int index = Array_FindIndexVargs(uintarray, predicate_example2, arg_count, ap);	//Result = 5
}


void remove_example()
{
	uint8_t* removeRangeArray = Array_RemoveRange(uintarray, 3, 3 );						//Result, {1, 2, 3, 7, 8, 9, 10}
	uint8_t* removeAtArray = Array_RemoveAt(uintarray, 3);									//Result, {1, 2, 3, 5, 6, 7, 8, 9, 10}
	
	bool removed;
	int* removeArray = Array_Remove(uintarray, 5, &removed);								//Result, {1, 2, 3, 4, 6, 7, 8, 9, 10}		removed = true
	removeArray = Array_Remove(removeArray, 11, 9, true, NULL );							//Result, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}	removed = false*  (removed not passed)
	
	uint32_t removeCount = 0;
	uint8_t* removeAllArray = Array_RemoveAll(pointerArray, 2, 8, false, &removeCount );	//Result, {0, 1, 3, 0, 1, 3}				removeCount = 2
	size_t removeCountArrayNewSize = 8 - removeCount;
	
	uint16_t* reverseArray = Array_Reverse(pointerArray, 8);								//Result, {'3, '2', '1', '0'};
	
	//Remember to free
	free(removeRangeArray);
	free(removeAtArray);
	free(removeArray);
	free(removeAllArray);
	free(reverseArray);
}

void list_example()
{

	char range[5] = {'r', 'a', 'n', 'g', 'e'};

	/*Initialize list - (List not created with malloc, internal array is)*/
	
	list_t listRange;					//When using list_t instead of LIST_PTR, when calling some macros you must cast to LIST_PTR, or you will get "invalid type argument of '->'..." Ex. (LIST_PTR)(&listRange)
	List_Init(&listRange, 1);
	List_AddRange(&listRange, range);
	
	/*Allocate list - (Malloc used to allocate list and array)*/
	
	LIST_PTR listRanger = new_List(1);	//Recommended instead of List_Init
	List_AddRange(listRanger, range);
	
	LIST_PTR newList = new_List(1);		//Default capacity of 4 (length is 0)
	
	/*Setting/Adding Values*/
	
	//List length is currently zero, doing this will change the memory in the internal array. But the list will not recognize them as existing.
	//If the list already had values there, it would recognize them.
	//I'm sure most of you know how a list works, but I am explaining it for those that don't(The best I can).
	newList->array[0] = 'l';	/*	list->array[0] != List_Array(list)[0] if a value was removed from the start of the array	*/
	//newList->array[1] = 'i';
	//newList->array[2] = 's';
	//newList->array[3] = 't';	//Legal
	//newList->array[4] = 's';	//Not legal (out of range)

	length_t newListLength = List_Count(newList);	//Returns 0
	newListLength = List_AdjustHead(newList, 4);	//Returns 4, This will increase the size of the internal array (as long as it does not go past its internal capacity).

	//These values would not be set if we didn't adjust the head.
	//While the internal array may be large enough to fit them,
	//the list thinks it has a length of zero.
	List_Array(newList)[1] = 'i';	//Gets internal array aligned to the tail.
	List_Val(newList, 's', 2);		//Sets value at index (aligned internally). *List_Val can also be used to get a value, example below
	List_Set(newList, 't', 3);		//Sets value at index (aligned internally).
	
	//Instead of the above, we could have just used List_Add, which will adjust the head automatically(As well as increase the capacity if need be).
	List_Add(newList, 's');			//Adds a value to the end of the list
	
	/*Result = {'l', 'i', 's', 't', 's'}*/
	
	char first = List_Array(newList)[0];
	char second = newList->array[newList->tail + 1];
	char* third = List_Get(newList, 2);							//Returns pointer
	char fourth  = *(char*)List_Val(newList, 3);				//Returns pointer, dereferenced to value.		*List_Val can also be used to set a value, example above
	fourth = List_Get_Std(newList, 3, fourth);					//Returns value, no dereferencing needed.		*Second argument is used to get the value's type.
	//fourth = List_Get_ValStd(newList, (char)0, 3);			//Also valid.
	
	char test = List_Get_Std(newList, 3, test);
	

	
	
	int capacity_length = List_Capacity(newList);			//Returns 14	- Gets the capacity (length).
	//List_Get_Capacity(list);
	capacity_length = List_Capacity(newList, 5);			//Returns true, new length is 5
	//List_Set_Capacity(list, 5);
	size_t capacitySize = List_Get_CapacitySize(newList);	//Returns 5		- Gets the capacity (bytes)
	
	/*Converting an array to a list*/
	//list_t charlist1 = Array_To_List(charray);	//Error, will return invalid initializer
	//list_t charlist1 = *Array_To_List(charray);	//Memory leak!
	LIST_PTR charlist1 = Array_To_List(charray);	//OK
	
	/*Inserting, Adding, Prepending, Removing*/
	bool success = List_InsertList(charlist1, (LIST_PTR)(&listRange), 2);	// Result = {'0', '1', 'r', 'a', 'n', 'g', 'e', '2', '3'} success almost never required unless ARRAY_MAX_SIZE is very low, or the array is very large.
	List_Add(charlist1, '5', 1);											// Result = {'0', '1', 'r', 'a', 'n', 'g', 'e', '2', '3', '5'}
	List_AddList(charlist1, (LIST_PTR)(&listRange));						// Result = {'0', '1', 'r', 'a', 'n', 'g', 'e', '2', '3', '5', 'r', 'a', 'n', 'g', 'e'}
	List_Prepend(charlist1,'0', 1);											// Result = {'0', '0', '1', 'r', 'a', 'n', 'g', 'e', '2', '3', '5', 'r', 'a', 'n', 'g', 'e'}
	List_PrependList(charlist1, (LIST_PTR)(&listRange));					// Result = {'r', 'a', 'n', 'g', 'e', '0','0', '1', 'r', 'a', 'n', 'g', 'e', '2', '3', '5', 'r', 'a', 'n', 'g', 'e'}
	List_RemoveAt(charlist1, 3);											// Result = {'r', 'a', 'n', 'e', '0', '0', '1', 'r', 'a', 'n', 'g', 'e', '2', '3', '5', 'r', 'a', 'n', 'g', 'e'}
	List_Remove(charlist1, '3');											// Result = {'r', 'a', 'n', 'e',  '0', '0', '1', 'r', 'a', 'n', 'g', 'e', '2', '5', 'r', 'a', 'n', 'g', 'e'}
	List_RemoveAll(charlist1, 'a');											// Result = {'r', 'n', 'e', '0', '0', '1', 'r', 'n', 'g', 'e', '2', '5', 'r', 'n', 'g', 'e'}

	List_Reverse(charlist1);
	//float* farr = Array_ConvertTo(intarray, TYPE_SIZED_I32, TYPE_SIZED_FLOAT);
	
	//Free list and internal array with List_Delete.
	List_Delete((LIST_PTR)(&listRange));
	List_Delete(listRanger);
	List_Delete(newList);
	List_Delete(charlist1);
}

bool predicateex( const ELEMENT_PTR value)
{
	return *((int*)value) > 5;
}


void list_find_example()
{
	
	int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	LIST_PTR list = Array_To_List(array);

	LIST_PTR matches = List_FindAll(list, predicateex); //Result = A list containing pointers to list->array[5 - 9]

	int** aligned = matches->array;

	for(int i = 0; i < List_Count(matches); i++)
	{
		*aligned[i] = i;
	}
	
}

/*
*	For these DMA examples to actually use DMA, you must configure the DMA to accept small transfers.
*
*	1. In geneticc_dma_config.h, set GENETICC_DMA_MIN_TRANSACTION_COUNT to 8
*	2. In geneticc_dma.h, under the current system, set GENETICC_DMA_MAX_BEATSIZE to BEATSIZE_BYTE (if you are using atmel start, it would be under #if GENETICC_DMA_SYSTEM == GENETICC_DMA_SYSTEM_ATMELSTART_ARM)
*
*	This will allow 8 bytes to be sent via DMA.
*/
void list_dma_example()
{
	int intchararray[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	int intrange[5] = {'r', 'a', 'n', 'g', 'e'};

	LIST_PTR intRanger = new_List(4);
	List_DMA_AddRange(intRanger, intrange);
	LIST_PTR intList = Array_To_List(intchararray, 21, 10);	//Setting capacity to 21 so it will be able to fit all the below functions.
	
	
	/*List DMA Functions*/

	List_DMA_InsertList(intList, intRanger, 2);		// Result = {'0', '1', 'r', 'a', 'n', 'g', 'e', '2', '3', '4', '5', '6', '7', '8', '9'}
	List_DMA_Add(intList, '5');						// Result = {'0', '1', 'r', 'a', 'n', 'g', 'e', '2', '3', '4', '5', '6', '7', '8', '9', '5'}
	List_DMA_AddList(intList, intRanger);			// Result = {'0', '1', 'r', 'a', 'n', 'g', 'e', '2', '3', '4', '5', '6', '7', '8', '9', '5', 'r', 'a', 'n', 'g', 'e'}
	List_DMA_Prepend(intList,'0');					// Result = {'0', '0', '1', 'r', 'a', 'n', 'g', 'e', '2', '3', '4', '5', '6', '7', '8', '9', '5', 'r', 'a', 'n', 'g', 'e'}
	List_DMA_PrependList(intList, intRanger);		// Result = {'r', 'a', 'n', 'g', 'e', '0','0', '1', 'r', 'a', 'n', 'g', 'e', '2', '3', '4', '5', '6', '7', '8', '9', '5', 'r', 'a', 'n', 'g', 'e'}
	List_DMA_RemoveAt(intList, 3);					// Result = {'r', 'a', 'n', 'e', '0', '0', '1', 'r', 'a', 'n', 'g', 'e', '2', '3', '4', '5', '6', '7', '8', '9', '5', 'r', 'a', 'n', 'g', 'e'}
	List_DMA_Remove(intList, '3');					// Result = {'r', 'a', 'n', 'e',  '0', '0', '1', 'r', 'a', 'n', 'g', 'e', '2', '4', '5', '6', '7', '8', '9', '5', 'r', 'a', 'n', 'g', 'e'}

	
	List_DMA_RemoveAll(intList, 'a');								// Result = {'r', 'n', 'e', '0', '0', '1', 'r', 'n', 'g', 'e', '2', '5', 'r', 'n', 'g', 'e'}

	//Wait for DMA transfer to finish.
	//Used the destination to find the transfer
	geneticc_dma_wait_for_transfer(intList);

	List_Delete(intList);
	List_Delete(intRanger);
}



void list_struct_example()
{
	
	example_struct_t* es_array[5];
	
	
	
	for(int i = 0; i < 5; i++)
	{
		example_struct_t es;
		
		es.a = i;
		es.b = i + 0.5f;
	}
	
	LIST_PTR list = Array_To_List(es_array);
	
	example_struct_t es1 = {.a = -1, .b = -1.5f};
	
	size_t esa_size = sizeof(es_array[0]);
	size_t es1_size = sizeof(es1);
	size_t es_size = sizeof(example_struct_t);
	
	
	
	List_Insert(list, CAST_STRUCT &es1, 2);
	
}

void geneticc_dma_transfer_done(const geneticc_dma_transfer_t* transfer)
{	
	#if GENETICC_DMA_AUTOFREE_TRANSFER == false
	//This must be called if you have registered the callback or you will have a memory leak.
	//You could store this pointer and free it at a later time if you wish, but it is not recommended.
	//If the callback is not registered, it will be freed internally.
	if(transfer->flags.no_delete)
	geneticc_dma_delete_transfer(transfer);
	#endif
}


int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	geneticc_dma_init(BEATSIZE_BYTE);
	geneticc_dma_register_user_callbacks(DMA_CB_TRANSFER_DONE, geneticc_dma_transfer_done);
	
	initialize_example_values();
	array_contains_example();
	array_indexOf_example();
	array_minmaxaverage_example();
	array_insert_example();
	remove_example();
	find_example();
	list_example();
	list_dma_example();
	list_struct_example();

	while (1) {

	}
}