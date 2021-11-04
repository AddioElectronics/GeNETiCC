
#ifndef GENETICC_ENUMERABLE_INTERNAL_FUNCTIONS_H_
#define GENETICC_ENUMERABLE_INTERNAL_FUNCTIONS_H_


/*
In the future, all the functions that reside here https://docs.microsoft.com/en-us/dotnet/api/system.linq.enumerable.all?view=net-5.0, 
that are currently in the Array and List internal functions will be moved here.

The Array and List macros will call into these functions, as long as it is reasonable to do so.
This is to allow any future "types" to share the functions, keeping program size as small as possible,
as well as keeping it as close to C#/.NET as possible.
*/



#endif /* GENETICC_ENUMERABLE_INTERNAL_FUNCTIONS_H_ */