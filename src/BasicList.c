#include <stdlib.h>

#include "BasicList.h"

LIST* NewList() {
    ALLOC_STRUCT(LIST, NewList);
    NewList->Head = NULL;
    return NewList;
}

void ListInsertPrimitiveData(LIST* List, OPAQUE_DATA NewData) {
    ALLOC_STRUCT(PRIMITIVE_DATA_ELEMENT, NewLink);
    NewLink->Data = NewData;

    if (List->Head == NULL) {
        NewLink->Next   = NULL;
        List->Head      = NewLink;
    } else {
        NewLink->Next   = List->Head;
        List->Head      = NewLink;
    }
}

void ListFreePrimitiveData(LIST* List) {
    PRIMITIVE_DATA_ELEMENT* Current = List->Head;
    while(Current != NULL) {
        PRIMITIVE_DATA_ELEMENT* Next = Current->Next;
        FeeGenericMemory(Current);
        Current = Next;
    }
    FeeGenericMemory(List);
}