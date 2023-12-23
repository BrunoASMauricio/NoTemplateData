#include "Common.h"

OPAQUE_MEMORY* AllocateOpaqueMemory(size_t Size) {
    ALLOC_STRUCT(OPAQUE_MEMORY, Memory);
    Memory->Size = Size;
    Memory->Data = AllocGenericMemory(Memory->Size);
    Memory->Allocated = TRUE;
    return Memory;
}

void ClearOpaqueMemory(OPAQUE_MEMORY* Memory) {
    if (Memory->Allocated == TRUE) {
        FeeGenericMemory(Memory->Data);
    }
    Memory->Allocated = FALSE;
}

void FreeOpaqueMemory(OPAQUE_MEMORY* Memory) {
    ClearOpaqueMemory(Memory);
    FeeGenericMemory(Memory);
}
