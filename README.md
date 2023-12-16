# No Template

This project is a simple exercise in how one can store "any type" data in list-like structures, without using `C++ templates`, nor copying the memory via `malloc + memcpy`.

There is an assumption that the data to be stored can fit in the `opaque_data` union defined (i.e. it isn't a structure with more than `N = MAX(sizeof(void*), sizeof(uint64_t))` bytes).

If that is a requirement, memory allocation and copying (without simple data loads/stores) will be necessary and this project, making this project unfit for that use case.

The reasoning behind this experiment is that a lot of data structures don't manipulate data, but simply hold it, at most copying it. So, to save memory in having multiple specializations of a single template data structure, we waste `N - (sizeof(stored_type))` per stored element.

Growing amounts of data vs template specializations, can quickly defeat the purpose, so caution is advised.
