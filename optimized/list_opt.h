#ifndef LIST_H
#define LIST_H

#define ASM

#include <smmintrin.h>
#include <sys/types.h>
#include <immintrin.h>

typedef char* Elem_list;

enum COMMAND
{
    LIST = 1,
    FREE = 0,
};

enum LIST_ERRORS
{
    INCORRECT_PLACE = 0,
};

enum LIST
{
    FREE_ELEM = 0,
    FILLED_ELEM = 1,

    NO_CONNECTION = -1,
};

#ifndef ASM

struct alignas(32) ListNode
{
    __m256i data;
    int next;
    int prev;

    int free_data;
    int free_next;
};

struct alignas(32) List
{
    int size;

    ListNode* list_elem;
    int head;
    int tail;

    int head_of_free;
};

#else

struct alignas(32) ListNode
{
    Elem_list data;
    int next;
    int prev;

    int free_data;
    int free_next;
};

struct alignas(32) List
{
    int size;

    ListNode* list_elem;
    int head;
    int tail;

    int head_of_free;
};

#endif

void UpdateListOfFree (List* list);
void ListPush         (List* list, Elem_list num);
void ListDtor         (List* list);
void ListCtor         (List* list, int size);
void Delete           (List* list, int place);
void ListRealloc      (List* list);

#endif
