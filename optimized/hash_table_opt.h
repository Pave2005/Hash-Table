#ifndef OPT_TABLE
#define OPT_TABLE

#include <smmintrin.h>
#include <sys/types.h>
#include <xmmintrin.h>
#include <x86intrin.h>
#include <immintrin.h>

#include "list_opt.h"

#define ALIGNMENT 32

typedef long long hash_t;

const int max_word_len   = 32;
const int max_table_size = 2000;
const int init_list_size = 50;

struct alignas(32) hash_subset
{
    int size;
    List* subset_addr;
};

struct alignas(32) hash_table
{
    int   size;
    hash_subset* subset;
    hash_t (*hash_function)(const char* data);
};

void InitHashTable      (hash_table* const self, const int size, hash_t (*function_ptr)(const char* data));
int  HashTableSearch    (hash_table* const self, const char* const element, int elem_len, int* in_table_pos, int* in_list_pos);
void AddElem            (hash_table* const self, const char* const element, const int size, const int word_size);
void FillTable          (hash_table* const self, char* word_buf, long buf_size);
int  HashTableExcelStat (hash_table* const self, const char* const file_name);
void HashTableDestr     (hash_table* const self);

#endif
