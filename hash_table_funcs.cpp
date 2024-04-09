#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

void InitHashTable (hash_table* const self, const int size, hash_t (*function_ptr)(const char* data))
{
    self->size = size;
    self->subset_addr = (List**)calloc (size, sizeof (List*));
    for (int i = 0; i < size; i++)
    {
        self->subset_addr[i] = (List*)calloc (1, sizeof (List));
        ListCtor (self->subset_addr[i], 1000);                             // создали сами списки.
    }
    self->hash_function = function_ptr;
}


void AddElem (hash_table* const self, const char* const element, const int size)
{
    hash_t elem_index = self->hash_function(element) % self->size;

    char* elem_data = (char*)calloc (strlen (element) + 1, sizeof (char));

    strcpy (elem_data, element);

    if (HashTableSearch (self, element, nullptr, nullptr) == 0)
    {
        ListPush (self->subset_addr[elem_index], elem_data);
    }
}


int HashTableSearch (hash_table* const self, const char* const element, int* in_table_pos, int* in_list_pos)
{
    hash_t elem_index = self->hash_function(element) % self->size;

    for (int i = 0; i < self->subset_addr[elem_index]->size; i++)
    {
        if (self->subset_addr[elem_index]->list_elem[i].data != nullptr &&
            strcmp (element, self->subset_addr[elem_index]->list_elem[i].data) == 0)
        {
            if (in_table_pos && in_list_pos)
            {
                *in_table_pos = elem_index;
                *in_list_pos  = i;
            }

            return 1;
        }
    }
    return 0;                                       // если не нашли элемент.
}


void FillTable (hash_table* const self, char* word_buf)
{
    for (int i = 0; word_buf[i] != '\0';)
    {
        while (word_buf[i] == ' ' || word_buf[i] == '\n')
            i++;

        char element[max_word_len] = {};            // временный массив для слова.
        int elem_idx = 0;

        while (word_buf[i] != ' ' && word_buf[i] != '\n' && word_buf[i] != '\0')
        {
            if (elem_idx == max_word_len - 1)
            {
                printf ("The maximum length of the word has been exceeded!\n");
                break;
            }

            element[elem_idx++] = word_buf[i++];
        }

        if (word_buf[i] == '\0')
            break;

        AddElem (self, element, self->size);
    }
}


void HashTableDestr (hash_table* const self)
{
    for (int i = 0; i < self->size; i++)
        ListDtor (self->subset_addr[i]);

    free (self->subset_addr);

    self->size = 0;
    self->subset_addr   = nullptr;
    self->hash_function = nullptr;
}
