#ifndef TABLE
#define TABLE

#include "list.h"

typedef long long hash_t;

const int max_word_len   = 100;
const int max_table_size = 2000;
const int init_list_size = 50;

struct hash_subset
{
    int size;
    List* subset_addr;
};

struct hash_table
{
    int   size;
    hash_subset* subset;
    hash_t (*hash_function)(const char* data);
};

void InitHashTable     (hash_table* const self, const int size, hash_t (*function_ptr)(const char* data));
void AddElem           (hash_table* const self, const char* const element, const int size);
int  HashTableSearch   (hash_table* const self, const char* const element, int* in_table_pos, int* in_list_pos);
void HashTableDestr    (hash_table* const self);
void FillTable         (hash_table* const self, char* word_buf);
int HashTableExcelStat (hash_table* const self, const char* const file_name);

#endif
