/******************************************************************************
 * Last modified: abl 04-10-2021
 *
 * NAME
 *   functions.h
 *
 * DESCRIPTION
 *   Function declarations for functions.c
 *
 * COMMENTS
 *
 ******************************************************************************/

#ifndef _functions__
#define __functions__

typedef struct pQueue
{
   int *vect;
   int free;
   int hsize;
}queue;

int PQEmpty(queue *pq);
queue* PQinit(unsigned Size);
void PQinsert(int Idx, queue *pq, int *wt, int* pQ_pos_arr);
int PQdelmax(queue* pq, int *wt, int* pQ_pos_arr);
void FixDown(int vec[], int Idx, int N, int *wt, int* pQ_pos_arr);
void FixUp (int vec[], int Idx, int *wt, int* pQ_pos_arr);
int lessPri(int a, int b, int *wt);
int find_index_in_queue(int vec[], int size, int index);

#endif    /* __functions__ */
