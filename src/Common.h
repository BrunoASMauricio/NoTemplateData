#ifndef COMMONH_H
#define COMMONH_H

#include <stdint.h>

typedef union{
    uint8_t    uint8_t_val;
    uint16_t   uint16_t_val;
    uint32_t   uint32_t_val;
    uint64_t   uint64_t_val;

    int8_t    int8_t_val;
    int16_t   int16_t_val;
    int32_t   int32_t_val;
    int64_t   int64_t_val;

    int    int_val;
    float  float_val;
    double double_val;

    void* pointer;
}opaque_data;

#define GLUE(X, Y) X ## Y

#define OPAQUE_DATA(type, data) \
(opaque_data){ .GLUE(type, _val) = data}

#define TYPE_STRUCT(NAME) \
typedef struct NAME NAME; \
struct NAME

#endif