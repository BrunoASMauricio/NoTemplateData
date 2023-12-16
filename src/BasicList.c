#include <stdlib.h>

#include "BasicList.h"

List* new_list() {
    List* list = (List*)malloc(sizeof(List));
    list->head = NULL;
    return list;
}


void insert(List* list, opaque_data obj) {
    ListEl* new_link = (ListEl*)malloc(sizeof(ListEl));
    new_link->data = obj;

    if (list->head == NULL) {
        new_link->next = NULL;
        list->head = new_link;
    } else {
        new_link->next  = list->head;
        list->head      = new_link;
    }
}

void free_list(List* list) {
    ListEl* current = list->head;
    while(current != NULL) {
        ListEl* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}