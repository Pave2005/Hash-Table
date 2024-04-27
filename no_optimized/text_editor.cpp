#include <stdio.h>
#include "text_editor_funcs.h"

int main ()
{
    FILE* text_file = fopen ("text_file.txt", "r");

    long file_size = FileSize (text_file);

    char* file_buf = ReadText (file_size, text_file);

    for (int i = 0; file_buf[i] != '\0'; i++)
    {
        if (file_buf[i] >= '!' && file_buf[i] <= '/' ||
            file_buf[i] >= ':' && file_buf[i] <= '@' ||
            file_buf[i] >= '[' && file_buf[i] <= '`' ||
            file_buf[i] >= '{' && file_buf[i] <= '~'  )
        {
            file_buf[i] = ' ';
        }
    }

    fclose (text_file);
    text_file = fopen ("text_file.txt", "w");

    fwrite (file_buf, sizeof (char), file_size, text_file);                     // записали все в исходный файл.

}
