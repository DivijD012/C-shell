#include "headers.h"

extern int bg_process_count;
extern int* process_id_list;

void back_g(char **input, int num)
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
    char proc_loc[PATH_MAX];
    sprintf(proc_loc, "/proc/%d/stat", proc_id);
    FILE *f = fopen(proc_loc, "r");
    int proc;
    char unused[4096], state;
    fscanf(f, "%d %s %c", &proc, unused, &state);
    fclose(f);
    kill(proc_id, SIGCONT);
return;
}