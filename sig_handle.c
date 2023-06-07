#include "headers.h"

extern int *process_id_list;
extern char **process_name_list;
extern int bg_process_count;
extern char *og_wd;
extern char *curr_wd;
extern char *last_wd;
extern int curr_fore_process;

void c_handler()
{
    //do nothing
}

void z_handler()
{
    //do nothing
}

void d_handler()
{   
    printf("\n");
    for(int process_counter = 0;process_counter<bg_process_count;process_counter++)
    {
        kill(process_id_list[process_counter], SIGKILL);
    }
    free(process_id_list);
    for(int i=0;i<4096;i++)
    {
        free(process_name_list[i]);
    }
    free(process_name_list);
    free(og_wd);
    free(curr_wd);
    free(last_wd);
    exit(0);
}