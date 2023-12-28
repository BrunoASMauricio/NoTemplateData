#ifndef COMMONH_H
#define COMMONH_H

#include <stdint.h>
#include <stddef.h>

//                      stdlib.h wrappers
#ifndef AllocGenericMemory
#include <stdlib.h>
#define AllocGenericMemory(BaseAddress) malloc(BaseAddress)
#endif

#ifndef FreeGenericMemory
#include <stdlib.h>
#define FreeGenericMemory(BaseAddress) free(BaseAddress)
#endif

#ifndef Assert
#define Assert(Condition) assert(Condition)
#endif

//                      string.h wrappers
#ifndef CopyAVGMemory
#include <string.h>
#define CopyAVGMemory memcpy
#endif

#ifndef CmpAVGMemory
#include <string.h>
#define CmpAVGMemory memcmp
#endif

//                      type wrappers
#ifndef BOOLEAN
typedef unsigned char BOOLEAN;
#define TRUE  ((BOOLEAN)(1==1))
#define FALSE ((BOOLEAN)(0==1))
#endif


//                      Helper macros
#define GLUE(X, Y) X ## Y

#define TYPE_STRUCT(NAME) \
typedef struct NAME NAME; \
struct NAME

#define ALLOC_STRUCT(TYPE, VAR) \
TYPE* VAR = (TYPE*)AllocGenericMemory(sizeof(TYPE))

#ifdef SANITY_CHECK
#define SANITY_ASSERT(Condition) Assert(Condition)
#else
#define SANITY_ASSERT(Condition)
#endif

//                      Compiler wrappers
#define RUN_BEFORE_MAIN __attribute__((constructor))


//          Common data structures, their macros and functions

/* Copy the data provided into a new generic memory location */
void* DuplicateGenericMemory(void* Base, size_t Size);

typedef union{
    uint8_t     Val_uint8_t;
    uint16_t    Val_uint16_t;
    uint32_t    Val_uint32_t;
    uint64_t    Val_uint64_t;
    int8_t      Val_int8_t;
    int16_t     Val_int16_t;
    int32_t     Val_int32_t;
    int64_t     Val_int64_t;
    size_t      Val_size_t;
    int         Val_int;
    float       Val_float;
    double      Val_double;

    intptr_t    Val_intptr_t;
    uintptr_t   Val_uintptr_t;

    void*       Val_pointer;
}OPAQUE_DATA;

#define GENERIC_DATA(Type, Data) \
(OPAQUE_DATA){ .GLUE(Val_, Type) = Data}


TYPE_STRUCT(OPAQUE_MEMORY){
    size_t  Size;
    BOOLEAN Allocated;
    void*   Data;
};

/* Setup new memory for the provided `Memory`.
 * Allocates `Size` bytes from generic memory
 */
void SetupOpaqueMemory(OPAQUE_MEMORY* Opaque, size_t Size);

/* Allocate a new OPAQUE_MEMORY and set it up */
OPAQUE_MEMORY* AllocateOpaqueMemory(size_t Size);

/* Free allocated data if (Allocated == True) */
void ClearOpaqueMemory(OPAQUE_MEMORY* Opaque);

/* Duplicate the memory in `Base` and wrap it with an Opaque memory */
OPAQUE_MEMORY DuplicateIntoOpaqueMemory(void* Base, size_t Size);

/* Clear Opaque structure and release its' memory */
void FreeOpaqueMemory(OPAQUE_MEMORY* Opaque);

#define GENERIC_MEMORY(Size, Data) \
((OPAQUE_MEMORY){Size, FALSE, Data})


#endif