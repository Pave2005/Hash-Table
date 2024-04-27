#ifndef FUNCS
#define FUNCS

typedef long long hash_t;

hash_t FastHash        (const char* const element, int word_size);

hash_t HashRetZero     (const char* const element);
hash_t HashFirstAscii  (const char* const element);
hash_t HashWordLen     (const char* const element);
hash_t HashAsciiSum    (const char* const element);
hash_t HashOriginal    (const char* const element);
hash_t HashRorAscii    (const char* const element);
hash_t MurmurHash      (const char* const key);

#endif
