#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

//                      stdlib.h wrappers
#define Malloc(BaseAddress) malloc(BaseAddress)
#define Free(BaseAddress) free(BaseAddress)
#define Assert(Condition) assert(Condition)

//                      string.h wrappers
#define Memcpy memcpy

//                      type wrappers
#ifndef BOOLEAN
typedef unsigned char BOOLEAN;
#define TRUE  ((BOOLEAN)(1==1))
#define FALSE ((BOOLEAN)(0==1))
#endif

//                      Helper macros
#define GLUE(X, Y) X ## Y

/* struct typedef which allows self-referencing without needing the struct
 * keyword in the fields type
 */
#define TYPE_STRUCT(NAME) \
typedef struct NAME NAME; \
struct NAME

#define ALLOC_STRUCT(TYPE, VAR) \
TYPE* VAR = (TYPE*)Malloc(sizeof(TYPE))

/* Toggleable sanity checks */
#ifdef ENABLE_SANITY_CHECKS
#define SANITY_CHECK(...) __VA_ARGS__
#else
#define SANITY_CHECK(...)
#endif

//                      Compiler wrappers
#define RUN_BEFORE_MAIN __attribute__((constructor))

//          Common data structures, their macros and functions

/* Copy the data provided into a new generic memory location */
void* DuplicateGenericMemory(void* Base, size_t Size);

/* Union for most of the useful data types.
 * For optimization purposes, keep size under sizeof(void*)
 */
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

/* Encapsulate static data into the appropriate OPAQUE_DATA field */
#define GENERIC_DATA(Type, Data) \
(OPAQUE_DATA){ .GLUE(Val_, Type) = Data}

/* Struct for generic memory manipulation
 * Allocated reports whether the `Data` shall be freed or not
 */
TYPE_STRUCT(OPAQUE_MEMORY){
    size_t  Size;
    BOOLEAN Allocated;
    void*   Data;
};

/* Encapsulate static data into an OPAQUE_MEMORY struct */
#define GENERIC_MEMORY(Size, Data) \
((OPAQUE_MEMORY){Size, FALSE, Data})

#define STATIC_MEMORY(Size, ...) \
{Size, FALSE, (uint8_t[]){__VA_ARGS__} }

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

#ifdef ENABLE_SANITY_CHECKS

/* Assert Opaque Memory is sane */
void AssertSaneOpaqueMemory(OPAQUE_MEMORY* Opaque);

#endif

#endif /* COMMON_H */