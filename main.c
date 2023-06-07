#include "headers.h"


char *og_wd;
char *curr_wd;
char *last_wd;
FILE *history_log, *history_num;
int history_size;
char commands[20][4096];
char self_path[4096] = {0};
// char job_queue[4096][4096];
int bg_process_count = 0;
int* process_id_list;
int time_flag = 0;
char** process_name_list;
int curr_fore_process;

int main()
{   
    process_name_list = (char **) malloc(sizeof(char *)*4096);
    process_id_list = (int *) malloc(sizeof(int)*4096);
    for(int i=0;i<4096;i++)
    {
        process_name_list[i] = (char *) malloc(sizeof(char)*4096);
    }
    ssize_t len = readlink("/proc/self/exe", self_path, sizeof(self_path)-1);
    if (len != -1)
      self_path[len] = '\0';
    char *self_ptr = strrchr(self_path, '/');
    if(self_ptr != NULL)
    {
        self_ptr[1] = '\0';
    }
    char history_log_path[4096], history_num_path[4096];
    strcpy(history_log_path, self_path);
    strcat(history_log_path, "history.txt");
    strcpy(history_num_path, self_path);
    strcat(history_num_path, "history_num.txt");
    history_num = fopen(history_num_path, "r");
    if(history_num == NULL)
    {
        history_num = fopen(history_num_path, "a+");
        fprintf(history_num, "0");
    }
    else
        history_num = fopen(history_num_path, "r");
    history_log = fopen(history_log_path, "a+");
    fscanf(history_num, "%d", &history_size);
    fclose(history_num);
    for(int i=0;i<history_size;i++)
    {
        fgets(commands[i], 4096, history_log);
        commands[i][strcspn(commands[i], "\n")] = 0;
    }
    fclose(history_log);
    og_wd = malloc(4096*sizeof(char));
    curr_wd = malloc(4096*sizeof(char));
    last_wd = NULL;
    getcwd(og_wd, 4096);
    getcwd(curr_wd, 4096);
    int book_in, book_out;
    int in_flag = 0, out_flag = 0, open_pipe_flag = 0, close_pipe_flag = 0;
    int open_pipe[2];
    int book2_in, book2_out;
    while (1)
    {   
        signal(SIGINT, c_handler);
        signal(SIGTSTP, z_handler);
        int process_exit_flag = 0;
        for(int i=0;i<bg_process_count;i++)
        {   
            int status;
            int rval = waitpid(process_id_list[i], &status, WNOHANG);
            if(rval > 0)
            {   
                process_exit_flag = 1;
                if(WIFEXITED(status))
                {   
                    int exitstatus = WEXITSTATUS(status);
                    printf("%s process with ID %d exited normally with exit status %d.\n", process_name_list[i], process_id_list[i], exitstatus);
                }
                else
                    printf("%s process with ID %d exited abnormally.\n", process_name_list[i], process_id_list[i]);
                for(int j = i;j<bg_process_count-1;j++)
                {
                    process_id_list[j] = process_id_list[j+1];
                    strcpy(process_name_list[j], process_name_list[j+1]);
                }
                bg_process_count--;
            }
        }
        if(process_exit_flag == 1)
            continue;
        prompt();
        time_flag = 0;
        char *a = takeInput();
        // char a[4096];
        // if(fgets(a, 4096, stdin) == NULL)
        //     d_handler();
        a[strcspn(a, "\n")] = 0;
        if(strcmp(a, " ;") == 0 || strcmp(a, "\t;") == 0)
        {
            fprintf(stderr, "%sUnexpected token near ;%s\n", COLOR_YELLOW, COLOR_RESET);
            continue;
        }
        if(strlen(a) != 0)
        {
            if(history_size == 0)
            {
                strcpy(commands[history_size++], a);
                history_log = fopen(history_log_path, "a+");
                fprintf(history_log, "%s\n", a);
                history_num = fopen(history_num_path, "w");
                fprintf(history_num, "%d", history_size);
                fclose(history_num);
                fclose(history_log);
            }
            else if(history_size < 20)
            {
                if(strcmp(commands[history_size - 1], a) != 0)
                {   
                    history_log = fopen(history_log_path, "a+");
                    strcpy(commands[history_size++], a);
                    fprintf(history_log, "%s\n", a);
                    history_num = fopen(history_num_path, "w");
                    fprintf(history_num, "%d", history_size);
                    fclose(history_log);
                    fclose(history_num);
                }
            }
            else
            {   
                if(strcmp(commands[history_size - 1], a) != 0)
                {    
                    for(int i=1;i<history_size;i++)
                    {
                        strcpy(commands[i-1], commands[i]);
                    }
                    history_size--;
                    strcpy(commands[history_size++], a);
                    history_log = fopen(history_log_path, "w");
                    for(int i=0;i<history_size;i++)
                    {
                        fprintf(history_log, "%s\n", commands[i]);
                    }
                    fclose(history_log);
                }
            }
        }
        char *mul_cmd[4096];
        int count_of_semi = 0;
        for(int k = 0;k < strlen(a) ; k++)
        {
            if(a[k] == ';')
                count_of_semi++;
        }
        mul_cmd[0] = strtok(a, ";");
        int cmd_count = 0;
        while(mul_cmd[cmd_count] != NULL)
        {
            cmd_count++;
            mul_cmd[cmd_count] = strtok(NULL, ";");
        }
        if(count_of_semi > cmd_count)
        {
            fprintf(stderr, "%sUnexpected token near ;%s\n", COLOR_YELLOW, COLOR_RESET);
            continue;
        }
        // printf("%d\n", cmd_count);
        for(int j=0;j<cmd_count;j++)
        {   
            int bg_flag = 0; 
            int andcount = 0;
            char *further_break[4096];
            for(int i=strlen(mul_cmd[j])-1;i>=0;i--)
            {   
                if(mul_cmd[j][i] == '&')
                {
                    bg_flag = 1;
                    break;
                }
                else if(mul_cmd[j][i] == '\t' || mul_cmd[j][i] == ' ')
                    continue;
                else
                    break;
            }
            further_break[0] = strtok(mul_cmd[j], "&");
            int first_counter = 0;
            while(further_break[first_counter] != NULL)
            {
                first_counter++;
                further_break[first_counter] = strtok(NULL, "&");
            }
            for(int i=0;i<first_counter;i++)
            {       
                char* dinput[4096];
                dinput[0] = strtok(further_break[i], " \t");
                int counter = 0;
                while(dinput[counter] != NULL)
                {
                    counter++;
                    dinput[counter] = strtok(NULL, " \t");
                }
                if(cmd_count == 1 && dinput[0] == NULL)
                {
                    continue;
                }
                else if(dinput[0] == NULL)
                {
                    fprintf(stderr, "%sUnexpected token near ;%s\n", COLOR_YELLOW, COLOR_RESET);
                    break;
                }
                char *input[4096];
                for(int copy_input_index=0;copy_input_index<4096;copy_input_index++)
                {
                    input[copy_input_index] = malloc(sizeof(char)*4096);
                }
                for(int copy_input_index=0;copy_input_index<counter;copy_input_index++)
                {
                    strcpy(input[copy_input_index], dinput[copy_input_index]);
                }
                reiter :
                int bg_process = 1;
                if(i == first_counter - 1)
                {
                    if(bg_flag == 1)
                        bg_process = 1;
                    else
                        bg_process = 0;
                }
                char *copy_input[4096];
                for(int copy_input_index=0;copy_input_index<4096;copy_input_index++)
                {
                    copy_input[copy_input_index] = malloc(sizeof(char)*4096);
                }
                int marked_in_index = counter, marked_out_index = counter, marked_pipe_index = counter;
                for(int redirect_counter=0;redirect_counter<counter;redirect_counter++)
                {   
                    if(strcmp(input[redirect_counter], "|") == 0)
                    {
                        if(redirect_counter == counter-1)
                        {
                            fprintf(stderr, "%sNo process to pipe to%s\n", COLOR_YELLOW, COLOR_RESET);
                        }
                        else
                        {   
                            open_pipe_flag = 1;
                            marked_pipe_index = redirect_counter;
                            for(int copy_input_index=0;copy_input_index<counter;copy_input_index++)
                            {
                                strcpy(copy_input[copy_input_index], input[copy_input_index]);
                            }
                            if(pipe(open_pipe) == -1)
                            {
                                fprintf(stderr, "%sSome error occurred in creating pipe%s\n", COLOR_YELLOW, COLOR_RESET);
                                break;
                            }
                            book2_out = dup(STDOUT_FILENO);
                            dup2(open_pipe[1], STDOUT_FILENO);
                            close(open_pipe[1]);
                            break;
                        }
                    }
                    if(strcmp(input[redirect_counter], "<") == 0)
                    {
                        if(redirect_counter == counter - 1)
                        {
                            fprintf(stderr, "%sNo input file found\n%s", COLOR_YELLOW, COLOR_RESET);
                        }
                        else
                        {   
                            int new_in = open(input[redirect_counter + 1], O_RDONLY);
                            if(new_in >= 0)
                            {
                                // printf("ALRIGHT INPUT CHANGED\n");
                                book_in = dup(STDIN_FILENO);
                                dup2(new_in, STDIN_FILENO);
                                close(new_in);
                                in_flag = 1;
                                marked_in_index = redirect_counter;
                            }
                            else
                                fprintf(stderr, "%sInvalid input file\n%s", COLOR_YELLOW, COLOR_RESET);
                        }
                    }
                    else if(strcmp(input[redirect_counter], ">") == 0)
                    {
                        if(redirect_counter == counter - 1)
                        {
                            fprintf(stderr, "%sNo output file found\n%s", COLOR_YELLOW, COLOR_RESET);
                        }
                        else
                        {   
                            out_flag = 1;
                            // printf("ALRIGHT OUTPUT CHANGING\n");
                            int new_out = open(input[redirect_counter + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
                            book_out = dup(STDOUT_FILENO);
                            if(new_out >= 0)
                                dup2(new_out, STDOUT_FILENO);
                            else
                                fprintf(stderr, "%sInvalid output file\n%s", COLOR_YELLOW, COLOR_RESET);
                            close(new_out);
                            marked_out_index = redirect_counter;
                        }
                    }
                    else if(strcmp(input[redirect_counter], ">>") == 0)
                    {
                        if(redirect_counter == counter - 1)
                        {
                            fprintf(stderr, "%sNo output file found\n%s", COLOR_YELLOW, COLOR_RESET);
                        }
                        else
                        {   
                            out_flag = 1;
                            // printf("ALRIGHT OUTPUT CHANGING\n");
                            int new_out = open(input[redirect_counter + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
                            book_out = dup(STDOUT_FILENO);
                            if(new_out >= 0)
                                dup2(new_out, STDOUT_FILENO);
                            else
                                fprintf(stderr, "%sInvalid output file\n%s", COLOR_YELLOW, COLOR_RESET);
                            close(new_out);
                            marked_out_index = redirect_counter;
                        }
                    }
                }
                int book_counter = counter;
                if(marked_in_index < marked_out_index)
                {
                    if(marked_pipe_index < marked_in_index)
                        counter = marked_pipe_index;
                    else
                        counter = marked_in_index;
                }
                else
                {
                    if(marked_pipe_index < marked_out_index)
                        counter = marked_pipe_index;
                    else
                        counter = marked_out_index;
                }
                if(strcmp(input[0], "cd") == 0)
                {
                    int err_flag = change_d(input, counter);
                    if(err_flag == 1)
                    {
                        fprintf(stderr, "%sToo many arguments!%s\n", COLOR_YELLOW, COLOR_RESET);
                    }
                    else if(err_flag == 2)
                    {
                        fprintf(stderr, "%sNo such file or directory found.%s\n", COLOR_YELLOW, COLOR_RESET);
                    }
                    else if(err_flag == 3)
                    {
                        fprintf(stderr, "%sOld working directory not set.%s\n", COLOR_YELLOW, COLOR_RESET);
                    }
                }
                else if(strcmp(input[0], "pwd") == 0)
                {
                    print_wd();
                }
                else if(strcmp(input[0], "echo") == 0)
                {
                    echo_this(input, counter);
                }
                else if(strcmp(input[0], "ls") == 0)
                {
                    int list_flag = listem(input, counter);
                }
                else if(strcmp(input[0], "pinfo") == 0)
                {
                    getpinfo(input, counter);
                }
                else if(strcmp(input[0], "history") == 0)
                {
                    hist();
                }
                else if(strcmp(input[0], "discover") == 0)
                {   
                    discover(input, counter);
                }
                else if(strcmp(input[0], "jobs") == 0)
                {   
                    jobs_user(input, counter);
                }
                else if(strcmp(input[0], "sig") == 0)
                {   
                    send_signal(input, counter);
                }
                else if(strcmp(input[0], "fg") == 0)
                {   
                    fore_g(input, counter);
                }
                else if(strcmp(input[0], "bg") == 0)
                {   
                    back_g(input, counter);
                }
                else if(strcmp(input[0], "exit") == 0)
                {   
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
                    return 0;
                }
                else
                {      
                    if(i == first_counter - 1 && bg_process == 0)
                        time_flag = 1;
                    else
                        time_flag = 0;
                    int process_id = execute(input, counter, bg_process);
                    if(bg_process == 1)
                    {   
                        strcpy(process_name_list[bg_process_count], input[0]);
                        for(int countar = 1;countar < counter;countar++)
                        {
                            strcat(process_name_list[bg_process_count], " ");
                            strcat(process_name_list[bg_process_count], input[countar]);
                        }
                        process_id_list[bg_process_count++] = process_id;
                    }
                }
                if(in_flag == 1)
                {
                    dup2(book_in, STDIN_FILENO);
                    close(book_in);
                }
                if(out_flag == 1)
                {
                    dup2(book_out, STDOUT_FILENO);
                    close(book_out);
                }
                in_flag = 0;
                out_flag = 0;
                if(close_pipe_flag == 1)
                {   
                    close_pipe_flag = 0;
                    dup2(book2_in, STDIN_FILENO);
                    close(book2_in);
                }
                if(open_pipe_flag == 1)
                {
                    open_pipe_flag = 0;
                    close_pipe_flag = 1;
                    dup2(book2_out, STDOUT_FILENO);
                    close(book2_out);
                    book2_in = dup(STDIN_FILENO);
                    dup2(open_pipe[0], STDIN_FILENO);
                    close(open_pipe[0]);
                    for(int copy_index = marked_pipe_index+1;copy_index < book_counter;copy_index++)
                    {   
                        strcpy(input[copy_index - marked_pipe_index - 1], copy_input[copy_index]);
                        // printf("\n%s\n", input[copy_index - marked_pipe_index - 1]);
                        // printf("%p\n", copy_index - marked_pipe_index - 1);
                        // if(copy_index - marked_pipe_index - 1 > 0)
                            // printf("%p", input[copy_index - marked_pipe_index - 2]);
                    }
                    counter = book_counter - marked_pipe_index - 1;
                    // for(int lmao_index = 0;lmao_index < counter;lmao_index++)
                    //     printf("\n%s\n", input[lmao_index]);
                    goto reiter;
                }
            }
        }
    }
return 0;
}