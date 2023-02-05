/******************************************************************************
 * Last modified: 27-03-2022
 *
 * NAME
 *   basic_functions.c
 *
 * DESCRIPTION
 *   Program with the basic functions called by main_functions.c for wrdmttns
 *
 * COMMENTS
 *   Todo:  
 ******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "basic_functions.h"

#define INFINITY 2147483640

/******************************************************************************
 *  NAME
 *      check_correct_input: funcao que verifica a validade dos argumentos recebidos da linha de comando
 * 
 *  Arguments
 *      variaveis temporarias com informacao a ser validada
 * 
 *  Return
 *      info: ponteiro para struct com informacao validada
 *      
 ******************************************************************************/
init_info_struct* check_correct_input(char* dict_filename, char* pals_filename, init_info_struct* info)
{
    
    ///////////processar input sobre o dicionario
    if (has_extension(dict_filename,"dict"))
    {
        info->dict_filename = (char*)malloc(100*sizeof(char));
        strcpy(info->dict_filename, dict_filename);
    }
    else
    {
        free_info(info);
        return NULL;
    }
    //////////////////////////

    ///////////processar input sobre o ficcheiro das palavras
    if (has_extension(pals_filename,"pals"))
    {
        info->pals_filename = (char*)malloc(100*sizeof(char));
        strcpy(info->pals_filename, pals_filename);
    }
    else
    {
        free_info(info);
        return NULL;
    }
    //////////////////////////

    return info;
}

/******************************************************************************
 *  NAME
 *      write_output: funcao que escreve no ficheiro de output
 * 
 *  Arguments
 *      info: ponteiro para struct com informacao da linha de comando
 *      game_mode: inteiro em frente as 2 palavras
 *      word: tabela com palavras do mesmo tamanho ja ordenada
 *      
 ******************************************************************************/
void write_output(init_info_struct* info, int game_mode, int dict_size, char** word, FILE* fp_out)
{
    int spot = 0;
    int spot2 = 0;
    switch(game_mode)
    {
        case 1:
            fprintf(fp_out, "%s %d\n", info->start, dict_size);
            break;

        case 2:
            if ((spot = search(info->start, word, dict_size)) == -1 || (spot2 = search(info->target, word, dict_size)) == -1)
            {
                spot = word_in_dict(word, info->start, dict_size);
                fprintf(fp_out, "%s %d\n", info->start, spot);
                spot2 = word_in_dict(word, info->target, dict_size);
                fprintf(fp_out, "%s %d\n", info->target, spot2);
            }
            else
            {
                fprintf(fp_out, "%s %d\n", info->start, spot);
                fprintf(fp_out, "%s %d\n", info->target, spot2);
            }
            break;

        default:
            fprintf(fp_out, "%s %s %d\n", info->start, info->target, game_mode);    //copiar linha original
            break;
    }
    fprintf(fp_out, "\n");      //linha em branco entre problemas
}

/******************************************************************************
 *  NAME
 *      split_string: funcao que separa uma string em 2
 * 
 *  Arguments
 *      source: string a separar
 *      token: char onde separar a string
 * 
 *  Return
 *      str2: segunda metade da string
 * 
 *  Comments
 *      A primeira metade da string fica na source
 *      
 ******************************************************************************/
char* split_string(char* source, char token)
{
    char* str2 = (char*)malloc((strlen(source)+1)*sizeof(char));
    strcpy(str2, source);
    int start_split = 0;
    int token_pos = 0;
    for (int i = 1; i < strlen(source)+1; ++i)
    {
        if (source[i-1] == token)
        {
            start_split = 1;
            token_pos = i-1;
        }
        if (start_split)
        {
            str2[i - token_pos-1] = source[i];
        }
    }
    source[token_pos] = '\0';
    return str2;
}

/******************************************************************************
 *  NAME
 *      word_in_dict: funcao que verifica se uma palavra esta na lista de palavras validas
 * 
 *  Arguments
 *      dict: lista de palavras validas 
 *      word: palavra a testar
 *      dict_size: numero de palavras na lista dict
 * 
 *  Return 
 *      1 -> palavra valida
 *      0 -> palavra nao valida
 *      
 ******************************************************************************/
int word_in_dict(char **dict, char* word, int dict_size)
{
    for (int i = 0; i < dict_size; i++)
    {
        if (strcmp(word,dict[i])==0)
        {
            return i;
        }
    }
    return -1;
}

//-----------------------------------------------------------------------------------------------------------

/******************************************************************************
 *  NAME
 *      read_dict: funcao que abre e verifica a abretura do ficheiro do diconario
 * 
 *  Arguments
 *      dict_filename: nome do ficheiro a abrir
 * 
 *  Return 
 *      ptr_dict: ponteiro para o ficheiro aberto
 *      
 ******************************************************************************/
FILE *read_dict(char* dict_filename)
{
    FILE *ptrDict;
    if ((ptrDict=fopen(dict_filename,"r"))==NULL)
    {
        return NULL;
    }
    return ptrDict;  
}


/******************************************************************************
 *  NAME
 *      free_info: funcao que faz free da struct com informacao da linha de comando
 * 
 *  Arguments
 *      info: ponteiro para struct com informacao
 *      
 ******************************************************************************/
void free_info(init_info_struct* info)
{
    if (info->dict_filename != NULL)
    {
        free(info->dict_filename);
    }
    if (info->pals_filename != NULL)
    {
        free(info->pals_filename);
    }
    if (info->out_filename != NULL)
    {
        free(info->out_filename);
    }
    if (info->start != NULL)
    {
        free(info->start);
    }
    if (info->target != NULL)
    {
        free(info->target);
    }
    free(info);
}


/******************************************************************************
 *  NAME
 *      search: funcao que faz binary search de uma palavra num array de strings
 * 
 *  Arguments
 *      str: palavra a procurar
 *      dict: array de strings
 *      size: tamanho do array
 * 
 *  return:
 *      index da palavra no array
 *      
 ******************************************************************************/
int search(char* str,char** dict, int size)
    {
        int mid;    // index do midpoint
        char* midStr;  
        int first = 0;
        int last = size-1;
        while (first < last)
        // test for nonempty sublist
        {
            mid = (first + last) / 2;
            midStr = dict[mid];

            int comparison = strcmp(str, midStr);

            if (comparison == 0)
                return mid;// have a match
            else
            {
                if (comparison < 0)
                    last = mid; // search lower sublist. Reset last                    
                else
                    first = mid + 1; // search upper sublist. Reset first
            }
        }
        return -1; // target not found
    }

/***************************************************
 *  NAME
 *      has_extension: funcao que verifica se o nome dos ficheiros introduzidos está correto
 *
 *  Arguments
 *      file_to_check   : char* do nome do ficheiro
 *      ext_to_check    : extensao a verificar
 *
 *  Return
 *      0   : valido
 *      !=0 : invalido
 *
 ***************************************************/
int has_extension(char* file_to_check, char* ext_to_check) //n tenho a certeza se este file_to_check funciona
{
    char* temp_first = (char*)malloc(100 * sizeof(char));
    char* aux_str = NULL;

    strcpy(temp_first, file_to_check);
    aux_str = split_string(temp_first, '.'); //split e no temp_first fica a 1a metade
    free(temp_first);

    if (strcmp(aux_str, ext_to_check) != 0) //.ext nao e a esperada
    {
        free(aux_str);
        return 0;
    }
    else
    {
        free(aux_str);
        return 1;
    }

}

/***************************************************
 *  NAME
 *      create_output_filename: funcao que cria o nome do ficheiro de output
 * 
 *  Arguments
 *      info: ponteiro para struct com informacao da linha de comando
 * 
 *  Return
 *      info: informação atualizada inicializado
 *
 ***************************************************/

int find_word(char* str, char** word, int dict_size)
{
    int spot = 0;
    if ((spot = search(str, word, dict_size)) == -1)
    {
        if ((spot = word_in_dict(word, str, dict_size)) == -1)
        {
            return -1;
        }
        else
        {
            return spot;
        }
    }
    else
    {
        return spot;
    }
}

/***************************************************
 *  NAME
 *      create_output_filename: funcao que cria o nome do ficheiro de output
 * 
 *  Arguments
 *      info: ponteiro para struct com informacao da linha de comando
 * 
 *  Return
 *      info: informação atualizada inicializado
 *
 ***************************************************/

init_info_struct* create_output_filename(init_info_struct* info)
{
    char* new_filename = (char*)malloc(100 * sizeof(char));
    char* aux_str = NULL;
    strcpy(new_filename,info->pals_filename); //copies filename 

    aux_str = split_string(new_filename, '.'); //split e no new_filename fica a 1a metade
    strncat(new_filename, ".paths", 6); //acrescenta .paths



    info->out_filename = (char*)malloc(100 * sizeof(char));
    strcpy(info->out_filename, new_filename);
    free(new_filename);
    free(aux_str);
    return info;

}

/*************************************************************************
 *  NAME
 *      sort : quicksort propriamente dito
 *
 *  Arguments
 *      file, menor palavra e maior
 *
 *  Return
 *      void
 *
 *************************************************************************/
char** sort(char** arr, int menor, int maior)
{
    char* pivot = (char*)malloc(strlen(arr[0]) + 2);
    char* aux_swap = (char*)malloc(strlen(arr[0]) + 2);
    if (menor < maior)
    {

        int k = partition(arr, menor, maior, pivot, aux_swap);

        arr = sort(arr, menor, k - 1);

        arr = sort(arr, k + 1, maior);

    }
    free(pivot);
    free(aux_swap);

    return arr;
}

/*************************************************************************
 *  NAME
 *      partition : divide and conquer - quicksort
 *
 *  Arguments
 *      valor max, min e o
 *
 *  Return
 *      void
 *
 *************************************************************************/

int partition(char** arr, int menor, int maior, char* pivot, char* aux_swap)
{
    strcpy(pivot, arr[maior]);

    int i = (menor - 1);

    for (int j = menor; j <= maior - 1; j++)
    {
        if (strcmp(arr[j], pivot) <= 0)
        {
            i++;

            swap(arr[i], arr[j], aux_swap);
        }
    }

    swap(arr[i + 1], arr[maior], aux_swap);

    return (i + 1);

}


/*************************************************************************
 *  NAME
 *      swap : self explanatory, troca duas variaveis de posicao
 *
 *  Arguments
 *      variaveis que serao trocadas
 *
 *  Return
 *      void
 *
 *************************************************************************/

void swap(char* x, char* y, char* aux_swap)
{
    if (strcmp(x,y) != 0)
    {
        strcpy(aux_swap, x);

        strcpy(x, y);

        strcpy(y, aux_swap);
    }

}

/*************************************************************************
 *  NAME
 *      swap : self explanatory, faz print do caminho entre 2 palvras
 *
 *  Arguments
 *      variaveis que serao trocadas
 *
 *  Return
 *      void
 *
 *************************************************************************/
void print_path(int* wt, int* st, FILE* fp_out, char** word, int start, int target)
{
    int current_print = target;

    if (current_print != start)     //condicao de saida da recursividade
    {
        print_path(wt, st, fp_out, word, start, st[target]);
    }

    if (current_print != start)     //nao queremos imprimir o start, pq ja o escrevemos com o print_total_cost la em cima
    {
        fprintf(fp_out, "%s\n", word[current_print]);
    }
} 

/*************************************************************************
 *  NAME
 *      howmanydif : diz quantos chars são diferentes entre os argumentos
 *
 *  Arguments
 *      strings a testar
 *      modo de funcionamento
 *
 *  Return
 *      int: mode == -1 -> numero de chars diferentes
 *           mode != -1 -> se o numero de chars diferentes é igual ao mode (1 = True; 0 = False)
 *
 *************************************************************************/
int howmanydif(char* str1, char* str2)
{
    int return_value = 0;

    size_t len = strlen(str1);      //guardar length

    for (int i = 0; i < len; ++i)
    {
        if (str1[i] != str2[i])         //percorer letras
        {
            return_value ++;
        }
    }

    return return_value;
} 

void find_biggest_graphs(int* vect, FILE* file)
{
    char* aux1 = (char*)malloc(100);
    char* aux2 = (char*)malloc(100);
    int game_mode = 0;
    int dif_char = 0;
    size_t len1 = 0;
    size_t len2 = 0;

    while((fscanf(file, "%s %s %d", aux1, aux2, &game_mode) != EOF))
    {
        len1 = strlen(aux1);
        len2 = strlen(aux2);

        if (len1 != len2)
        {
            continue;
        }

        if (game_mode < 1)
        {
            continue;
        }

        dif_char = howmanydif(aux2, aux1);

        if (dif_char <= game_mode)
        {
            game_mode = dif_char - 1;
        }

        if (vect[(int)len1] < game_mode)
        {
            vect[(int)len1] = game_mode;
        }
    }

    fseek(file, 0, SEEK_SET);

    free(aux1);
    free(aux2);
}