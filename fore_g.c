#include "headers.h"

extern int bg_process_count;
extern int* process_id_list;
extern char** process_name_list;
extern double time_elapsed;
extern int time_flag;
extern int curr_fore_process;

void fore_g(char **input, int num)
{   
    if(num != 2)
    {
        fprintf(stderr, "%sIncorrect number of arguments\n%s", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    int nums = atoi(input[1]);
    if(nums <= 0)
    {
        fprintf(stderr, "%sPlease enter a valid job number\n%s", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    if(nums > bg_process_count)
    {
        fprintf(stderr, "%sJob with the specified number not found.\n%s", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    pid_t proc_id = process_id_list[nums-1];
    char *tempname_proc = malloc(sizeof(char)*4096);
    strcpy(tempname_proc, process_name_list[nums-1]);
    for(int j = nums-1;j<bg_process_count-1;j++)
    {
        process_id_list[j] = process_id_list[j+1];
        strcpy(process_name_list[j], process_name_list[j+1]);
    }
    bg_process_count--;
    char proc_loc[PATH_MAX];
    sprintf(proc_loc, "/proc/%d/stat", proc_id);
    FILE *f = fopen(proc_loc, "r");
    int proc, ppid, pgrp;
    char unused[4096], state;
    fscanf(f, "%d %s %c %d %d", &proc, unused, &state, &ppid, &pgrp);
    fclose(f);
    struct timeval begin, end;
    int status;
    tcsetpgrp(STDERR_FILENO, pgrp);
    kill(proc_id, SIGCONT);
    gettimeofday(&begin, 0);
    waitpid(proc_id, &status, WUNTRACED);
    if(WIFSTOPPED(status))
    {
        int stop_status = WSTOPSIG(status);
        if(stop_status == SIGTSTP)
        {
            process_id_list[bg_process_count] = proc_id;
            strcpy(process_name_list[bg_process_count], tempname_proc);
            bg_process_count++;
        }
    }
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(STDERR_FILENO, getpgid(0));
    gettimeofday(&end, 0);
    time_elapsed = end.tv_sec - begin.tv_sec;
    time_elapsed += (end.tv_usec - begin.tv_usec)/(double)1000000;
    signal(SIGTTOU, SIG_DFL);
    time_flag = 1;
return;
}