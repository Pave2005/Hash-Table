#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdalign.h>

#include <xmmintrin.h>
#include <x86intrin.h>

#include "hash_table.h"
#include "hash_funcs.h"

#define SIMD

extern "C" int strcmp_asm (const char* str1, const char* str2);
inline int inline_strcmp_asm (const char* str1, const char* str2);
int strcmp_simd (const char* str1, const char* str2);

int64_t strcmp_count = 0;

void InitHashTable (hash_table* const self, const int size, hash_t (*function_ptr)(const char* data))
{
    self->size = size;
    #ifdef SIMD
    self->subset = (hash_subset*)aligned_alloc (ALIGNMENT, size * sizeof (hash_subset));
    #else
    self->subset = (hash_subset*)calloc (size, sizeof (hash_subset));
    #endif
    self->subset->size = 0;

    for (int i = 0; i < size; i++)
    {
        #ifdef SIMD
        self->subset[i].subset_addr = (List*)aligned_alloc (ALIGNMENT, sizeof (List));
        #else
        self->subset[i].subset_addr = (List*)calloc (1, sizeof (List));
        #endif
        ListCtor (self->subset[i].subset_addr, init_list_size);
    }
    self->hash_function = function_ptr;
}


void AddElem (hash_table* const self, const char* const element, const int size, const int word_size)
{
    hash_t elem_index = FastHash (element, word_size) % self->size;
    #ifdef SIMD
    char* elem_data = (char*)aligned_alloc (ALIGNMENT, 32);
    #else
    char* elem_data = (char*)calloc (16, 1);
    #endif
    memcpy (elem_data, element, word_size);

    if (HashTableSearch (self, element, word_size, nullptr, nullptr) == 0)
    {
        ListPush (self->subset[elem_index].subset_addr, elem_data);
        self->subset[elem_index].size++;
    }
}


int HashTableSearch (hash_table* const self, const char* const element, int elem_len, int* in_table_pos, int* in_list_pos)
{
    hash_t elem_index = FastHash (element, elem_len) % self->size;
    //strcmp_count++;
    #ifdef SIMD
    alignas(32) char tmp_element[32] = {};
    #else
    char tmp_element[16] = {};
    #endif
    memcpy (tmp_element, element, elem_len);

    for (int i = self->subset[elem_index].subset_addr->head; i != 0; i = self->subset[elem_index].subset_addr->list_elem[i].next)
    {
        if (self->subset[elem_index].subset_addr->list_elem[i].data != nullptr)
        {
            #ifdef SIMD
            if (strcmp_simd (tmp_element, self->subset[elem_index].subset_addr->list_elem[i].data) == 0)
            #else
            if (strcmp_asm (tmp_element, self->subset[elem_index].subset_addr->list_elem[i].data) == 0)
            #endif
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

inline int inline_strcmp_asm (const char * str1, const char * str2)
{
    int res = 0;
    asm
    (
        "vmovdqu   xmm1, [rdi]         ;\n"
        "vpcmpeqd  xmm0, xmm1, [rsi]   ;\n"
        "vmovmskps eax, xmm0           ;\n"
        "xor eax, 0xFF                 ;\n"

        : "=r" (res)
        :
        : "xmm1", "xmm0", "cc"
    );

    return res;
}

int strcmp_simd (const char* str1, const char* str2)
{
    __m256i str1_buf = _mm256_load_si256 ((__m256i*)str1);
    __m256i str2_buf = _mm256_load_si256 ((__m256i*)str2);

    __m256i mask = _mm256_cmpeq_epi8 (str1_buf, str2_buf);
    int i_mask   = _mm256_movemask_epi8 (mask);

    if (i_mask == 0xFFFFFFFF)
        return 0;
    return 1;
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
