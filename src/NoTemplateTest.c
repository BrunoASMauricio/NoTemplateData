#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "BasicList.h"
#include "NoTemplateTest.h"

void TestPrimitiveData(void);
void TestMemoryData(void);

void TestPrimitiveData(void) {
    LIST* DataList = NewList();

    uint64_t TestVector[] = {
        0x0123456789abcdef,
        0x0000000056789abc,
        0x0000000000009abc,
        0x0000000040133333 // 2.3 (2.29999) floats' 64 bit representation
    };

    uint64_t SomeVar = 0x0123456789abcdef;
    DataListInsert(DataList, GENERIC_DATA(uint64_t, SomeVar));
    DataListInsert(DataList, GENERIC_DATA(int, 0x56789abc));
    DataListInsert(DataList, GENERIC_DATA(int, 0x9abc));
    DataListInsert(DataList, GENERIC_DATA(float, 2.3));

    uint64_t StoredData;
    int Index = 0;

    ITERATE_PRIMITIVE_DATA_TYPE(DataList, uint64_t, StoredData) {
        Assert(StoredData == TestVector[Index]);
        Index++;
    }

    FreeDataList(DataList);
}

void TestMemoryData(void) {
    LIST* MemoryList = NewList();

    // Setup some static memory
    OPAQUE_MEMORY StaticTestVector[] = {
        {0},
        STATIC_MEMORY(1, 0x00),
        STATIC_MEMORY(4, 0x56, 0x78, 0x9a, 0xbc),
        STATIC_MEMORY(8, 0x01, 0x23, 0x45, 0x67, 0x56, 0x78, 0x9a, 0xbc)
    };

    // Setup some dynamic memory
    OPAQUE_MEMORY DynTestVector[] = {
        {0},
        {1, TRUE, AllocGenericMemory(1)},
        {4, TRUE, AllocGenericMemory(4)},
        {8, TRUE, AllocGenericMemory(8)}
    };
    // just copy the static stuff
    for (size_t i = 0; i < 4; i++) {
        CopyAVGMemory(DynTestVector[i].Data, StaticTestVector[i].Data,
                      StaticTestVector[i].Size);
    }

    // Insert static memory (backwards so validation can use normal order)
    for (int i = 0; i < 4; i++) {
        MemoryListInsert(MemoryList, StaticTestVector[i]);
    }

    // Insert dynamic memory (backwards so validation can use normal order)
    for (int i = 0; i < 4; i++) {
        MemoryListInsert(MemoryList, DynTestVector[i]);
    }

    // Validate memory
    OPAQUE_MEMORY ListMemory;
    int Index = 0;
    ITERATE_MEMORY_TYPE(MemoryList, ListMemory) {
        Assert(0 == memcmp(ListMemory.Data, StaticTestVector[Index].Data, ListMemory.Size));
        Index++;
        // Cycle back, we reuse the StaticTestVector
        Index = Index % 4;
    }

    FreeMemoryList(MemoryList);
}

int main(void){
    TestPrimitiveData();
    TestMemoryData();
    return 0;
}