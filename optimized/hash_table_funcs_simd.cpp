#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdalign.h>
#include <emmintrin.h>

#include "hash_table_opt.h"
#include "hash_funcs.h"

#define SUBSET_ADDR(number) self->subset[number].subset_addr
#define ASM

int64_t strcmp_count = 0;

#ifndef ASM

void InitHashTable (hash_table* const self, const int size, hash_t (*function_ptr)(const char* data))
{
    self->size = size;
    self->subset = (hash_subset*)aligned_alloc (ALIGNMENT, size * sizeof (hash_subset));     // выровнили по 32, для
                                                                // для того чтобы список начинался с кратного адреса.
    self->subset->size = 0;

    for (int i = 0; i < size; i++)
    {
        self->subset[i].subset_addr = (List*)aligned_alloc (ALIGNMENT, sizeof (List));
        ListCtor (self->subset[i].subset_addr, init_list_size);     // создали список, тоже выровнен.
    }
    self->hash_function = function_ptr;
}

int HashTableSearch (hash_table* const self, const char* const element, int elem_len,
                     int* in_table_pos,      int* in_list_pos)
{
    constexpr int max_word_len = 32;
    alignas(32) char tmp_buf[max_word_len] = {};

    hash_t elem_index = self->hash_function(element) % self->size;

    strncpy (tmp_buf, element, elem_len);

    __m256i cmp_buf = _mm256_load_si256 ((__m256i*)tmp_buf);

    for (int i = SUBSET_ADDR(elem_index)->head; i != 0; i = SUBSET_ADDR(elem_index)->list_elem[i].next)
    {
        __m256i mask = _mm256_cmpeq_epi8    (cmp_buf, SUBSET_ADDR(elem_index)->list_elem[i].data);
        int i_mask   = _mm256_movemask_epi8 (mask);

        if (i_mask == 0xFFFFFFFF)
        {
            if (in_table_pos && in_list_pos)
            {
                *in_table_pos = elem_index;
                *in_list_pos  = i;
            }
            return 1;
        }
    }
    return 0;
}

#endif
// изменил
void AddElem (hash_table* const self, const char* const element, const int size, const int word_size)
{
    hash_t elem_index = self->hash_function(element) % self->size;

    //char elem_data[32] = {}; // изначально делаем буфер размера 32.

    char* elem_data = (char*)aligned_alloc (ALIGNMENT, 32 * sizeof (char));
    //char* elem_data = (char*)calloc (str_len + 1, sizeof (char));

    strncpy (elem_data, element, word_size);

    if (HashTableSearch (self, element, word_size, nullptr, nullptr) == 0)
    {
        ListPush (SUBSET_ADDR(elem_index), elem_data);
        self->subset[elem_index].size++;
    }
}


void FillTable (hash_table* const self, char* word_buf, long buf_size)
{
    for (int i = 0; i < buf_size;)
    {
        if (word_buf[i] == '\0')
            break;

        while (word_buf[i] == ' ' || word_buf[i] == '\n' && i < buf_size)
            i++;

        char element[max_word_len] = {};            // временный массив для слова.
        int elem_idx = 0;

        while (word_buf[i] != ' ' && word_buf[i] != '\n' && word_buf[i] != '\0' && i < buf_size)
        {
            if (elem_idx == max_word_len - 1)
                break;

            element[elem_idx++] = word_buf[i++];
        }

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
            //fprintf(excel_data, "%d ", self->subset[iter_count].size);
            fprintf(excel_data, "%d, %d\n", iter_count, self->subset[iter_count].size);

        fclose(excel_data);

        return 0;
}


#ifdef ASM

// strlen много жрет, нужно это переделать и наверное сразу класть в таблицу элемент с его длиной
// выравниваниет на 32, и временные буферы длинной 32

extern "C" int strcmp_asm (const char* str1, const char* str2);
static inline int inline_asm_strcmp (const char * str1, const char * str2);
int strcmp_simd (const char* str1, const char* str2);

void InitHashTable (hash_table* const self, const int size, hash_t (*function_ptr)(const char* data))
{
    self->size = size;
    self->subset = (hash_subset*)aligned_alloc (ALIGNMENT, size * sizeof (hash_subset));
    //self->subset = (hash_subset*)calloc (size, sizeof (hash_subset));
    self->subset->size = 0;

    for (int i = 0; i < size; i++)
    {
        self->subset[i].subset_addr = (List*)aligned_alloc (ALIGNMENT, sizeof (List));
        //self->subset[i].subset_addr = (List*)calloc (1, sizeof (List));
        ListCtor (self->subset[i].subset_addr, init_list_size);
    }
    self->hash_function = function_ptr;
}

int HashTableSearch (hash_table* const self, const char* const element, int elem_len, int* in_table_pos, int* in_list_pos)
{
    hash_t elem_index = self->hash_function(element) % self->size;

    char tmp_element[32] = {};
    strncpy (tmp_element, element, elem_len);

    for (int i = SUBSET_ADDR(elem_index)->head; i != 0; i = SUBSET_ADDR(elem_index)->list_elem[i].next)
    {
        if (self->subset[elem_index].subset_addr->list_elem[i].data != nullptr)
        {
            // теперь при создании она создается на 32 символа.
            //char tmp_table_data[32] = {};
            //strncpy (tmp_table_data, SUBSET_ADDR(elem_index)->list_elem[i].data,
            //        strlen (SUBSET_ADDR(elem_index)->list_elem[i].data));

            if (strcmp (tmp_element, SUBSET_ADDR(elem_index)->list_elem[i].data) == 0)
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

int strcmp_simd (const char* str1, const char* str2)
{
    __m128i elem1 = _mm_load_si128 ((const __m128i*)str1);
    __m128i elem2 = _mm_load_si128 ((const __m128i*)str2);

    __m128i cmp  = _mm_cmpeq_epi8 (elem1, elem2);
    if (_mm_movemask_epi8 (cmp) == 0xFF)
        return 0;
    return 1;
}

#endif
