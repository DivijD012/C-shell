#include "headers.h"

extern char *og_wd;
extern char *curr_wd;

int queryDiscFlag(char **input, int num)
{
    int dflag = 0;
    int fflag = 0;
    for(int i = 1; i <num;i++)
    {
        if(strcmp(input[i], "-d") == 0)
            dflag = 1;
        else if(strcmp(input[i], "-f") == 0)
            fflag = 1;
        else if(strcmp(input[i], "-fd") == 0 || strcmp(input[i], "-df") == 0)
        {
            dflag = 1;
            fflag = 1;
        }
    }
    if(dflag == 0 && fflag == 0)
        return 0;
    else if(dflag == 1 && fflag == 0)
        return 1;
    else if(dflag == 0 && fflag == 1)
        return 2;
    else
        return 0;
}

int doDiscoveryRecursively(char *directory_name, char *directory_path, char *target_file, int flag)
{   
    int return_val = 0;
    DIR *curr_dir = opendir(directory_path);
    struct dirent *curr;
    while(curr = readdir(curr_dir))
    {   
        struct stat st;
        char temp[4096];
        strcpy(temp, directory_path);
        strcat(temp, "/");
        strcat(temp, curr->d_name);
        stat(temp, &st);
        if(flag == 0 || (flag == 1 && S_ISDIR(st.st_mode)) || (flag == 2 && S_ISREG(st.st_mode)))
        {   
            if(strcmp(target_file, curr->d_name) == 0)
            {   
                return_val++;
                printf("%s/%s\n", directory_name, curr->d_name);
            }
        }
        if(S_ISDIR(st.st_mode) && strcmp(curr->d_name, ".") != 0 && strcmp(curr->d_name, "..") != 0)
        {   
            char temp_again[4096];
            strcpy(temp_again, directory_name);
            strcat(temp_again, "/");
            strcat(temp_again, curr->d_name);
            return_val += doDiscoveryRecursively(temp_again, temp, target_file, flag);
        }
    }
return return_val;
}

void doGenericDiscoveryRecursively(char *directory_name, char *directory_path, int flag)
{
    DIR *curr_dir = opendir(directory_path);
    struct dirent *curr;
    while(curr = readdir(curr_dir))
    {   
        struct stat st;
        char temp[4096];
        strcpy(temp, directory_path);
        strcat(temp, "/");
        strcat(temp, curr->d_name);
        stat(temp, &st);
        if(flag == 0 || (flag == 1 && S_ISDIR(st.st_mode)) || (flag == 2 && S_ISREG(st.st_mode)))
        {
            if(strcmp(curr->d_name, "..") != 0  && strcmp(curr->d_name, ".") != 0)
                printf("%s/%s\n", directory_name, curr->d_name);
        }
        if(S_ISDIR(st.st_mode) && strcmp(curr->d_name, ".") != 0 && strcmp(curr->d_name, ".."))
        {   
            char temp_again[4096];
            strcpy(temp_again, directory_name);
            strcat(temp_again, "/");
            strcat(temp_again, curr->d_name);
            doGenericDiscoveryRecursively(temp_again, temp, flag);
        }
    }
}

void discover(char **input, int num)
{   
    int isTarDir = 0, isTarFile = 0;
    int flag = queryDiscFlag(input, num);
    char target_directory_path[4096], target_file[4096], target_directory_name[4096];
    for(int i=1;i<num;i++)
    {
        if(input[i][0] == '"' && input[i][strlen(input[i]) - 1] == '"')
        {   
            isTarFile = 1;
            // strncpy(target_file, &input[i][1], strlen(input[i]) - 2);
            // continue;
            int len= 0;
            for(int h=1;h<strlen(input[i]) - 1;h++)
            {
                target_file[len++] = input[i][h];
            }
            target_file[len] = '\0';
            continue;
        }
        else if(strcmp(input[i], "-d") != 0 && strcmp(input[i], "-f") != 0 && strcmp(input[i], "-df") != 0 && strcmp(input[i], "-fd") != 0)
        {   
            isTarDir = 1;
            char temp[4096];
            strcpy(target_directory_name, input[i]);
            strcpy(temp, input[i]);
            if(strncmp(input[i], "~/", 2) == 0)
            {
                strcpy(temp,og_wd);
                strcat(temp, &input[i][1]);
            }
            else if(strcmp(input[i], "~") == 0)
            {
                strcpy(temp, og_wd);
            }
            else if(strncmp(input[i], "../", 3) == 0)
            {
                chdir("..");
                getcwd(temp, 4096);
                chdir(curr_wd);
                strcat(temp, &input[i][2]);
            }
            else if(strcmp(input[i], "..") == 0)
            {
                chdir("..");
                getcwd(temp, 4096);
                chdir(curr_wd);
            }
            strcpy(target_directory_path, temp);
            if(target_directory_name[strlen(target_directory_name) - 1] == '/')
                target_directory_name[strlen(target_directory_name) - 1] = '\0';
            DIR *testdir = opendir(target_directory_path);
            if(testdir == NULL)
            {
                fprintf(stderr, "%s%s : couldn't find target directory.\n%s", COLOR_YELLOW, target_directory_name, COLOR_RESET);
                return;
            }
        }
    }
    if(isTarDir == 0)
    {
        getcwd(target_directory_path, 4096);
        strcpy(target_directory_name, ".");
    }
     
    if(isTarFile == 1)
    {   
        
        int return_val = doDiscoveryRecursively(target_directory_name, target_directory_path, target_file, flag);
        if(return_val == 0 && flag == 0)
            printf("%s : File/directory not found\n", target_file);
        else if(return_val == 0 && flag == 1)
            printf("%s : Directory not found\n", target_file);
        else if(return_val == 0 && flag == 2)
            printf("%s : File not found\n", target_file);
    }
    else
    {   
        doGenericDiscoveryRecursively(target_directory_name, target_directory_path, flag);
    }
return;
}