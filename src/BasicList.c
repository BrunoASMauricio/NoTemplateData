#include <stdlib.h>

#include "Common.h"
#include "BasicList.h"

#ifdef SANITY_CHECK
#include <assert.h>
#include <stddef.h>
// Validate our usage of NO_DATA_ELEMENT (Next is at the same offset everywhere)
static_assert(offsetof(NO_DATA_ELEMENT, Next) ==
              offsetof(PRIMITIVE_DATA_ELEMENT, Next));

static_assert(offsetof(MEMORY_DATA_ELEMENT, Next) ==
              offsetof(PRIMITIVE_DATA_ELEMENT, Next));

// Validate we aren't mixing types
static void ValidateInsertion(LIST* List, LIST_DATA_TYPE DataType) {
    if (List->InsertedTypes == NoType) {
        List->InsertedTypes = DataType;
    } else {
        Assert(List->InsertedTypes == DataType);
    }
}
#endif

LIST* NewList(void) {
    ALLOC_STRUCT(LIST, NewList);
    NewList->Head = NULL;
    return NewList;
}


static void AddListElement(LIST* List, void* _NewLink) {
    NO_DATA_ELEMENT* NewLink = _NewLink;
    if (List->Head == NULL) {
        NewLink->Next   = NULL;
        List->Head      = NewLink;
    } else {
        NewLink->Next   = List->Head;
        List->Head      = NewLink;
    }
}

void MemoryListInsert(LIST* List, OPAQUE_MEMORY NewMemory) {
    ALLOC_STRUCT(MEMORY_DATA_ELEMENT, NewLink);
    NewLink->Memory = NewMemory;

    AddListElement(List, NewLink);

    #ifdef SANITY_CHECK
    ValidateInsertion(List, MemoryDataType);
    #endif
}

void DataListInsert(LIST* List, OPAQUE_DATA NewData) {
    ALLOC_STRUCT(PRIMITIVE_DATA_ELEMENT, NewLink);
    NewLink->Data = NewData;

    AddListElement(List, NewLink);

    #ifdef SANITY_CHECK
    ValidateInsertion(List, PrimitiveDataType);
    #endif
}

void FreeDataList(LIST* List) {
    PRIMITIVE_DATA_ELEMENT* Current = List->Head;
    while(Current != NULL) {
        PRIMITIVE_DATA_ELEMENT* Next = Current->Next;
        FeeGenericMemory(Current);
        Current = Next;
    }
    FeeGenericMemory(List);
}

void FreeMemoryList(LIST* List) {
    MEMORY_DATA_ELEMENT* Current = List->Head;
    while(Current != NULL) {
        MEMORY_DATA_ELEMENT* Next = Current->Next;
        ClearOpaqueMemory(&(Current->Memory));
        FeeGenericMemory(Current);
        Current = Next;
    }
    FeeGenericMemory(List);
}