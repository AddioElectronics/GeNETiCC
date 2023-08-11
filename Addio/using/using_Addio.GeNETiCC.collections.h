#ifndef USING_GENETICC_COLLECTIONS_H_
#define USING_GENETICC_COLLECTIONS_H_

#if __has_include("Addio/GeNETiCC/GeNETiCC.h")


#include "Addio/GeNETiCC/geneticc.h"

#include "Addio/GeNETiCC/collections/array/array_geneticc.h"
#include "Addio/GeNETiCC/collections/list/list_geneticc.h"
//#include "Addio/GeNETiCC/collections/dictionary/dictionary_geneticc.h"
//#include "Addio/GeNETiCC/collections/enumerator/enumerator_geneticc.h"

#else
#error Library does not exist.
#endif

#endif /* USING_GENETICC_COLLECTIONS_H_ */