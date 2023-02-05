/******************************************************************************
 * Last modified: 25-04-2022
 *
 * NAME
 *   main_functions.h
 *
 * DESCRIPTION
 *   lista de funcoes principais para a implementacao do jogo
 *
 * COMMENTS
 *
 ******************************************************************************/

#ifndef __main_functions__
#define __main_functions__
 
#include "LinkedList.h"
#include "pQueue.h"
#include "basic_functions.h"

init_info_struct* process_init_input(init_info_struct* info, int argc, char *argv[]);

void phase_one(init_info_struct* info, FILE* fp_pals, FILE* fp_dict, FILE* fp_out);

void phase_two(init_info_struct* info, int game_mode, int dict_size, char** word, FILE* fp_out, LinkedList** G, int gm_shortened, int* graph_max_cost, size_t len_start, ptrLinkedList** pointers, int file_biggest_path);

void Dijkstra(init_info_struct* info, int game_mode, int* wt, int* st, int dict_size, LinkedList** G, char** word, int start, int target, ptrLinkedList** pointers, int* pQ_pos_arr);

LinkedList** get_graph(char** word, int dict_size, int game_mode, LinkedList** graph, int* graph_max_mut, size_t len_start, ptrLinkedList** pointers);

char*** get_dict(FILE *ptrDict, int* size, init_info_struct* info);

LinkedList*** createPlaca(int* dict_size);

ptrLinkedList*** createPlacaPointers(int* dict_size);

#endif    /* __main_functions__ */
