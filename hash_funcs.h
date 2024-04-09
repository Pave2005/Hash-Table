#ifndef FUNCS
#define FUNCS

#include "hash_table.h"

hash_t HashRetZero     (const char* const element);
hash_t HashFirstLatter (const char* const element);
hash_t HashWordLen     (const char* const element);
hash_t HashControlSum  (const char* const element);
hash_t HashFifth       (const char* const element);
// hash_t HashCirShift    (const char* const element);
hash_t MurmurHash      (const char* const key);

#endif
