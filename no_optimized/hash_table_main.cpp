#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

//#include "hash_table_opt.h"

#include "hash_funcs.h"
#include "text_editor_funcs.h"

extern int64_t strcmp_count;
#define TEST_COUNT 100'000'000

int main ()
{
    FILE* text_file = fopen ("text_file.txt", "r");

    long file_size = FileSize (text_file);
    char* word_buf = ReadText (file_size, text_file);

    struct hash_table self;
    InitHashTable (&self, max_table_size, MurmurHash);

    FillTable (&self, word_buf, file_size);

    fclose (text_file);

    //HashTableExcelStat (&self, "exel.txt");

    int in_table_pos = 0;
    int in_list_pos  = 0;

    for (int i = 0; i < TEST_COUNT; i++)
        volatile int tmp = HashTableSearch (&self, "ask", 3, &in_table_pos, &in_list_pos);

    //printf("strcmp_count = %ld\n", strcmp_count);
    //printf ("in_table_pos = %d\nin_list_pos = %d\n", in_table_pos, in_list_pos);

    HashTableDestr (&self);
}
