#include "headers.h"

// extern char job_queue[4096][4096];
double time_elapsed;
extern int time_flag;
extern int* process_id_list;
extern char **process_name_list;
extern int bg_process_count;
extern int curr_fore_process;

int execute(char **input, int num, int flag)
{   
    char *new_input[num+1];
    for(int i=0;i<num;i++)
    {
        new_input[i] = malloc(sizeof(char)*4096);
        strcpy(new_input[i], input[i]);
    }
    new_input[num] = NULL;
    pid_t Process_ID;
    if(flag == 0)
    {   
        struct timeval begin, end;
        Process_ID = fork();
        if(Process_ID == 0)
        {   
            setpgid(0, 0);
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(STDERR_FILENO, getpgid(0));
            signal(SIGTTOU, SIG_DFL);
            int return_value = execvp(new_input[0], new_input);
            if(return_value == -1)
                fprintf(stderr, "%s%s : Command not found.%s\n", COLOR_YELLOW, new_input[0],COLOR_RESET);
            exit(108);
        }
        gettimeofday(&begin, 0);
        int status;
        waitpid(Process_ID, &status, WUNTRACED);
        if(WIFSTOPPED(status))
        {
            int stop_status = WSTOPSIG(status);
            if(stop_status == SIGTSTP)
            {
                strcpy(process_name_list[bg_process_count], input[0]);
                for(int countar = 1;countar < num;countar++)
                {
                    strcat(process_name_list[bg_process_count], " ");
                    strcat(process_name_list[bg_process_count], input[countar]);
                }
                process_id_list[bg_process_count++] = Process_ID;
            }
        }
        signal(SIGTTOU, SIG_IGN);
        tcsetpgrp(STDERR_FILENO, getpgid(0));
        signal(SIGTTOU, SIG_DFL);
        if(WIFEXITED(status))
        {
            if(WEXITSTATUS(status) == 108)
            {
                time_flag = 0;
                return Process_ID;
            }
        }
        gettimeofday(&end, 0);
        time_elapsed = end.tv_sec - begin.tv_sec;
        time_elapsed += (end.tv_usec - begin.tv_usec)/(double)1000000;
    }
    else
    {   
        printf("Creating background process...\n");
        Process_ID = fork();
        if(Process_ID == 0)
        {   
            setpgid(0, 0);
            int return_value = execvp(new_input[0], new_input);
            if(return_value == -1)
                fprintf(stderr, "%s%s : Command not found.%s\n", COLOR_YELLOW, input[0], COLOR_RESET);
            exit(108);
        }
        printf("%d\n", Process_ID);
    }
    return Process_ID;
}