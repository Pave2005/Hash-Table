#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>
#include <string.h>

#include "list_opt.h"
#include "hash_table_opt.h"

void ListCtor (List* list, int size)
{
    list->list_elem = (ListNode*)aligned_alloc (ALIGNMENT, size * sizeof (ListNode)); // для выравнивания

    list->head = 0;
    list->tail = 0;

    list->list_elem[0].next = list->head;
    list->list_elem[0].prev = list->tail;

    list->list_elem[0].free_data = -8;
    list->list_elem[0].free_next = 1;
    list->head_of_free = 1;

    for (int i = 1; i < size; i++)
    {
        list->list_elem[i].next = NO_CONNECTION;
        list->list_elem[i].prev = NO_CONNECTION;

        list->list_elem[i].free_data = FREE_ELEM;
        if (i != size - 1)
        {
            list->list_elem[i].free_next = i + 1;
        }
    }
    list->list_elem[size - 1].free_next = 0;

    list->size = size;
}


void ListDtor (List* list)
{
    free (list->list_elem);
    list->head = 0;
    list->tail = 0;

    list->head_of_free = 0;
    free (list);
}


void UpdateListOfFree (List* list)
{
    list->list_elem[list->head_of_free].free_data = FILLED_ELEM;
    int pre_head_of_free = list->head_of_free;
    list->head_of_free = list->list_elem[list->head_of_free].free_next;
    list->list_elem[pre_head_of_free].free_next = NO_CONNECTION;
}

#ifndef ASM

void ListPush (List* list, Elem_list number)
{
    alignas(32) char tmp_buf[max_word_len] = {};

    strncpy(tmp_buf, number, strlen (number));
    list->list_elem[list->head_of_free].data = _mm256_load_si256 ((__m256i*)tmp_buf);

    list->list_elem[list->head_of_free].next = 0;
    list->list_elem[list->head_of_free].prev = list->tail;
    list->list_elem[list->tail].next = list->head_of_free;
    list->tail = list->head_of_free;

    list->list_elem[0].prev = list->tail;
    if (list->head == 0)
    {
        list->head = 1;
    }

    UpdateListOfFree (list);

    if (list->head_of_free == 0)
        ListRealloc (list);
}

#else

void ListPush (List* list, Elem_list number)
{
    alignas(32) char tmp_buf[max_word_len] = {};

    strncpy(tmp_buf, number, strlen (number));

    list->list_elem[list->head_of_free].data = tmp_buf;
    list->list_elem[list->head_of_free].next = 0;
    list->list_elem[list->head_of_free].prev = list->tail;
    list->list_elem[list->tail].next = list->head_of_free;
    list->tail = list->head_of_free;

    list->list_elem[0].prev = list->tail;
    if (list->head == 0)
    {
        list->head = 1;
    }

    UpdateListOfFree (list);

    if (list->head_of_free == 0)
        ListRealloc (list);
}

#endif

#ifndef ASM

void ListRealloc (List* list)
{
    int new_size = list->size * 2;

    ListNode* tmp_list = list->list_elem;

    //list->list_elem = (ListNode*)calloc (new_size, 256);
    list->list_elem = (ListNode*)aligned_alloc (ALIGNMENT, new_size * sizeof(ListNode));

    memcpy (list->list_elem, tmp_list, sizeof(ListNode) * list->size);

    for (int i = list->size; i < new_size; i++)
    {
        //list->list_elem[i].data = _mm256_setzero_si256 ();
        list->list_elem[i].next = NO_CONNECTION;
        list->list_elem[i].prev = NO_CONNECTION;

        list->list_elem[i].free_data = FREE_ELEM;
        if (i != new_size - 1)
            list->list_elem[i].free_next = i + 1;
    }
    list->list_elem[new_size - 1].free_next = 0;

    list->list_elem[list->size - 1].free_next = list->size;

    list->head_of_free = list->list_elem[list->size - 1].free_next;

    list->size = new_size;
    free (tmp_list);
}

#else

void ListRealloc (List* list)
{
    int new_size = list->size * 2;

    ListNode* tmp_list = list->list_elem;

    list->list_elem = (ListNode*)aligned_alloc (ALIGNMENT, new_size * sizeof(ListNode));

    memcpy (list->list_elem, tmp_list, sizeof(ListNode) * list->size);

    for (int i = list->size; i < new_size; i++)
    {
        list->list_elem[i].next = NO_CONNECTION;
        list->list_elem[i].prev = NO_CONNECTION;

        list->list_elem[i].free_data = FREE_ELEM;
        if (i != new_size - 1)
            list->list_elem[i].free_next = i + 1;
    }
    list->list_elem[new_size - 1].free_next = 0;

    list->list_elem[list->size - 1].free_next = list->size;

    list->head_of_free = list->list_elem[list->size - 1].free_next;

    list->size = new_size;
    free (tmp_list);
}

#endif
