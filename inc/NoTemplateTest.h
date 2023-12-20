#ifndef NO_TEMPLATE_TEST_H
#define NO_TEMPLATE_TEST_H

#define STATIC_MEMORY(Size, ...) \
{Size, FALSE, (uint8_t[]){__VA_ARGS__} }

#endif