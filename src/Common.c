#include "Common.h"

void FreeOpaqueMemory(OPAQUE_MEMORY* Memory) {
    if (Memory->Allocated) {
        FeeGenericMemory(Memory->Data);
    }
    Memory->Allocated = FALSE;
}
