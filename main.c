#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include "main_functions.h"

int main(int argc, char *argv[])
{
    init_info_struct* info = (init_info_struct*)malloc(sizeof(init_info_struct));
    info->dict_filename = NULL;
    info->pals_filename = NULL;
    info->out_filename = NULL;
    info->start = (char*)malloc(42*sizeof(char));
    info->target = (char*)malloc(42*sizeof(char));

    FILE* fp_out = NULL;
    FILE* fp_pals = NULL;
    FILE* fp_dict = NULL;

    info = process_init_input(info, argc, argv);

    ////////////////////////////////////////////////////////////////////////// abrir ficheiros
    if((fp_pals = fopen(info->pals_filename, "r")) == NULL)
    {
        free_info(info);
        return 0;
    }

    if((fp_dict = read_dict(info->dict_filename)) == NULL)
    {
        fclose(fp_pals);
        free_info(info);
        return 0;
    }

    fp_out = fopen(info->out_filename, "w");
    ////////////////////////////////////////////////////////////////////////////////////////////

    phase_one(info, fp_pals, fp_dict, fp_out);

    ////////////////////////////////////////////////////// fechar ficheiros
    if (fp_pals != NULL)
    {
        fclose(fp_pals);
    }   

    if (fp_dict != NULL)
    {
        fclose(fp_dict);
    }

    if (fp_out != NULL)
    {
        fclose(fp_out);
    }
    free_info(info);
    ////////////////////////////////////////////////////////////////////////
    return 0;
}

