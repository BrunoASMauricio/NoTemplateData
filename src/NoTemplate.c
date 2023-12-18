#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "BasicList.h"

int main(int argc, char* argv[]){
    printf("Template built-in test for repository testproject1\n");
    LIST* List = NewList();

    uint32_t TestVector[] = {
        0x40133333, // 2.3 (2.29999) floats' binary representation
        0x00009abc,
        0x56789abc,
        0x01234567
    };

    int SomeVar = 0x01234567;
    ListInsertPrimitiveData(List, CAST_OPAQUE_DATA(int, SomeVar));
    ListInsertPrimitiveData(List, CAST_OPAQUE_DATA(int, 0x56789abc));
    ListInsertPrimitiveData(List, CAST_OPAQUE_DATA(int, 0x9abc));
    ListInsertPrimitiveData(List, CAST_OPAQUE_DATA(float, 2.3));

    uint32_t obj;
    int Index = 0;
    ITERATE_PRIMITIVE_DATA_TYPE(List, uint32_t, obj) {
        assert(obj == TestVector[Index]);
        Index++;
    }

    ListFreePrimitiveData(List);

    return 0;
}