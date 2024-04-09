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
};

struct FreeElemsListNode
{
    int data;
    int next;
};

struct List
{
    int size;

    ListNode* list_elem;
    int head;
    int tail;

    // ????
    FreeElemsListNode* list_free_elems;
    int head_of_free;
};

void InsertAfter (List* list, int place_after, Elem_list number);
void ChangingListOfFree (List* list);
void DumpListNode(List* list_node);
void ListPush (List* list, Elem_list num);
void ListDtor (List* list);
void ListCtor (List* list, int size);
void ListDump (List* list);
void InsertBefore (List* list, int place_before, Elem_list number);
void Delete (List* list, int place);

#endif
