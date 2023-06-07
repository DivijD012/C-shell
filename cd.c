#include "headers.h"

extern char *curr_wd;
extern char *last_wd;
extern char *og_wd;

int change_d(char **input, int num)
{   
    if(num > 2)
        return 1;
    if(num == 1)
    {
        if(last_wd == NULL)
        {
           last_wd = malloc(sizeof(char)*4096);
        }
        strcpy(last_wd, curr_wd);
        strcpy(curr_wd, og_wd);
        chdir(curr_wd);
    }
    else
    {
        if(strcmp(input[1], ".") == 0)
        {
            if(last_wd == NULL)
            {
                last_wd = malloc(sizeof(char)*4096);
            }
            strcpy(last_wd, curr_wd);
            // printf("%s\n", curr_wd);
        }
        else if(strcmp(input[1], "..") == 0)
        {
            if(last_wd == NULL)
            {
                last_wd = malloc(sizeof(char)*4096);
            }
            strcpy(last_wd, curr_wd);
            chdir("..");
            getcwd(curr_wd, 4096);
        }
        else if(strcmp(input[1], "~") == 0)
        {
            if(last_wd == NULL)
            {
                last_wd = malloc(sizeof(char)*4096);
            }
            strcpy(last_wd, curr_wd);
            strcpy(curr_wd, og_wd);
            chdir(curr_wd);
        }
        else if(strcmp(input[1], "-") == 0)
        {   
            if(last_wd == NULL)
            {
                return 3;
            }
            char temp[4096];
            strcpy(temp, last_wd);
            strcpy(last_wd, curr_wd);
            strcpy(curr_wd, temp);
            printf("%s\n", curr_wd);
            chdir(curr_wd);
        }
        else
        {
            if(last_wd == NULL)
            {
                last_wd = malloc(sizeof(char)*4096);
            }
            if(input[1][0] == '~')
            {   
                input[1] = &input[1][1];
                char temp[4096], temp1[4096], temp2[4096];
                strcpy(temp1, last_wd);
                strcpy(temp2, curr_wd);
                strcpy(temp, og_wd);
                strcat(temp, input[1]);
                strcpy(last_wd, curr_wd);
                strcpy(curr_wd, temp);
                // printf("%s", curr_wd);
                int cd_flag = chdir(curr_wd);
                if(cd_flag == 0)
                    return 0;
                else
                {
                    strcpy(curr_wd, temp2);
                    strcpy(last_wd, temp1);
                    return 2;
                }
            }
            else
            {
                int cd_flag = chdir(input[1]);
                if(cd_flag != 0)
                    return 2;
                strcpy(last_wd, curr_wd);
                getcwd(curr_wd, 4096);
            }
        }
    }
    return 0;
}