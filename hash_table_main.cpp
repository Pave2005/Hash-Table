#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "hash_funcs.h"
#include "text_editor_funcs.h"

int main ()
{
    FILE* text_file = fopen ("text_file.txt", "r");

    long file_size = FileSize (text_file);                                  // функции определены в text_editor.
    char* word_buf = ReadText (file_size, text_file);

    struct hash_table self;
    InitHashTable (&self, max_table_size, MurmurHash);

    FillTable (&self, word_buf);

    fclose (text_file);

    int in_list_pos = 0;
    int in_table_pos = 0;

    HashTableDestr (&self);
}
