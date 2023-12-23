#ifndef BASIC_LIST_HPP
#define BASIC_LIST_HPP

#include "Common.h"

/* For NO_DATA_ELEMENT to be compatible with Primitive and Memory, Next
 * must always be the first element
 */
TYPE_STRUCT(NO_DATA_ELEMENT) {
    NO_DATA_ELEMENT* Next;
};

// Primitive Data element
TYPE_STRUCT(PRIMITIVE_DATA_ELEMENT){
    PRIMITIVE_DATA_ELEMENT* Next;
    OPAQUE_DATA             Data;
};

// Memory Based Data element
TYPE_STRUCT(MEMORY_DATA_ELEMENT){
    MEMORY_DATA_ELEMENT*    Next;
    OPAQUE_MEMORY           Memory;
};

#ifdef SANITY_CHECK
typedef enum{
    NoType,
    PrimitiveDataType,
    MemoryDataType
}LIST_DATA_TYPE;
#endif

TYPE_STRUCT(LIST){
    #ifdef SANITY_CHECK
    LIST_DATA_TYPE InsertedTypes;
    #endif
    void* Head;
};


#define ITERATE_PRIMITIVE_DATA_TYPE(List, Type, Var)                        \
for (PRIMITIVE_DATA_ELEMENT* DataElement = ((List)->Head);                  \
    (DataElement) && ((Var) = ((DataElement)->Data.GLUE(Val_, Type)), 1);   \
        (DataElement) = (DataElement->Next))

#define ITERATE_MEMORY_TYPE(List, Var)                    \
for (MEMORY_DATA_ELEMENT* DataElement = ((List)->Head);   \
    (DataElement) && ((Var) = ((DataElement)->Memory), 1);\
        (DataElement) = (DataElement->Next))

LIST* NewList(void);

void DataListInsert(LIST* List, OPAQUE_DATA NewData);
void MemoryListInsert(LIST* List, OPAQUE_MEMORY NewMemory);

void FreeDataList(LIST* List);
void FreeMemoryList(LIST* List);


#endif