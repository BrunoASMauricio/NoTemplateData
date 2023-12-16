#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "BasicList.h"

int main(int argc, char* argv[]){
    printf("Template built-in test for repository testproject1\n");
    List* my_list = new_list();

    uint32_t test_vector[] = {
        0x40133333, // 2.3 (2.29999) floats' binary representation
        0x00009abc,
        0x56789abc,
        0x01234567
    };

    int a = 0x01234567;
    insert(my_list, OPAQUE_DATA(int, a));
    insert(my_list, OPAQUE_DATA(int, 0x56789abc));
    insert(my_list, OPAQUE_DATA(int, 0x9abc));
    insert(my_list, OPAQUE_DATA(float, 2.3));

    uint32_t obj;
    int ind = 0;
    ITERATE(my_list, uint32_t, obj) {
        assert(obj == test_vector[ind]);
        ind++;
    }

    free_list(my_list);

    return 0;
}