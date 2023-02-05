/******************************************************************************
 * Last modified: 27-03-2022
 *
 * NAME
 *   basic_functions.h
 *
 * DESCRIPTION
 *   lista de funcoes auxiliares as funcoes principais
 *
 * COMMENTS
 *
 ******************************************************************************/

#ifndef __basic_functions__
#define __basic_functions__

////////////////////////    struct com informacao que vai ser precisa ao longo de todo o projeto
typedef struct init_info
{
    char* dict_filename;
    char* pals_filename;
    char* out_filename; 
    char* start;
    char* target;

}init_info_struct;

////////////////////////////////////////////////////////////////////////

char* split_string(char* source, char token);
int word_in_dict(char **dict, char* word, int dict_size);

init_info_struct* check_correct_input(char* dict_filename, char* pals_filename, init_info_struct* info);
init_info_struct* create_output_filename(init_info_struct* info);

FILE *read_dict(char* dict_filename);

int search(char* str,char** dict, int size);
int find_word(char* str, char** word, int dict_size);

int has_extension(char* file_to_check, char* ext_to_check);

void write_output(init_info_struct* info, int game_mode, int dict_size, char** word, FILE* fp_out);

char** sort(char** arr, int menor, int maior);
int partition(char** arr, int menor, int maior, char* pivot, char* aux_swap);
void swap(char* x, char* y, char* aux_swap);

int howmanydif(char* str1, char* str2);

void free_info(init_info_struct* info);

void print_path(int* wt, int* st, FILE* fp_out, char** word, int start, int target);

void find_biggest_graphs(int* vect, FILE* file);  

#endif    /* __basic_functions__ */
