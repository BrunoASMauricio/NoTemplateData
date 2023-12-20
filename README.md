# No Template

This project is an exercise in providing an alternative to `C++ templates`, for C.

The motivation behind is is in preventing code duplication for data structures
that solely manage data regardless of type.

For primitive data types, there is no memory copy nor explicit allocation
(no `malloc + memcpy`).

Instead, a union is used with most common data types.
Benefits:

1. Generic.
2. No explicit memory allocation/copies
3. If union size remains less than `N = MAX(sizeof(void*), sizeof(uint64_t))`,
little to no implicit memory copies will be inserted by the compiler

Cons:

1. Ugly cast required (either `GENERIC_DATA(Type, Data)` or `(OPAQUE_DATA)
{ .GLUE(Val_, Type) = Data})`
1. Increased user responsibility. No differing data type warning.
2. Differences in size between the used union and the target data type lead to
unused memory

The reasoning behind this experiment is that a lot of data structures don't
manipulate data besides copying it around. For primitive data types this usually
either requires a copy of the entire structure (i.e. C++ templates) or allocating
new memory via malloc and copying the data over with memcpy.

Since we always store the union, the difference between its' size and the target
data types' size dictates the amount of wasted memory.

If the amount of data stored is extremely large, it might be better to just use
a specialized version (i.e. via templates)

## Relevant defines

**Functionality switches**:

* SANITY_CHECK - Enable some sanity checks. NULL pointers aren't checked here
                  either since segfaults are easy to pinpoint (in non embedded or
                  in simulators)

**Wrappers**:

* AllocGenericMemory - Wrapper for malloc
* FeeGenericMemory   - Wrapper for free
* Assert             - Wrapper for assert
* CopyAVGMemory      - Wrapper for memcpy
* CmpAVGMemory       - Wrapper for memcmp

## Some metrics

Let us run some comparisons with C++ list templates.
At the end of this README there are two pieces of code, one with C++ lists and
one with this implementation.

Compiling both executables with `-O2`, we get this data for executable size:

 | text  |  data  |  bss  |   dec   |  hex | filename             |
 |-------|--------|-------|---------|------| ---------------------|
 | 2244  |   600  |    8  |  2852   |  b24 | exe/no_template.exe  |
 | 2834  |   680  |    8  |  3522   |  dc2 | ./a.out (int/int)    |
 | 3070  |   680  |    8  |  3758   |  eae | ./a.out (int/float)  |

And the following data for runtime memory usage:

```shell
 $ valgrind -s --leak-check=full --track-origins=yes ./exe/no_template.exe
...
==24321== HEAP SUMMARY:
==24321==     in use at exit: 0 bytes in 0 blocks
==24321==   total heap usage: 11 allocs, 11 frees, 1,168 bytes allocated
...

 $ valgrind -s --leak-check=full --track-origins=yes ./a.out
...
==24418== HEAP SUMMARY:
==24418==     in use at exit: 0 bytes in 0 blocks
==24418==   total heap usage: 10 allocs, 10 frees, 74,944 bytes allocated
...
```

As expected with a higher-level language, when using `-O2`, the executable
ditches both unused and repeated code and thier sizes go from 7679 and 12253
bytes to 2834 and 3070 bytes respectively.

However, we can see that the C++ executables consume an order extra of bytes
compared to our implementation.

This is surely due to extra functionality provided (even if not used) and
optimization for larger data amounts.

```c++
#include <stdio.h>
#include <list>
using namespace std;

int main(int argc, char** argv) {
    list<int> a;
    list<float> a2;         // list<int> a2;

    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);

    for(int& b: a) {
        printf("%d\n", b);
    }

    a2.push_back(1);
    a2.push_back(2);
    a2.push_back(3);
    a2.push_back(4);

    for(float& b: a2) {       // for(int& b: a2) {
        printf("%f\n", b);    //     printf("%d\n", b);
    }
}
```

```C++
#include <stdio.h>

#include "BasicList.h"

int main(void){
    LIST* IntMemoryList = NewList();
    LIST* FloatMemoryList = NewList();

    ListInsertPrimitiveData(IntMemoryList, GENERIC_DATA(int, 1));
    ListInsertPrimitiveData(IntMemoryList, GENERIC_DATA(int, 2));
    ListInsertPrimitiveData(IntMemoryList, GENERIC_DATA(int, 3));
    ListInsertPrimitiveData(IntMemoryList, GENERIC_DATA(int, 4));

    int IntData;
    ITERATE_PRIMITIVE_DATA_TYPE(IntMemoryList, int, IntData) {
        printf("%d\n", IntData);
    }

    ListInsertPrimitiveData(FloatMemoryList, GENERIC_DATA(float, 1));
    ListInsertPrimitiveData(FloatMemoryList, GENERIC_DATA(float, 2));
    ListInsertPrimitiveData(FloatMemoryList, GENERIC_DATA(float, 3));
    ListInsertPrimitiveData(FloatMemoryList, GENERIC_DATA(float, 4));

    float FloatData;
    ITERATE_PRIMITIVE_DATA_TYPE(FloatMemoryList, float, FloatData) {
        printf("%f\n", FloatData);
    }

    ListFreePrimitiveData(IntMemoryList);
    ListFreePrimitiveData(FloatMemoryList);
}
```
