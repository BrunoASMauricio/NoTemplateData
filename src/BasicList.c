#include <stdlib.h>
#include <assert.h>

#include "Common.h"
#include "BasicList.h"

#ifdef SANITY_CHECK
#include <stdint.h>
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
    NewList->Head   = NULL;
    NewList->Tail   = NULL;
    NewList->Length = 0;

    #ifdef SANITY_CHECK
    NewList->InsertedTypes = NoType;
    #endif

    return NewList;
}


static void AddListElement(LIST* List, void* _NewLink) {
    NO_DATA_ELEMENT* NewLink = _NewLink;
    NO_DATA_ELEMENT* Tail = List->Tail;
    if (List->Head == NULL) {
        NewLink->Next   = NULL;
        List->Head      = NewLink;
        List->Tail      = NewLink;
    } else {
        Tail->Next = NewLink;
        List->Tail = NewLink;
    }
    List->Length += 1;
}

void MemoryListInsert(LIST* List, OPAQUE_MEMORY NewMemory) {
    ALLOC_STRUCT(MEMORY_DATA_ELEMENT, NewLink);
    NewLink->Memory = NewMemory;
    NewLink->Next = NULL;

    AddListElement(List, NewLink);

    #ifdef SANITY_CHECK
    ValidateInsertion(List, MemoryDataType);
    #endif
}

void DataListInsert(LIST* List, OPAQUE_DATA NewData) {
    ALLOC_STRUCT(PRIMITIVE_DATA_ELEMENT, NewLink);
    NewLink->Data = NewData;
    NewLink->Next = NULL;

    AddListElement(List, NewLink);

    #ifdef SANITY_CHECK
    ValidateInsertion(List, PrimitiveDataType);
    #endif
}

OPAQUE_MEMORY* SerializeDataList(LIST* List, size_t ElementSize) {
    OPAQUE_MEMORY* Total = AllocateOpaqueMemory(ElementSize * List->Length);
    uint8_t* MemoryIndex;

    MemoryIndex = (uint8_t*)Total->Data;

    uintptr_t Element;
    ITERATE_PRIMITIVE_DATA_TYPE(List, uintptr_t, Element) {
        int Start, Direction;
        if (BYTE_ORDER == LITTLE_ENDIAN) {
            Start = 0;
            Direction = 1;
        } else {
            assert(0);
            Start = sizeof(OPAQUE_DATA) - 1;
            Direction = -1;
        }
        // 0 -> ElementSize
        for(int Ind = Start; (Ind < (int)ElementSize && Ind >= 0); Ind += Direction) {
            MemoryIndex[Ind] = Element & 0xFF;
            Element = Element >> 8;
        }
        MemoryIndex += ElementSize;
    }

    return Total;
}

LIST* DeSerializeDataList(OPAQUE_MEMORY* Memory, size_t ElementSize) {
    uint8_t* MemoryIndex = Memory->Data;
    intptr_t Field;
    LIST* List = NewList();
    while (MemoryIndex < (uint8_t*)Memory->Data + Memory->Size) {
        // Assume same endianness
        CopyAVGMemory(&Field, MemoryIndex, ElementSize);
        DataListInsert(List, GENERIC_DATA(intptr_t, Field));
        MemoryIndex += ElementSize;
    }
    SANITY_ASSERT(MemoryIndex == (uint8_t*)Memory->Data + Memory->Size);
    return List;
}

size_t SerializedMemoryListSize(LIST* List) {
    size_t TotalSize = 0;
    OPAQUE_MEMORY Element;
    ITERATE_MEMORY_TYPE(List, Element) {
        TotalSize += sizeof(Element.Size);
        TotalSize += Element.Size;
    }
    return TotalSize;
}

LIST* DeSerializeMemoryList(OPAQUE_MEMORY* Memory) {
    uint8_t* MemoryIndex = Memory->Data;
    size_t FieldSize;
    LIST* List = NewList();
    while (MemoryIndex < (uint8_t*)Memory->Data + Memory->Size) {
        // Assume same endianness
        CopyAVGMemory(&FieldSize, MemoryIndex, sizeof(FieldSize));
        MemoryIndex += sizeof(FieldSize);
        MemoryListInsert(List, DuplicateIntoOpaqueMemory(MemoryIndex, FieldSize));
        MemoryIndex += FieldSize;
    }
    SANITY_ASSERT(MemoryIndex == (uint8_t*)Memory->Data + Memory->Size);
    return List;
}

OPAQUE_MEMORY* SerializeMemoryList(LIST* List) {
    OPAQUE_MEMORY* Total = AllocateOpaqueMemory(SerializedMemoryListSize(List));

    uint8_t* MemoryIndex;
    MemoryIndex = (uint8_t*)Total->Data;

    OPAQUE_MEMORY Element;
    ITERATE_MEMORY_TYPE(List, Element) {
        CopyAVGMemory(MemoryIndex, &(Element.Size), sizeof(Element.Size));
        MemoryIndex += sizeof(Element.Size);
        CopyAVGMemory(MemoryIndex, Element.Data, Element.Size);
        MemoryIndex += Element.Size;
    }
    return Total;
}

void FreeDataList(LIST* List) {
    PRIMITIVE_DATA_ELEMENT* Current = List->Head;
    while(Current != NULL) {
        PRIMITIVE_DATA_ELEMENT* Next = Current->Next;
        FreeGenericMemory(Current);
        Current = Next;
    }
    FreeGenericMemory(List);
}

void FreeMemoryList(LIST* List) {
    MEMORY_DATA_ELEMENT* Current = List->Head;
    while(Current != NULL) {
        MEMORY_DATA_ELEMENT* Next = Current->Next;
        ClearOpaqueMemory(&(Current->Memory));
        FreeGenericMemory(Current);
        Current = Next;
    }
    FreeGenericMemory(List);
}