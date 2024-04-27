#include <stdio.h>
#include <string.h>

#include "hash_table.h"

hash_t HashRetZero (const char* const element)
{
    return 0;
}

hash_t HashFirstAscii (const char* const element)
{
    return (hash_t)element[0];
}

hash_t HashWordLen (const char* const element)
{
    return strlen (element);
}

hash_t HashAsciiSum (const char* const element)
{
    int sum = 0;

    for (int i = 0; element[i] != '\0'; i++)
        sum += (int)(element[i]);

    return sum;
}

hash_t HashOriginal (const char* const element)
{
    hash_t sum = 0;
    hash_t len = 0;

    for (int i = 0; element[i] != '\0'; i++)
    {
        sum += (hash_t)(element[i]);
        len++;
    }

    return sum / len;
}

// hash_t HashRorAscii (const char* const element)
// {
//     if (element[0] == '\0')
//         return 0;
//
//     int tmp = element[0];
//
//     for (int i = 1; element[i] != '\0'; i++)
//     {
//         __asm__
//         (
//             "mov eax, %1 ;\n"
//             "mov cl, 1   ;\n"
//
//             "ror ax, cl  ;\n"
//
//             "mov ebx, %2 ;\n"
//
//             "xor ax, bx  ;\n"
//
//             "mov %0, eax ;"
//
//             : "=m"(tmp)
//             : "g"(tmp), "g"((int)(element[i]))
//             : "ax", "bx", "cx"
//         );
//     }
//     return (hash_t)tmp;
// }

hash_t MurmurHash (const char* const key)
{
  const hash_t m = 0x5bd1e995;
  const hash_t seed = 0;
  const int r = 24;

  hash_t len = strlen (key);

  hash_t h = seed ^ len;

  const unsigned char* data = (const unsigned char*)key;
  hash_t k = 0;

  while (len >= 4)
  {
      k  = data[0];
      k |= data[1] << 8;
      k |= data[2] << 16;
      k |= data[3] << 24;

      k *= m;
      k ^= k >> r;
      k *= m;

      h *= m;
      h ^= k;

      data += 4;
      len -= 4;
  }

  switch (len)
  {
    case 3:
      h ^= data[2] << 16;
    case 2:
      h ^= data[1] << 8;
    case 1:
      h ^= data[0];
      h *= m;
  };

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}
