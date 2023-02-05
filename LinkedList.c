/******************************************************************************
 * (c) 2018-2019 AED Team
 * Last modified: abl 2019-03-17
 *
 * NAME
 *      LinkedList.c
 *
 * DESCRIPTION
 *      Implementation of an abstract Linked List
 *
 * COMMENTS
 *      (Derived from 2018/10/24 lab4 code)
 *
 *  Implementation details:
 *      Linked list node: Each node of the list contains an item and a
 *      pointer to the next node.
 *      The item is specified in the file defs.h, expectably also a pointer.
 *
 *      Item deletion/freeing and comparison functions must be supplied
 *
 *****************************************************************************/

#include<stdio.h>
#include<stdlib.h>

#include "LinkedList.h"


void freeLinkedList(LinkedList *first)
{
    LinkedList *aux, *next;

    for(aux = first; aux != NULL; aux = next)
    {
        next = aux->next;           /* Keep track of the next node */
        free(aux);                  /* Free current node    */
    }
    return;
}

void freeLinkedListPointers(ptrLinkedList *first)
{
    ptrLinkedList *aux, *next;

    for(aux = first; aux != NULL; aux = next)
    {
        next = aux->next;           /* Keep track of the next node */
        free(aux);                  /* Free current node    */
    }
    return;
}


/*********************************************************************
 *  Function:
 *    lengthLinkedList
 *
 *  Description:
 *    Determines the length of a linked list.
 *
 *  Arguments:
 *    Pointer to the first node of the linked list:
 *        (LinkedList *) first
 *
 *  Return value:
 *    Returns the length of the linked list.
 **************************************************************************/
int lengthLinkedList(LinkedList *first)
{
    LinkedList *aux;
    int counter;

    for(aux = first, counter = 0;
        aux != NULL;
        counter++, aux = aux->next);

    return counter;
}

/***********************************************************************
 *  Function:
 *    getNextLinkedList
 *
 *  Description:
 *    Returns the next node of a linked list.
 *
 *  Arguments:
 *    Pointer to the current linked list node:
 *        (LinkedList *) node
 *
 *  Return value:
 *    Returns the pointer to the next node of a linked list. NULL
 *   is returned in case the current node is empty or there is no
 *   node following the current node.
 ************************************************************************/
LinkedList * getNextLinkedList(LinkedList * node)
{
  return ((node == NULL) ? NULL : node->next);
}

/**************************************************************************
 *  Function:
 *    getCostLinkedList
 *
 *  Description:
 *    Gets the cost of a linked list node.
 *
 *  Arguments:
 *    Pointer to a linked list node:
 *        (LinkedList *) node
 *
 *  Return value:
 *    Returns the cost value of the given node
 *************************************************************************************/
int getCostLinkedList(LinkedList *node)
{
    if(node == NULL)  /* Check that node is not empty */
        return -1;

    return node->cost;
}

/**************************************************************************
 *  Function:
 *    getCostLinkedList
 *
 *  Description:
 *    Gets the node id of a linked list node.
 *
 *  Arguments:
 *    Pointer to a linked list node:
 *        (LinkedList *) node
 *
 *  Return value:
 *    Returns the node id value of the given node
 *************************************************************************************/
int getNodeLinkedList(LinkedList *node)
{
    if(node == NULL)  /* Check that node is not empty */
        return -1;

    return node->node;
}

// Create a node
LinkedList* createNode(int v) 
{
    LinkedList* newNode = (LinkedList*)malloc(sizeof(LinkedList));
    newNode->node = v;
    newNode->next = NULL;
    return newNode;
}


// Add edge no topo da lista mas dps do listhead
void addEdge(LinkedList* graph, int d, int cost) {
    // Add edge from s to d
    LinkedList* newNode = createNode(d);
    newNode->cost = cost*cost;
    newNode->next = graph->next;
    graph->next = newNode;

}



int already_in_list(LinkedList** graph, int list_index, int node_to_find)
{
    LinkedList* t;

    for(t = graph[list_index]; t != NULL; t = getNextLinkedList(t))
    {
        if (t->node == node_to_find)
        {
            return 1;
        }
    }

    return 0;
}

void add_pointer_list(ptrLinkedList* pointers, int extend_to)
{
    ptrLinkedList* t = NULL;
    int cnt = -1;           //o primeiro no tem custo 0

    for (t = pointers; t != NULL; t = t->next)     //encontrar fim da lista
    {
        cnt++;      //custo maximo ate agr e igual ao numero de nos na lista de ponteiros sem contar com o primeiro

        if (t->next == NULL)        //se for a ulatima iteracao queremos guardar o t
        {
            break;
        }
    }

    for (int i = 0; i < extend_to - cnt; ++i)
    {
        t->next = (ptrLinkedList*)malloc(sizeof(ptrLinkedList));
        t->next->node = NULL;
        t->next->next = NULL;
        t = t->next;
    }
}

ptrLinkedList* get_pointer(ptrLinkedList* pointers, int dif_char)
{
    ptrLinkedList* t = NULL;
    int cnt = 0;           //o primeiro no tem custo 0

    for (t = pointers; cnt != dif_char; t = t->next)     //encontrar fim da lista
    {
        cnt++;      //numero do mutacoes do no em que estamos e igual ao numero de nos na lista de ponteiros sem contar com o primeiro

        if (t->next == NULL)        //se for a ulatima iteracao queremos guardar o t
        {
            break;
        }
    }

    return t;
}


