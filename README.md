# Generic Array ( C Library)

A C libary which implements generic functions for working with arrays.

## Usage

Just import the headers and source files into your project, and include the headers.

```
#include "generic.h"
#include "generic_array.h"
```

### Macros (Main Usage)
` length is size in bytes, not the  element count. `

` Macros below contain siblings where the first characters are capitalized, there are different ways to call them. `

[Learn about macro differences, and how macros can be called](#Calling-Conventions)

##### array_contains(value, array, length)
 Searches an array up to the length for "value", and returns a bool depending on if it exists in the context.
 
##### array_indexOf(value, array, length)
Searches an array up to the length for "value", and returns the first index of the value.
If "value" does not exist, -1 will be returned.

##### array_lastIndexOf(value, array, length)
Searches an array up to the length for "value", and returns the last index of the value.
If "value" does not exist, -1 will be returned.

#### Examples
` More examples can be found in main.c`
>The main way to use this library is to use the lower-case macros.
>The example below only contains lower-case-macros.
[Learn about lower/upper-case macros](#Calling-Conventions)
``` C
#include "generic_array.h"

//Initialize Arrays
uint16_t shortarray [10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};	
float  floatarray[3] = {1.0f, 2.5f, -5.25f};	

void main()
{
    //Length not passed, full array checked
    bool containsShort = array_contains(5, shortarray);     
    Or
    //Length passed, half of array checked
    int value = 5;
    bool containsShort = array_contains(value, shortarray, (5 * sizeof(uint16_t));  
}
```

### Functions
`Functions are mainly for internal use, to be called from the macros. They are still available for use, if you choose to use them.`


###### array_..._memory(const uint8_t* value, const uint8_t* array, size_t length, uint8_t elem_size)
All the the main functionality is located in these functions.

###### array_..._generic(const generic_union_t value, const uint8_t* array, size_t length, uint8_t elem_size)
Used for standard value types. 



### Type Specific Functions
> Type specific functions can be added to improve performance. Float and Double use them by default. 
> They can be called directly, or via the macros.

 ` To have new functions used by macros, the macros will have to be edited. `


###### bool array_contains_float(const float value, const float* array, size_t length)
###### bool array_contains_double(const double value, const double* array, size_t length)
###### int array_indexOf_float(const float value, const float* array, size_t length)
###### int array_indexOf_double(const double value, const double* array, size_t length)
###### int array_lastIndexOf_float(const float value, const float* array, size_t length)
###### int array_lastIndexOf_double(const double value, const double* array, size_t length)

### Calling Conventions



Each macro contains a "sibling," where the first characters are/aren't capitalized.

##### Lower-Case Macros (array_contains, array_indexOf, array_lastIndexOf)
`length is the optional parameter (For arrays only, length is required for pointer arrays)`
The all-lower-case macros can take a variable length of parameters.
These figure out the parameters internally, and then call their upper-case sibling.

``` C
int intarray[10] = {1, 2, 3, 4, 5 , 6, 7, 8, 9, 10};
//Both allowed
bool contains = array_contains(5, intArray); // length is calculated internally if not passed.
bool contains = array_contains(5, intArray, sizeof(int) * 5); // Length is passed, only half the array will be searched.
```

##### Upper-Case Macros (Array_Contains, Array_IndexOf, Array_LastIndexOf)
`These are for internal features, but are available to use.`

`All parameters are required`

These upper case siblings have a locked parameter count.

``` C
int intarray[10] = {1, 2, 3, 4, 5 , 6, 7, 8, 9, 10};
bool contains = Array_Contains(5, intArray); // Not Allowed
bool contains = array_contains(5, intArray, sizeof(intarray); // Required
```


## Author

- Author   : Addio from Addio Electronics (Canada)
- Website  : www.Addio.io

