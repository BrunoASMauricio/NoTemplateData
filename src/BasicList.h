#ifndef BASIC_LIST_HPP
#define BASIC_LIST_HPP

#include "Common.h"

// Primitive Data element
TYPE_STRUCT(PRIMITIVE_DATA_ELEMENT){
    PRIMITIVE_DATA_ELEMENT* Next;
    OPAQUE_DATA             Data;
};

// Memory Based Data element
TYPE_STRUCT(MEMORY_DATA_ELEMENT){
    MEMORY_DATA_ELEMENT*    Next;
    OPAQUE_MEMORY           Data;
};

TYPE_STRUCT(LIST){
    void* Head;
};


#define ITERATE_PRIMITIVE_DATA_TYPE(List, Type, Var)                \
for (PRIMITIVE_DATA_ELEMENT* DataElement = ((List)->Head);          \
    (DataElement) && ((Var) = ((DataElement)->Data.GLUE(Type, Val)), 1);   \
        (DataElement) = (DataElement->Next))


void ListInsertPrimitiveData(LIST* List, OPAQUE_DATA NewData);
void ListInsertMemoryData(LIST* List, OPAQUE_DATA NewData);


LIST* NewList();

void ListFreePrimitiveData(LIST* List);


#endif