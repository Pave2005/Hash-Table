#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdalign.h>

#include <xmmintrin.h>
#include <x86intrin.h>

#include "hash_table.h"

int64_t strcmp_count = 0;

void InitHashTable (hash_table* const self, const int size, hash_t (*function_ptr)(const char* data))
{
    self->size = size;
    self->subset = (hash_subset*)calloc (size, sizeof (hash_subset));
    self->subset->size = 0;

    for (int i = 0; i < size; i++)
    {
        self->subset[i].subset_addr = (List*)calloc (1, sizeof (List));
        ListCtor (self->subset[i].subset_addr, init_list_size);
    }
    self->hash_function = function_ptr;
}


void AddElem (hash_table* const self, const char* const element, const int size, const int word_size)
{
    hash_t elem_index = self->hash_function(element) % self->size;
    int str_len = strlen (element);
    char* elem_data = (char*)calloc (str_len + 1, sizeof (char));
    strcpy (elem_data, element);

    if (HashTableSearch (self, element, str_len, nullptr, nullptr) == 0)
    {
        ListPush (self->subset[elem_index].subset_addr, elem_data);
        self->subset[elem_index].size++;
    }
}


int HashTableSearch (hash_table* const self, const char* const element, int elem_len, int* in_table_pos, int* in_list_pos)
{
    hash_t elem_index = self->hash_function(element) % self->size;
    //strcmp_count++;

    for (int i = self->subset[elem_index].subset_addr->head; i != 0; i = self->subset[elem_index].subset_addr->list_elem[i].next)
    {
        if (self->subset[elem_index].subset_addr->list_elem[i].data != nullptr)
        {
            if (strcmp (element, self->subset[elem_index].subset_addr->list_elem[i].data) == 0)
            {
                if (in_table_pos && in_list_pos)
                {
                    *in_table_pos = elem_index;
                    *in_list_pos  = i;
                }
                return 1;
            }
        }
    }
    return 0;
}


void FillTable (hash_table* const self, char* word_buf, long buf_size)
{
    for (int i = 0; i < buf_size;)
    {
        while (word_buf[i] == ' ' || word_buf[i] == '\n')
            i++;

        char element[max_word_len] = {};
        int elem_idx = 0;

        while (word_buf[i] != ' ' && word_buf[i] != '\n' && word_buf[i] != '\0')
        {
            if (elem_idx == max_word_len - 1)
                break;

            element[elem_idx++] = word_buf[i++];
        }
        if (word_buf[i] == '\0')
            break;

        AddElem (self, element, self->size, elem_idx);
    }
}


void HashTableDestr (hash_table* const self)
{
    for (int i = 0; i < self->size; i++)
        ListDtor (self->subset[i].subset_addr);

    free (self->subset);

    self->size = 0;
    self->subset->subset_addr = nullptr;
    self->hash_function = nullptr;
}


int HashTableExcelStat (hash_table* const self, const char* const file_name)
{
        FILE* excel_data = fopen(file_name, "w");

        for (int iter_count = 0; iter_count < self->size; ++iter_count)
            fprintf(excel_data, "%d, %d\n", iter_count, self->subset[iter_count].size);

        fclose(excel_data);

        return 0;
}
