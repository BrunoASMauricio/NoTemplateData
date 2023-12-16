#ifndef BASIC_LIST_HPP
#define BASIC_LIST_HPP

#include "Common.h"

TYPE_STRUCT(ListEl){
    opaque_data data;
    ListEl* next;
};

TYPE_STRUCT(List){
    ListEl* head;
};


#define ITERATE(list, type, var)                                    \
for (ListEl* iter_el = ((list)->head);                              \
    (iter_el) && ((var) = ((iter_el)->data.GLUE(type, _val)), 1);   \
        (iter_el) = (iter_el->next))

void insert(List* iter, opaque_data obj);
List* new_list();
void free_list(List* list);


#endif