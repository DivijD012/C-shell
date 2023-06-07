#include "headers.h"

extern int bg_process_count;
extern int* process_id_list;
extern char** process_name_list;

int cmpjfunc(const void *a, const void *b)
{
    return strcasecmp(process_name_list[*(int *)a], process_name_list[*(int *)b]);
}

int flagChecker(char **input, int num)
{
    int rflag = 0;
    int sflag = 0;
    for(int i = 1; i <num;i++)
    {
        if(strcmp(input[i], "-s") == 0)
            sflag = 1;
        else if(strcmp(input[i], "-r") == 0)
            rflag = 1;
        else if(strcmp(input[i], "-rs") == 0 || strcmp(input[i], "-sr") == 0)
        {
            rflag = 1;
            sflag = 1;
        }
    }
    if(rflag == 0 && sflag == 0)
        return 0;
    else if(rflag == 1 && sflag == 0)
        return 1;
    else if(rflag == 0 && sflag == 1)
        return 2;
    else
        return 0;
}

void jobs_user(char **input, int num)
{   
    if(bg_process_count == 0)
    {
        fprintf(stderr, "%sNo backgound processes found!%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    int jflag = flagChecker(input, num);
    int index[bg_process_count];
    for(int i=0;i<bg_process_count;i++)
    {
        index[i] = i;
    }
    qsort(index, bg_process_count, sizeof(int), cmpjfunc);
    int *sorted_pid_list;
    sorted_pid_list = (int *) malloc(sizeof(int)*4096);
    char** sorted_process_name_list = (char **) malloc(sizeof(char *)*4096);
    for(int i=0;i<4096;i++)
    {
        sorted_process_name_list[i] = (char *) malloc(sizeof(char)*4096);
    }
    for(int i=0;i<bg_process_count;i++)
    {
        sorted_pid_list[i] = process_id_list[index[i]];
        strcpy(sorted_process_name_list[i], process_name_list[index[i]]);
    }
    for(int i = 0;i<bg_process_count;i++)
    {   
        int proc_id = sorted_pid_list[i];
        char proc_loc[PATH_MAX];
        sprintf(proc_loc, "/proc/%d/stat", proc_id);
        FILE *f = fopen(proc_loc, "r");
        int proc;
        char unused[PATH_MAX], state;
        fscanf(f, "%d %s %c", &proc, unused, &state);
        if((jflag == 0 && (state == 'S' || state == 'T')) || (jflag == 1 && state == 'S') || (jflag == 2 && state == 'T'))
        {   
            // int old_index;
            // for(int j=0;j<bg_process_count;j++)
            // {
            //     if(index[j] == i)
            //         old_index = j;
            // }
            printf("[%d] ", index[i]+1);
            if(state == 'S')
                printf("Running ");
            else 
                printf("Stopped ");
            printf("%s ", sorted_process_name_list[i]);
            printf("[%d]\n", sorted_pid_list[i]);
        }
    }
    free(sorted_pid_list);
    for(int i=0;i<4096;i++)
    {
        free(sorted_process_name_list[i]);
    }
    free(sorted_process_name_list);
return;
}