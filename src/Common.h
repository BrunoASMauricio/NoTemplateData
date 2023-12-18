#ifndef COMMONH_H
#define COMMONH_H

#include <stdint.h>

#ifndef AllocGenericMemory
#define AllocGenericMemory(BaseAddress) malloc(BaseAddress)
#endif

#ifndef FeeGenericMemory
#define FeeGenericMemory(BaseAddress) free(BaseAddress)
#endif


#define TYPE_STRUCT(NAME) \
typedef struct NAME NAME; \
struct NAME

#define ALLOC_STRUCT(TYPE, VAR) \
TYPE* VAR = (TYPE*)AllocGenericMemory(sizeof(TYPE))


typedef union{
    uint8_t     uint8_tVal;
    uint16_t    uint16_tVal;
    uint32_t    uint32_tVal;
    uint64_t    uint64_tVal;
    int8_t      int8_tVal;
    int16_t     int16_tVal;
    int32_t     int32_tVal;
    int64_t     int64_tVal;
    int         intVal;
    float       floatVal;
    double      doubleVal;

    void* pointer;
}OPAQUE_DATA;

TYPE_STRUCT(OPAQUE_MEMORY){
    size_t size;
    void*  data;
};

#define GLUE(X, Y) X ## Y

#define CAST_OPAQUE_DATA(type, data) \
(OPAQUE_DATA){ .GLUE(type, Val) = data}


#endif