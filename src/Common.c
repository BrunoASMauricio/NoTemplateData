#include "Common.h"

#ifdef ENABLE_SANITY_CHECKS

#include <assert.h>
void AssertSaneOpaqueMemory(OPAQUE_MEMORY* Opaque) {
    Assert(Opaque != NULL);
    if (Opaque->Allocated == TRUE) {
        Assert(Opaque->Data != NULL);
        Assert(Opaque->Size > 0);
    }
}

#endif

void* DuplicateGenericMemory(void* Base, size_t Size) {
    void* NewAddress = AllocGenericMemory(Size);
    CopyAVGMemory(NewAddress, Base, Size);
    return NewAddress;
}

void SetupOpaqueMemory(OPAQUE_MEMORY* Opaque, size_t Size) {
    Opaque->Size = Size;
    if (Size > 0) {
        Opaque->Data = AllocGenericMemory(Opaque->Size);
        if (Opaque->Data != NULL) {
            Opaque->Allocated = TRUE;
            return;
        }
    }
    // error fall through
    Opaque->Data = NULL;
    Opaque->Allocated = FALSE;

}

OPAQUE_MEMORY DuplicateIntoOpaqueMemory(void* Base, size_t Size) {
    OPAQUE_MEMORY Opaque;
    Opaque.Size = Size;
    Opaque.Data = DuplicateGenericMemory(Base, Size);
    if (Opaque.Data != NULL) {
        Opaque.Allocated = TRUE;
    } else {
        Opaque.Allocated = FALSE;
    }
    return Opaque;
}

OPAQUE_MEMORY* AllocateOpaqueMemory(size_t Size) {
    ALLOC_STRUCT(OPAQUE_MEMORY, Opaque);
    SetupOpaqueMemory(Opaque, Size);
    return Opaque;
}

void ClearOpaqueMemory(OPAQUE_MEMORY* Opaque) {
    if (Opaque->Allocated == TRUE) {
        FreeGenericMemory(Opaque->Data);
    }
    Opaque->Allocated = FALSE;
}

void FreeOpaqueMemory(OPAQUE_MEMORY* Opaque) {
    ClearOpaqueMemory(Opaque);
    FreeGenericMemory(Opaque);
}
