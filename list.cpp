#include <stdio.h>
#include <stdlib.h>

#include "list.h"

void ListDtor (List* list)
{
    free (list->list_elem);
    list->head = 0;
    list->tail = 0;
    free (list);
}

// UpdateListOfFree
void ChangingListOfFree (List* list)
{
    list->list_free_elems[list->head_of_free].data = FILLED_ELEM;
    int pre_head_of_free = list->head_of_free;
    list->head_of_free = list->list_free_elems[list->head_of_free].next;
    list->list_free_elems[pre_head_of_free].next = NO_CONNECTION;
}

void InsertBefore (List* list, int place_before, Elem_list number)
{
    if (place_before < 1 || list->list_elem[place_before].prev == NO_CONNECTION)
    {
        printf ("The element cannot be inserted into this place!\n");
        return;
    }
    // случай, когда вставляется перед первым элементом
    if (place_before == 1)
    {
        list->list_elem[list->head_of_free].data = number;
        list->list_elem[list->head_of_free].next = list->head;
        list->list_elem[list->head_of_free].prev = 0;

        list->list_elem[list->head].prev = list->head_of_free;
        list->head = list->head_of_free;
        list->list_elem[0].next = list->head;

        ChangingListOfFree (list);
        return;
    }
    // случай, когда вставляется перед элемпентом из середины
    list->list_elem[list->head_of_free].data = number;
    list->list_elem[list->head_of_free].next = place_before;
    // prev от того перед которым вставили
    list->list_elem[list->head_of_free].prev = list->list_elem[place_before].prev;

    // #define NEXT(i) list->list_elem[i].next
    // меняем next ячейки перед которой вставили
    list->list_elem[list->list_elem[place_before].prev].next = list->head_of_free;
    // меняем prev от места до которого вставили
    list->list_elem[place_before].prev = list->head_of_free;

    // элеменнт в который вставили становится занят
    ChangingListOfFree (list);
}

void InsertAfter (List* list, int place_after, Elem_list number)
{
    if (place_after < 1 || list->list_elem[place_after].prev == NO_CONNECTION)
    {
        printf ("The element cannot be inserted into this place!\n");
        return;
    }
    // InsertBefore
    list->list_elem[list->head_of_free].data = number;
    list->list_elem[list->head_of_free].next = list->list_elem[place_after].next;
    list->list_elem[list->head_of_free].prev = place_after;

    list->list_elem[list->list_elem[place_after].next].prev = list->head_of_free;
    list->list_elem[place_after].next = list->head_of_free;

    ChangingListOfFree (list);
}

void ListPush (List* list, Elem_list number)
{
    list->list_elem[list->head_of_free].data = number;
    list->list_elem[list->head_of_free].next = 0; // так как последний элемент то next 0
    list->list_elem[list->head_of_free].prev = list->tail;
    list->list_elem[list->tail].next = list->head_of_free;
    list->tail = list->head_of_free;

    list->list_elem[0].prev = list->tail;
    if (list->head == 0)
    {
        list->head = 1;
    }

    ChangingListOfFree (list);
}

// ElemDelete
void Delete (List* list, int place)
{
    if (place < 1 || list->list_elem[place].prev == NO_CONNECTION)
    {
        printf ("There is no such element!\n");
        return;
    }

    list->list_elem[place].data = 0; // изначально все нулями заполняет, то есть пустая ячейка
    // меняется next того после которого удалили
    list->list_elem[list->list_elem[place].prev].next = list->list_elem[place].next;
    // меняется prev того перед которым удалили
    list->list_elem[list->list_elem[place].next].prev = list->list_elem[place].prev;
    list->list_elem[place].next = NO_CONNECTION;
    list->list_elem[place].prev = NO_CONNECTION;

    if (list->list_elem[place].prev == 0)
    {
        list->head = list->list_elem[place].next;
    }

    if (list->list_elem[place].next == 0)
    {
        list->tail = list->list_elem[list->tail].prev;
    }

    list->list_free_elems[place].data = FREE_ELEM;
    list->list_free_elems[place].next = list->head_of_free;
    list->head_of_free = place;
}

void ListCtor (List* list, int size)
{
    list->list_elem = (ListNode*)calloc (size, sizeof (ListNode));
    list->list_elem[0].data = nullptr;
    list->head = 0;
    list->tail = 0;

    list->list_elem[0].next = list->head;
    list->list_elem[0].prev = list->tail;

    for (int i = 1; i < size; i++)
    {
        list->list_elem[i].next = NO_CONNECTION;
        list->list_elem[i].prev = NO_CONNECTION;
    }

    list->list_free_elems = (FreeElemsListNode*)calloc (size, sizeof (FreeElemsListNode));
    list->list_free_elems[0].data = -8;
    list->list_free_elems[0].next = 1;
    list->head_of_free = 1;

    list->size = size;

    for (int i = 1; i < size; i++)
    {
        list->list_free_elems[i].data = FREE_ELEM;
        if (i != size - 1)
        {
            list->list_free_elems[i].next = i + 1;
        }
    }
}

