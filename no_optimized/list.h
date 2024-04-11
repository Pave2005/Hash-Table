#ifndef LIST_H
#define LIST_H

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

struct ListNode
{
    Elem_list data;
    int next;
    int prev;

    int free_data;
    int free_next;
};

struct List
{
    int size;

    ListNode* list_elem;
    int head;
    int tail;

    int head_of_free;
};

void UpdateListOfFree (List* list);
void ListPush         (List* list, Elem_list num);
void ListDtor         (List* list);
void ListCtor         (List* list, int size);
void Delete           (List* list, int place);
void ListRealloc      (List* list);

#endif
