/******************************************************************************
 *
 * NAME
 *   main_functions.c
 *
 * DESCRIPTION
 *   Programa com o codigo das funcoes principais para a implementacao do jogo
 *
 * COMMENTS
 *   Todo:  
 * 
 ******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "main_functions.h"

#define MAX_SIZE 100

#define infinity 2147483640

/******************************************************************************
 * 
 * 
 * NAME
 *      process_init_input: funcao que recebe os argumentos da linha de comando
 * 
 *  Arguments
 *      info: ponteiro para struct onde a informacao vai ser guardada
 *      argc
 *      argv
 * 
 *  Return
 *      info: ponteiro para struct ja com informacao
 *         
 ******************************************************************************/
init_info_struct* process_init_input(init_info_struct* info, int argc, char *argv[])
{
    char* pals_filename = NULL;
    char* dict_filename = NULL;

    //ver se tem argumentos a mais ou a menos
    if (argc != 3)
    {
        free_info(info);
        exit(0);
    }

    //atribuir com ordem certa
    dict_filename = argv[1];
    pals_filename = argv[2];

    //chama funcao que valida argumentos lidos e os insere na struct info
    info = check_correct_input(dict_filename, pals_filename, info);

    //info é NULL se um dos ficheiros estiver mal
    //qd é assim já está freed
    if (info == NULL)
    {
        exit(0);
    }

    info = create_output_filename(info);

    return info;

}

/******************************************************************************
 * 
 *  NAME
 *      phase_one: funcao que resolve a primeira fase e prapara a resolucao da segunda fase
 * 
 *  Arguments
 *      info: ponteiro para struct com informacao
 *      ponteiros para os ficheiros ja abertos
 *         
 ******************************************************************************/
void phase_one(init_info_struct* info, FILE* fp_pals, FILE* fp_dict, FILE* fp_out)
{
    int game_mode = 0;
    char*** word = NULL;
    ptrLinkedList*** pointers = NULL;     
    int* graph_max_mut = (int*)calloc(101, sizeof(int));       //array com custo maximo das ligacoes de cada grafo
    int* dict_size = (int*)calloc(101, sizeof(int));            //array com numero de palavras de cada tamanho
    char *aux_str = (char*)malloc(42*sizeof(char));
    size_t len_start = 0;
    int gm_shortened = 0;       //flag que diz se o numero de letras para trocar e >= ao tamanho (e por isso ha sempre solucao)
    
    LinkedList*** graphs = NULL;

    word = get_dict(fp_dict, dict_size, info);

    graphs = createPlaca(dict_size);            //placas sao as alocacoes iniciais das linked lists

    pointers = createPlacaPointers(dict_size);

    int* file_biggest_graph = (int*)calloc(101, sizeof(int));            //array com modo de jogo maximo de cada tamanho

    find_biggest_graphs(file_biggest_graph, fp_pals);

    while((fscanf(fp_pals, "%s %s %d", info->start, info->target, &game_mode) != EOF))
    {
        gm_shortened = 0;

        len_start = strlen(info->start);        //para nao ter de andar sempre a fzr strcmp

        if (game_mode >= (int)len_start)        //se o numero de mutacoes e maior que o numero de letras da palavra, reduzimos o numero de mutacoes para o tamanho da palavra
        {
            game_mode = (int)len_start;
            gm_shortened = 1;
        }

        if (game_mode >= howmanydif(info->start, info->target))
        {
            game_mode = howmanydif(info->start, info->target);
            gm_shortened = 1;
        }

        if (len_start != strlen(info->target))      //problema mal definido
        {
            fprintf(fp_out, "%s -1\n", info->start);
            fprintf(fp_out, "%s\n", info->target);
        }
        else if (game_mode < 0)
        {
            fprintf(fp_out, "%s -1\n", info->start);
            fprintf(fp_out, "%s\n", info->target);
        }
        else
        {
            if (search(info->start, word[len_start], dict_size[len_start]) == -1 || search(info->target, word[len_start], dict_size[len_start]) == -1)
            {
                //fail safe do search
                if (word_in_dict(word[len_start], info->start, dict_size[len_start]) == -1 || word_in_dict(word[len_start], info->target, dict_size[len_start]) == -1)
                {
                    fprintf(fp_out, "%s -1\n", info->start);
                    fprintf(fp_out, "%s\n", info->target);
                }
                else if (strcmp(info->start, info->target) == 0)        //resolucao imediata
                {
                    fprintf(fp_out, "%s 0\n", info->start);
                    fprintf(fp_out, "%s\n", info->target);
                }
                else
                {
                    phase_two(info, game_mode, dict_size[len_start], word[len_start], fp_out, graphs[len_start], gm_shortened, graph_max_mut, len_start, pointers[len_start], file_biggest_graph[len_start]);
                }
            }
            else if (strcmp(info->start, info->target) == 0)        //resolucao imediata
            {
                fprintf(fp_out, "%s 0\n", info->start);
                fprintf(fp_out, "%s\n", info->target);
            }
            else
            {
                phase_two(info, game_mode, dict_size[len_start], word[len_start], fp_out, graphs[len_start], gm_shortened, graph_max_mut, len_start, pointers[len_start], file_biggest_graph[len_start]);
            }
        }    
        fprintf(fp_out, "\n"); 
            
    }

    for (int i = 0; i <= 100; ++i)
    {
        for (int j = 0; j < dict_size[i]; ++j)
        {
            if (dict_size[i] != 0)
            {
                free(word[i][j]);
                freeLinkedList(graphs[i][j]);
                freeLinkedListPointers(pointers[i][j]);
            }
        }
        free(word[i]);
        free(graphs[i]);
        free(pointers[i]);
    }
    free(word);
    free(graphs);
    free(pointers);

    free(file_biggest_graph);
    free(graph_max_mut);

    free(dict_size);
    free(aux_str);
}

/******************************************************************************
 * 
 *  NAME
 *      phase_two: funcao que resolve a segunda fase
 * 
 *  Arguments
 *      info: ponteiro para struct com informacao
 *      G: grafo do tamanho certo
 *      word: dicionario do tamanho certo
 *      ponteiros para os ficheiros ja abertos
 *      variaveis auxiliares 
 *         
 ******************************************************************************/

void phase_two(init_info_struct* info, int game_mode, int dict_size, char** word, FILE* fp_out, LinkedList** G, int gm_shortened, int* graph_max_cost, size_t len_start, ptrLinkedList** pointers, int file_biggest_graph)
{
    int* pQ_pos_arr = (int*)malloc(dict_size*sizeof(int));
    int* wt = (int*)malloc(sizeof(int)*(dict_size+1));          //vetor com custo total para chegar a palavra de cada indice
    int* st = (int*)malloc(sizeof(int)*(dict_size+1));          //vetor que indica qual e o vertice de onde vem qd chega a palavra de cada indice

    int print_total_cost = 0;

    int start = find_word(info->start, word, dict_size);        //id do vertice inicial (ou seja posicao no dicionario do seu tamanho ordenado)
    int target = find_word(info->target, word, dict_size);      //id do vertice final (ou seja posicao no dicionario do seu tamanho ordenado)

    if (gm_shortened)       //se o numero de mutacoes e igual ao numero de letras diferentes entre start e target ou ao game_mode, ent nao vale a pena 
    {                       //fazer as ligacoes de maior custo porque se o algoritmo disser que nao ha solucao, ent o custo é o de trocar todas as letras
        game_mode -= 1;

        if (game_mode == 0)             //resolucao imediata
        {
            fprintf(fp_out, "%s 1\n", info->start);
            fprintf(fp_out, "%s\n", info->target);
            free(pQ_pos_arr);
            free(wt);
            free(st);
            return;
        }
    }

    else if (game_mode == 0)             //resolucao imediata, so tem solucao se palavras forem iguais
    {
        fprintf(fp_out, "%s -1\n", info->start);
        fprintf(fp_out, "%s\n", info->target);
        free(pQ_pos_arr);
        free(wt);
        free(st);
        return;
    }

    G = get_graph(word, dict_size, file_biggest_graph, G, graph_max_cost, len_start, pointers); //constroi o grafo desse tamanho com o numero maximo de mutacoes possiveis

    Dijkstra(info, game_mode, wt, st, dict_size, G, word, start, target, pointers, pQ_pos_arr);      //calcula wt e st

    print_total_cost = wt[target];    //vai buscar o wt do target

    if (print_total_cost == infinity)       //se o target nao foi encontrado nao ha solucao
    {
        if (gm_shortened)       //aqui sabemos que ha sempre solucao e o custo e maximo
        {
            fprintf(fp_out, "%s %d\n", info->start, (game_mode + 1)*(game_mode + 1));
            fprintf(fp_out, "%s\n", info->target);
        }
        else
        {
            fprintf(fp_out, "%s -1\n", info->start);
            fprintf(fp_out, "%s\n", info->target);
        }
    }
    else
    {
        if ((print_total_cost > (game_mode + 1)*(game_mode + 1)) && (gm_shortened)) 
        {
            fprintf(fp_out, "%s %d\n", info->start, (game_mode + 1)*(game_mode + 1));
            fprintf(fp_out, "%s\n", info->target);
        }
        else
        {
            fprintf(fp_out, "%s %d\n", info->start, print_total_cost);
            print_path(wt, st, fp_out, word, start, target);
        }     
    }

    free(pQ_pos_arr);
    free(wt);
    free(st);
    
}

/******************************************************************************
 * 
 *  NAME
 *      Dijkstra: funcao que calcula os vetores st e wt do algoritmo dijskstra
 * 
 *  Arguments
 *      info: ponteiro para struct com informacao
 *      ponteiros para os ficheiros ja abertos
 *      wt e st ja alocados
 *      variaveis auxiliares
 *         
 ******************************************************************************/
void Dijkstra(init_info_struct* info, int game_mode, int* wt, int* st, int dict_size, LinkedList** G, char** word, int start, int target, ptrLinkedList** pointers, int* pQ_pos_arr)
{
    for (int i = 0; i < dict_size + 1; ++i)
    {
        wt[i] = infinity;       //custo para chegar a qq vertice noinicio e infinito
        st[i] = infinity;       //origem dos caminhos tb infinito pq nao pode ser 0, ja que ha uma palavra no indice zero
    }

    queue* pQ = PQinit(dict_size);      

    PQinsert(start, pQ, wt, pQ_pos_arr);        //por palavra de origem na queue

    for (int i = 0; i < dict_size; ++i)
    {
        if (i == start)
        {
            continue;
        }
        PQinsert(i, pQ, wt, pQ_pos_arr);
    }

    wt[start] = 0;      //custo zero para chegar a si propria de si propria
    st[start] = start;      //origem igual a si propria

    int v = start;
    int aux_cost, aux_node;
    LinkedList* t;
    ptrLinkedList* p;

    while(!PQEmpty(pQ))
    {
        v = PQdelmax(pQ, wt, pQ_pos_arr);           //tirar da queue o vertice com mais prioridade, ou seja o que vamos analisar

        if (v == target)        //podemos parar qd dermos pop do target
        {
            break;
        }

        if (wt[v] != infinity)         //se v ainda nao foi visitado
        {
            for (int j = 1; j <= game_mode; ++j)    //percorrer os pointers de custo 
            {
                p = get_pointer(pointers[v], j);

                if (p->node == NULL)
                {
                    continue;
                }

                for(t = p->node; t != NULL; t = getNextLinkedList(t))      //percorrer os vertices adjacentes ao que estamos a analisar com custo igual a j^2
                {

                    if (t->cost != j*j)
                    {
                        break;
                    }

                    aux_cost = getCostLinkedList(t);
                    aux_node = getNodeLinkedList(t);       

                    if (wt[aux_node] > (wt[v] + aux_cost))      //se este caminho é o mais curto para chegar a t
                    {
                        wt[aux_node] = wt[v] + aux_cost;        //o novo custo passa a ser este
                        st[aux_node] = v;                       //o novo source passa a ser v
                        FixUp(pQ->vect, pQ_pos_arr[aux_node], wt, pQ_pos_arr);
                    }                     
                }
            }
        }
    }

    free(pQ->vect);
    free(pQ);
}

/******************************************************************************
 *  NAME
 *      get_dict: funcao que passa as palavras de um dicionario que tem uma palavra por linha para um array
 *                         e ordena por tamanho
 *  Arguments
 *      ptrDict: ponteiro para o dicionario
 *      size: variavel usada como segundo return; representa numero de palavras passadas para a lista
 *      info
 * 
 *  Return 
 *      word: lista das palvras validas
 * 
 *  Variaveis declaradas
 *      aux_word: string usada para receber linhas do ficheiro
 *      verify: flag que indica a validade de cada palavra
 *      count: numero de palavra postas na lista (retornado atraves do parametro size)
 *      
 ******************************************************************************/
char*** get_dict(FILE *ptrDict, int* size, init_info_struct* info)
{
    char ***word = NULL;
    char *aux_word = (char*)malloc(42*sizeof(char));
    int* aux_arr = (int*)calloc(101, sizeof(int));
    int verify = 0;
    size_t len_aux_word = 0;
    

    word=(char***)malloc(101*sizeof(char**));
    word[0] = NULL;

    fseek(ptrDict, 0, SEEK_SET);

    while (fscanf(ptrDict, "%s ", aux_word) != EOF) 
    {
        size[strlen(aux_word)] += 1;        //para descobrir qts palavras ha de cada tamanho no ficheiro
    }

    for (int i = 0; i <= 100; ++i)
    {
        word[i] = (char**)malloc((size[i])*sizeof(char*)); //allocar numero de palavras de cada tamanho
    }

    fseek(ptrDict, 0, SEEK_SET);
    while (fscanf(ptrDict, "%s ", aux_word) != EOF) 
    {
        len_aux_word = strlen(aux_word);
        for (int j = 0; j < len_aux_word; j++)//exclusão de palavras que não têm apenas minúsculas
        {
            if (aux_word[j]>='a' && aux_word[j]<='z')
            { 
                verify = 1;
            }
            else
            {
                verify = 0;
                break;
            }
            
        }
        if (verify==1)
        {
            word[len_aux_word][aux_arr[len_aux_word]]=(char*)malloc(sizeof(char)*(len_aux_word + 1));
            strcpy(word[len_aux_word][aux_arr[len_aux_word]], aux_word);
            aux_arr[len_aux_word] += 1;
        }
    }   

    for (int i = 0; i <= 100; ++i)
    {
        if (size[i] > 1)
        {
            word[i] = sort(word[i], 0, size[i] - 1);
        }
    }
    
    free(aux_arr);
    free(aux_word);
    return word;
}

/******************************************************************************
 *  NAME
 *      get_graph: funcao que cria grafos
 *  Arguments
 *      word: dicionario
 *      graphs: ponteiro para os grafos
 *      size: variavel usada como segundo return; representa numero de palavras passadas para a lista
 *      info
 * 
 *  Return 
 *      word: lista das palvras validas
 * 
 *  Variaveis declaradas
 *      aux_word: string usada para receber linhas do ficheiro
 *      verify: flag que indica a validade de cada palavra
 *      count: numero de palavra postas na lista (retornado atraves do parametro size)
 *      
 ******************************************************************************/
LinkedList** get_graph(char** word, int dict_size, int game_mode, LinkedList** graph, int* graph_max_mut, size_t len_start, ptrLinkedList** pointers)
{
    int dif_char = 0;
    ptrLinkedList* ptr = NULL;

    if (game_mode <= graph_max_mut[len_start])     //se o grafo ja tiver estas ligacoes segue em frente
    {
        return graph;
    }

    for (int i = 0; i < dict_size; i++)
    {
        add_pointer_list(pointers[i], game_mode);          //extends list of poinetrs to save pointers to new costs
        for (int j = 0; j < dict_size; j++)
        {
            if (i == j)
            {
                continue;
            }

            dif_char = howmanydif(word[i], word[j]);

            if (dif_char > game_mode)        //se o numero de chars diferentes for maior que o numero de mutacoes pedido seguimos em frente
            {
                continue;
            }

            if (dif_char <= graph_max_mut[len_start])           //se a ligacao ja esta na lista
            {       
                continue;
            }

            ptr = get_pointer(pointers[i], dif_char);          //encontra pointer onde comeca a lista deste custo

            if (ptr->node == NULL)
            {
                addEdge(graph[i], j, dif_char);         //se for a primeira vez que este custo aparece na linkedlist
                ptr->node = graph[i]->next;             //porno inicio da lista e guardar o inicio da lista desse custo no pointers
            }
            else
            {
                addEdge(ptr->node, j, dif_char);
            }
        }
    }

    graph_max_mut[len_start] = game_mode;

    return graph;
}
    

//Cria os listheads todos

LinkedList*** createPlaca(int* dict_size) //create placa
{
    LinkedList*** graphs = (LinkedList***)malloc(101*sizeof(LinkedList**));     //alloca 101 arrays de ponteiros, um por cada tamanho

    for (int i = 0; i <= 100; ++i)
    {
        graphs[i] = (LinkedList**)malloc(dict_size[i]*sizeof(LinkedList*));     //para cada tamanho alloca um no por palavra desse tamanho
    }

    for (int i = 0; i <= 100; i++)
    {
        for (int j = 0; j < dict_size[i]; ++j)
        {
            graphs[i][j] = createNode(j);     //isto sera o indice da palavra no dict
            graphs[i][j]->cost = 0;
        }
    }

    return graphs;
}

//Cria os listheads todos

ptrLinkedList*** createPlacaPointers(int* dict_size) //create placa
{
    ptrLinkedList*** pointers = (ptrLinkedList***)malloc(101*sizeof(ptrLinkedList**));     //alloca 101 arrays de ponteiros, um por cada tamanho

    for (int i = 0; i <= 100; ++i)
    {
        pointers[i] = (ptrLinkedList**)malloc(dict_size[i]*sizeof(ptrLinkedList*));     //para cada tamanho alloca um no por palavra desse tamanho
    }

    for (int i = 0; i <= 100; i++)
    {
        for (int j = 0; j < dict_size[i]; ++j)
        {
            pointers[i][j] = (ptrLinkedList*)malloc(sizeof(ptrLinkedList));
            pointers[i][j]->node = NULL;
            pointers[i][j]->next = NULL;
        }
    }

    return pointers;
}


    