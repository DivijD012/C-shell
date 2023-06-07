#include "headers.h"

extern int bg_process_count;
extern int *process_id_list;
extern char **process_name_list;

void send_signal(char **input, int num)
{
    if(num!=3)
    {
        fprintf(stderr, "%sError in input, format :\nsig <job_number> <signal_number>\n%s", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    int job_number = atoi(input[1]);
    if(job_number == 0)
    {
        fprintf(stderr, "%sPlease enter a number as job number.\n%s", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    if(job_number > bg_process_count || job_number < 0)
    {
        fprintf(stderr, "%sJob not found.\n%s", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    int sig_number = atoi(input[2]);
    int proc_id = process_id_list[job_number-1];
    if(sig_number > 0 && sig_number < 32)
        kill(proc_id, sig_number);
    else
        fprintf(stderr, "%sPlease enter a valid signal number\n%s", COLOR_YELLOW, COLOR_RESET);
return;
}