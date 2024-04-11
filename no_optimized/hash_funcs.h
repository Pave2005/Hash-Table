#ifndef FUNCS
#define FUNCS

#include "hash_table.h"

hash_t HashRetZero     (const char* const element);
hash_t HashFirstAscii  (const char* const element);
hash_t HashWordLen     (const char* const element);
hash_t HashAsciiSum    (const char* const element);
hash_t HashOriginal    (const char* const element);
hash_t HashRorAscii    (const char* const element);
hash_t MurmurHash      (const char* const key);

#endif
