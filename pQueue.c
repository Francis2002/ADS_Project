/******************************************************************************
 * Projeto Intermedio - Davide Silva 99911 - Francisco Silva 99937
 * Last modified: 19-10-2021
 *
 * NAME
 *   functions.c
 *
 * DESCRIPTION
 *   Program with all the functions for Raiders of all Pyramids
 *
 * COMMENTS
 *   Todo: 
 ******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pQueue.h"

#define infinity 2147483647;

#define exch(A, B)  {int t = A; A = B; B = t;};



queue* PQinit(unsigned Size)
{
    queue* pq;

    pq=(queue*) malloc (sizeof(queue));
    if(pq==NULL){
        exit(0);
    }

    (pq->vect) = (int *)malloc(Size*sizeof(int));
    if(pq->vect==NULL){
        exit(0);
    }

    (pq->hsize) = Size; 
    (pq->free) = 0; 

    return pq;
}

void PQinsert(int Idx, queue* pq, int *wt, int* pQ_pos_arr) 
{
    if ((pq->free) < pq->hsize) {

        (pq->vect[pq->free]) = Idx;

        pQ_pos_arr[Idx] = pq->free;

        FixUp(pq->vect, pq->free, wt, pQ_pos_arr);

        pq->free += 1;
    }
}

int PQdelmax(queue* pq, int *wt, int* pQ_pos_arr)
{
    exch(pq->vect[0], pq->vect[(pq->free)-1]);
    exch(pQ_pos_arr[pq->vect[0]], pQ_pos_arr[pq->vect[(pq->free)-1]])
    FixDown(pq->vect, 0, pq->free-1, wt, pQ_pos_arr); 

    pq->free-=1;

    return pq->vect[pq->free]; 
}

int PQEmpty(queue* pq)
{
    if(pq->free==0) {
        return 1;
    }else{
        return 0;
    }
}

void FixUp(int vec[], int Idx, int *wt, int*  pQ_pos_arr)
{
    while(Idx>0 && lessPri(vec[(Idx-1)/2], vec[Idx], wt)){
        exch(vec[Idx], vec[(Idx-1)/2]);
        exch(pQ_pos_arr[vec[Idx]], pQ_pos_arr[vec[(Idx-1)/2]]);
        Idx=(Idx-1)/2;
    }
}

void FixDown(int vec[], int Idx, int N, int *wt, int* pQ_pos_arr)
{
    int Child;

    while(2*Idx < N-1) { 
        Child = 2*Idx+1;

        if (Child < (N-1)  &&  lessPri(vec[Child], vec[Child+1], wt)) Child++;
        if (!lessPri(vec[Idx], vec[Child], wt)) break;

        exch(vec[Idx], vec[Child]);
        exch(pQ_pos_arr[vec[Idx]], pQ_pos_arr[vec[Child]]);
        Idx = Child;
    }
}

int lessPri(int a, int b, int wt[])
{
    if(wt[a]>wt[b]){
        return 1;
    }else{
        return 0;
    }

}

int find_index_in_queue(int vec[], int size, int index)
{
    for (int i = 0; i < size; ++i)
    {
        if (vec[i] == index)
        {
            return i;
        }
    }

    return -1;
}