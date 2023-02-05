/******************************************************************************
 * (c) 2018-2019 AED Team
 * Last modified: abl 2019-03-17
 *
 * NAME
 *   LinkedList.h
 *
 * DESCRIPTION
 *  Header file for an abstract implementation of a Linked List
 *
 * COMMENTS
 *  (Derived from 2018/10/24 lab4 code)
 *
 *  Data type list:
 *    Linked list node: LinkedList
 *
 *  Function list:
 *    A) Initialization & Termination
 *        initLinkedList
 *        freeLinkedList
 *
 *    B) Properties
 *        lengthLinkedList
 *
 *    C) Navigation
 *        getNextNodeLinkedList
 *
 *    D) Lookup
 *        getItemLinkedList
 *
 *    E) Insertion - simple at head, sorted
 *       insertUnsortedLinkedList
 *       insertSortedLinkedList
 *
 *  Dependencies:
 *    stdio.h
 *    stdlib.h
 *    defs.h
 *
 *****************************************************************************/


/* Prevent multiple inclusions */
#ifndef LinkedListHeader
#define LinkedListHeader

typedef struct LinkedListStruct
{
    int node;
    int cost;
    struct LinkedListStruct *next;
}LinkedList;

typedef struct pointersLinkedList
{
    struct LinkedListStruct* node;
    struct pointersLinkedList *next;
}ptrLinkedList;


void freeLinkedList(LinkedList * first);

void freeLinkedListPointers(ptrLinkedList *first);

int lengthLinkedList(LinkedList * first);

LinkedList * getNextLinkedList(LinkedList * node);

int getCostLinkedList(LinkedList * node);

int getNodeLinkedList(LinkedList * node);

LinkedList* createNode(int v);

void addEdge(LinkedList* graph, int d, int cost);

int already_in_list(LinkedList** graph, int list_index, int node_to_find);

void add_pointer_list(ptrLinkedList* pointers, int extend_to);

ptrLinkedList* get_pointer(ptrLinkedList* pointers, int dif_char);

#endif
