#include "headers.h"

extern char *curr_wd;
extern char *last_wd;
extern char *og_wd;



// IMPORTANT : Add the total number of blocks shit to this thing.



int queryFlag(char **input, int num)
{
    int aflag = 0;
    int lflag = 0;
    for(int i = 1; i <num;i++)
    {
        if(strcmp(input[i], "-l") == 0)
            lflag = 1;
        else if(strcmp(input[i], "-a") == 0)
            aflag = 1;
        else if(strcmp(input[i], "-al") == 0 || strcmp(input[i], "-la") == 0)
        {
            aflag = 1;
            lflag = 1;
        }
    }
    if(aflag == 0 && lflag == 0)
        return 0;
    else if(aflag == 1 && lflag == 0)
        return 1;
    else if(aflag == 0 && lflag == 1)
        return 2;
    else
        return 3;
}

int cmpfunc(const void *a, const void *b)
{
    return strcasecmp(*(char **)a, *(char **)b);
}

char *file_names[4096];
char *folder_names[4096];

int cmpffunc(const void *a, const void *b)
{
    return strcasecmp(file_names[*(int *)a], file_names[*(int *)b]);
}

int cmpdfunc(const void *a, const void *b)
{
    return strcasecmp(folder_names[*(int *)a], folder_names[*(int *)b]);
}

int listem(char **input, int num)
{
    int flag = queryFlag(input, num);
    DIR *dr_list[4096];
    FILE *file_list[4096];
    char *file_path[4096];
    char *og_file_names[4096];
    char *og_folder_names[4096];
    for(int i=0;i<4096;i++)
    {   
        file_names[i] = (char *) malloc(sizeof(char)* 4096);
        folder_names[i] = (char *) malloc(sizeof(char)* 4096);
        file_path[i] = (char *) malloc(sizeof(char) * 4096);
        og_file_names[i] = (char *) malloc(sizeof(char)* 4096);
        og_folder_names[i] = (char *) malloc(sizeof(char)* 4096);
    }
    char *dir_path[4096];
    for(int i=0;i<4096;i++)
    {
        dir_path[i] = (char *) malloc(sizeof(char) * 4096);
    }
    int count = 0, file_count = 0, just_flag = 0;
    char *inside_file_names[4096];
    for(int x = 0;x<4096;x++)
    {
        inside_file_names[x] = (char *) malloc(sizeof(char)*4096);
    }
    for(int i=1;i<num;i++)
    {
        if(strcmp(input[i], "-l") != 0 && strcmp(input[i], "-a") != 0 && strcmp(input[i], "-la") != 0 && strcmp(input[i], "-al") != 0)
        {   
            just_flag = 1;
            char temp[4096];
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
                // printf("%s\n", temp);
                chdir(curr_wd);
            }
            dr_list[count] = opendir(temp);
            if(dr_list[count] != NULL)
            {   
                strcpy(dir_path[count], temp);
                strcpy(folder_names[count], temp);
                for(int x = strlen(temp) - 1;x>=0;x--)
                {
                    if(temp[x] == '/' && x != strlen(temp) - 1)
                    {
                        strcpy(folder_names[count], &temp[x+1]);
                        break;
                    }
                }
                strcpy(og_folder_names[count], temp);
                if(strcmp(input[i], "..") == 0)
                {   
                    strcpy(folder_names[count], input[i]);
                    strcpy(og_folder_names[count], input[i]);
                }
                else if(strncmp(input[i], "../", 3) == 0)
                {
                    strcpy(folder_names[count], input[i]);
                    strcpy(og_folder_names[count], input[i]);
                }
                count++;
                continue;
            }
            file_list[file_count] = fopen(temp, "r");
            if(file_list[file_count] != NULL)
            {
                strcpy(file_path[file_count], temp);
                strcpy(file_names[file_count], temp);
                for(int x = strlen(temp) - 1;x>=0;x--)
                {
                    if(temp[x] == '/' && x != strlen(temp) - 1)
                    {
                        strcpy(file_names[file_count], &temp[x+1]);
                        break;
                    }
                }
                strcpy(og_file_names[file_count], temp);
                if(strcmp(input[i], "..") == 0)
                {   
                    strcpy(file_names[file_count], input[i]);
                    strcpy(og_file_names[file_count], input[i]);
                }
                else if(strncmp(input[i], "../", 3) == 0)
                {
                    strcpy(file_names[file_count], input[i]);
                    strcpy(og_file_names[file_count], input[i]);
                }
                file_count++;
                continue;
            }
            fprintf(stderr, "%s%s : File or Directory could not be accessed.%s\n", COLOR_YELLOW, temp, COLOR_RESET);
            continue;
        }
    }
    int newarr[count], newarr2[file_count];
    for(int x = 0;x<count;x++)
    {
        newarr[x] = x;
    }
    for(int x = 0;x<file_count;x++)
    {
        newarr2[x] = x;
    }
    if(file_count > 1)
        qsort(newarr2, file_count, sizeof(int), cmpffunc);
    if(count > 1)
        qsort(newarr, count, sizeof(int), cmpdfunc);
    char *tempptrarr[4096];
    for(int x=0;x<count;x++)
    {
        tempptrarr[x] = dir_path[newarr[x]];
    }
    for(int x=0;x<count;x++)
    {   
        dir_path[x] = tempptrarr[x];
    }
    for(int x=0;x<count;x++)
    {
        tempptrarr[x] = og_folder_names[newarr[x]];
    }
    for(int x=0;x<count;x++)
    {   
        og_folder_names[x] = tempptrarr[x];
    }
    for(int x=0;x<file_count;x++)
    {
        tempptrarr[x] = file_path[newarr2[x]];
    }
    for(int x=0;x<file_count;x++)
    {
        file_path[x] = tempptrarr[x];
    }
    for(int x=0;x<file_count;x++)
    {
        tempptrarr[x] = og_file_names[newarr2[x]];
    }
    for(int x=0;x<file_count;x++)
    {
        og_file_names[x] = tempptrarr[x];
    }
    DIR *tempdirptr[4096];
    for(int x=0;x<count;x++)
    {
        tempdirptr[x] = dr_list[newarr[x]];
    }
    for(int x=0;x<count;x++)
    {
        dr_list[x] = tempdirptr[x];
    }
    FILE *tempfileptr[4096];
    for(int x=0;x<file_count;x++)
    {
        tempfileptr[x] = file_list[newarr2[x]];
    }
    for(int x=0;x<file_count;x++)
    {
        file_list[x] = tempfileptr[x];
    }
    if(just_flag == 0 && file_count == 0 && count == 0)
    {   
        int another_counter = 0;
        DIR *curr_dir = opendir(curr_wd);
        struct dirent *curr;
        int total_first = 0;
        int this_is_counter = 0;
        int max[4] = {0};
        while(curr = readdir(curr_dir))
        {   
            struct stat st;
            stat(curr->d_name, &st);
            long int temp_link = st.st_nlink;
            long int temp_max = 1;
            temp_link/=10;
            while(temp_link != 0)
            {
                temp_max++;
                temp_link/=10;
            }
            if(max[0] < temp_max)
                max[0] = temp_max;
            struct passwd *new_det = getpwuid(st.st_uid);
            temp_max = strlen(new_det->pw_name); 
            if(max[1] < temp_max)
                max[1] = temp_max;
            struct group *new_grp_det = getgrgid(st.st_gid);
            temp_max = strlen(new_grp_det->gr_name);
            if(max[2] < temp_max)
                max[2] = temp_max;
            long int temp_size = st.st_size;
            temp_max = 1;
            temp_size/=10;
            while(temp_size != 0)
            {
                temp_max++;
                temp_size/=10;
            }
            if(max[3] < temp_max)
                max[3] = temp_max;
            strcpy(inside_file_names[this_is_counter++], curr->d_name);
            if(flag == 3 || (flag == 2 && curr->d_name[0] != '.'))
                total_first += st.st_blocks;
        }
        qsort(inside_file_names, this_is_counter, sizeof(char *), cmpfunc);
        if(flag == 2 || flag == 3)
            printf("total %d\n", total_first/2);
        rewinddir(curr_dir);
        for(int x =0;x<this_is_counter;x++)
        {   
            if(flag == 0)
            {   
                struct stat st;
                stat(inside_file_names[x], &st);
                int x_flag = (st.st_mode & S_IXUSR != 0) || (st.st_mode & S_IXGRP != 0) || (st.st_mode & S_IXOTH != 0);
                if(inside_file_names[x][0] != '.')
                {   
                    another_counter++;   
                    if(S_ISDIR(st.st_mode))
                        printf("%s%s%s  ", COLOR_RED, inside_file_names[x], COLOR_RESET);
                    else if(x_flag != 0)
                        printf("%s%s%s  ", COLOR_GREEN, inside_file_names[x], COLOR_RESET);
                    else
                        printf("%s  ", inside_file_names[x]);
                }
            }
            else if(flag == 1)
            {   
                struct stat st;
                stat(inside_file_names[x], &st);
                int x_flag = (st.st_mode & S_IXUSR != 0) || (st.st_mode & S_IXGRP != 0) || (st.st_mode & S_IXOTH != 0);
                another_counter++;
                if(S_ISDIR(st.st_mode))
                    printf("%s%s%s  ", COLOR_RED, inside_file_names[x], COLOR_RESET);
                else if(x_flag != 0)
                    printf("%s%s%s  ", COLOR_GREEN, inside_file_names[x], COLOR_RESET);
                else
                    printf("%s  ", inside_file_names[x]);
            }
            else if(flag == 2 || flag == 3)
            {
                struct stat st;
                if(stat(inside_file_names[x], &st) == 0)
                {   
                    if(flag == 3 || (flag == 2 && inside_file_names[x][0] != '.'))
                    {    
                        if(S_ISBLK(st.st_mode))
                            printf("b");
                        else if(S_ISCHR(st.st_mode))
                            printf("c");
                        else if(S_ISFIFO(st.st_mode))
                            printf("p");
                        else if(S_ISLNK(st.st_mode))
                            printf("l");
                        else if(S_ISSOCK(st.st_mode))
                            printf("s");
                        else if(S_ISDIR(st.st_mode))
                            printf("d");
                        else
                            printf("-");
                        if(st.st_mode & S_IRUSR)
                            printf("r");
                        else
                            printf("-");
                        if(st.st_mode & S_IWUSR)
                            printf("w");
                        else
                            printf("-");
                        if(st.st_mode & S_IXUSR)
                            printf("x");
                        else
                            printf("-");
                        if(st.st_mode & S_IRGRP)
                            printf("r");
                        else
                            printf("-");
                        if(st.st_mode & S_IWGRP)
                            printf("w");
                        else
                            printf("-");
                        if(st.st_mode & S_IXGRP)
                            printf("x");
                        else
                            printf("-");
                        if(st.st_mode & S_IROTH)
                            printf("r");
                        else
                            printf("-");
                        if(st.st_mode & S_IWOTH)
                            printf("w");
                        else
                            printf("-");
                        if(st.st_mode & S_IXOTH)
                            printf("x ");
                        else
                            printf("- ");
                        char str_nlink[4096];
                        sprintf(str_nlink, "%ld", st.st_nlink);
                        printf("%*s ", max[0], str_nlink);
                        struct passwd *u_det;
                        if (NULL == (u_det = getpwuid(st.st_uid)))
                            fprintf(stderr, "\n%sUnexpected error occurred\n%s", COLOR_YELLOW, COLOR_RESET);
                        else
                            printf("%*s ", max[1],u_det->pw_name);
                        struct group *grp_det;
                        if ((grp_det = getgrgid(st.st_gid)) == NULL)
                            fprintf(stderr, "\n%sUnexpected error occurred\n%s", COLOR_YELLOW, COLOR_RESET);
                        else
                            printf("%*s ", max[2], grp_det->gr_name);
                        char str_size[4096];
                        sprintf(str_size, "%ld", st.st_size);
                        printf("%*s ", max[3], str_size);
                        char lastmodif[4096];
                        strcpy(lastmodif, ctime(&st.st_mtime));
                        char *date_time[4096];
                        date_time[0] = strtok(lastmodif, " \t");
                        int date_time_counter = 0;
                        while(date_time[date_time_counter] != NULL)
                        {
                            date_time_counter++;
                            date_time[date_time_counter] = strtok(NULL, " \t");
                        }
                        time_t time_now;
                        time(&time_now);
                        printf("%s ", date_time[1]);
                        printf("%*s", 2, date_time[2]);
                        date_time[4][strlen(date_time[4]) - 1] = '\0';
                        if(difftime(time_now, st.st_mtim.tv_sec) > 15552000LL)
                            printf("%*s ", 6, date_time[4]);
                        else
                        {
                            date_time[3][5] = '\0';
                            printf(" %s ", date_time[3]);
                        }
                        if(S_ISDIR(st.st_mode))
                            printf("%s%s%s  ", COLOR_RED, inside_file_names[x], COLOR_RESET);
                        else if((st.st_mode & S_IXUSR != 0) || (st.st_mode & S_IXGRP != 0) || (st.st_mode & S_IXOTH != 0))
                            printf("%s%s%s  ", COLOR_GREEN, inside_file_names[x], COLOR_RESET);
                        else
                            printf("%s  ", inside_file_names[x]);
                        printf("\n");
                    }
                }
            }
        }
        if(another_counter != 0)
            printf("\n");
    }
    else if(file_count + count == 1)
    {   
        int another_counter = 0;
        if(file_count == 1)
        {
            another_counter++;
            if(flag == 1 || flag == 0)
                {   
                    struct stat st;
                    stat(file_path[0], &st);
                    if((st.st_mode & S_IXUSR != 0) || (st.st_mode & S_IXGRP != 0) || (st.st_mode & S_IXOTH != 0))
                        printf("%s%s%s  ", COLOR_GREEN, og_file_names[0], COLOR_RESET);
                    else
                        printf("%s  ", file_path[0]);
                }
            else
            {
                struct stat st;
                if(stat(file_path[0], &st) == 0)
                {   
                    {    
                        if(S_ISBLK(st.st_mode))
                            printf("b");
                        else if(S_ISCHR(st.st_mode))
                            printf("c");
                        else if(S_ISFIFO(st.st_mode))
                            printf("p");
                        else if(S_ISLNK(st.st_mode))
                            printf("l");
                        else if(S_ISSOCK(st.st_mode))
                            printf("s");
                        else if(S_ISDIR(st.st_mode))
                            printf("d");
                        else
                            printf("-");
                        if(st.st_mode & S_IRUSR)
                            printf("r");
                        else
                            printf("-");
                        if(st.st_mode & S_IWUSR)
                            printf("w");
                        else
                            printf("-");
                        if(st.st_mode & S_IXUSR)
                            printf("x");
                        else
                            printf("-");
                        if(st.st_mode & S_IRGRP)
                            printf("r");
                        else
                            printf("-");
                        if(st.st_mode & S_IWGRP)
                            printf("w");
                        else
                            printf("-");
                        if(st.st_mode & S_IXGRP)
                            printf("x");
                        else
                            printf("-");
                        if(st.st_mode & S_IROTH)
                            printf("r");
                        else
                            printf("-");
                        if(st.st_mode & S_IWOTH)
                            printf("w");
                        else
                            printf("-");
                        if(st.st_mode & S_IXOTH)
                            printf("x ");
                        else
                            printf("- ");
                        printf("%ld ", st.st_nlink);
                        struct passwd *u_det;
                        if (NULL == (u_det = getpwuid(st.st_uid)))
                            fprintf(stderr, "\n%sUnexpected error occurred\n%s", COLOR_YELLOW, COLOR_RESET);
                        else
                            printf("%s ", u_det->pw_name);
                        struct group *grp_det;
                        if ((grp_det = getgrgid(st.st_gid)) == NULL)
                            fprintf(stderr, "\n%sUnexpected error occurred\n%s", COLOR_YELLOW, COLOR_RESET);
                        else
                            printf("%s ", grp_det->gr_name);
                        printf("%ld ", st.st_size);
                        char lastmodif[4096];
                        strcpy(lastmodif, ctime(&st.st_mtime));
                        char *date_time[4096];
                        date_time[0] = strtok(lastmodif, " \t");
                        int date_time_counter = 0;
                        while(date_time[date_time_counter] != NULL)
                        {
                            date_time_counter++;
                            date_time[date_time_counter] = strtok(NULL, " \t");
                        }
                        time_t time_now;
                        time(&time_now);
                        printf("%s ", date_time[1]);
                        printf("%*s", 2, date_time[2]);
                        date_time[4][strlen(date_time[4]) - 1] = '\0';
                        if(difftime(time_now, st.st_mtim.tv_sec) > 15552000LL)
                            printf("%*s ", 6, date_time[4]);
                        else
                        {
                            date_time[3][5] = '\0';
                            printf(" %s ", date_time[3]);
                        }
                        if((st.st_mode & S_IXUSR != 0) || (st.st_mode & S_IXGRP != 0) || (st.st_mode & S_IXOTH != 0))
                            printf("%s%s%s ", COLOR_GREEN, og_file_names[0], COLOR_RESET);
                        else
                            printf("%s ", file_path[0]);
                    }
                }
            }
        }
        else
        {   
            struct dirent *curr;
            int total_first = 0;
            char temp_path[4096];
            int this_is_counter = 0;
            int max[4] = {0};
            while(curr = readdir(dr_list[0]))
            {   
                strcpy(temp_path, dir_path[0]);
                if(temp_path[strlen(temp_path) - 1] != '/')
                    strcat(temp_path, "/");
                strcat(temp_path, curr->d_name);
                strcpy(inside_file_names[this_is_counter++], curr->d_name);
                struct stat st;
                // printf("%s\n", temp_path);
                stat(temp_path, &st);
                long int temp_link = st.st_nlink;
                long int temp_max = 1;
                temp_link/=10;
                while(temp_link != 0)
                {
                    temp_max++;
                    temp_link/=10;
                }
                if(max[0] < temp_max)
                    max[0] = temp_max;
                struct passwd *new_det = getpwuid(st.st_uid);
                temp_max = strlen(new_det->pw_name); 
                if(max[1] < temp_max)
                    max[1] = temp_max;
                struct group *new_grp_det = getgrgid(st.st_gid);
                temp_max = strlen(new_grp_det->gr_name);
                if(max[2] < temp_max)
                    max[2] = temp_max;
                long int temp_size = st.st_size;
                temp_max = 1;
                temp_size/=10;
                while(temp_size != 0)
                {
                    temp_max++;
                    temp_size/=10;
                }
                if(max[3] < temp_max)
                    max[3] = temp_max;
                if(flag == 3 || (flag == 2 && curr->d_name[0] != '.'))
                    total_first += st.st_blocks;
            }
            qsort(inside_file_names, this_is_counter, sizeof(char *), cmpfunc);
            if(flag == 2 || flag == 3)
                printf("total %d\n", total_first/2);
            rewinddir(dr_list[0]);
            for(int x =0;x<this_is_counter;x++)
            {   
                struct stat st;
                strcpy(temp_path, dir_path[0]);
                strcat(temp_path, "/");
                strcat(temp_path, inside_file_names[x]);
                // printf("%s\n", dir_path[0]);
                if(flag == 0)
                {
                    if(inside_file_names[x][0] != '.')
                    {   
                        stat(temp_path, &st);
                        another_counter++;   
                        if(S_ISDIR(st.st_mode))
                            printf("%s%s%s  ", COLOR_RED, inside_file_names[x], COLOR_RESET);
                        else if((st.st_mode & S_IXUSR != 0) || (st.st_mode & S_IXGRP != 0) || (st.st_mode & S_IXOTH != 0))
                            printf("%s%s%s  ", COLOR_GREEN, inside_file_names[x], COLOR_RESET);
                        else
                            printf("%s  ", inside_file_names[x]);
                    }
                }
                else if(flag == 1)
                {   
                    stat(temp_path, &st);
                    another_counter++;
                    if(S_ISDIR(st.st_mode))
                        printf("%s%s%s  ", COLOR_RED, inside_file_names[x], COLOR_RESET);
                    else if((st.st_mode & S_IXUSR != 0) || (st.st_mode & S_IXGRP != 0) || (st.st_mode & S_IXOTH != 0))
                        printf("%s%s%s  ", COLOR_GREEN, inside_file_names[x], COLOR_RESET);
                    else
                        printf("%s  ", inside_file_names[x]);
                }
                else
                {   
                    if(stat(temp_path, &st) == 0)
                    {  
                        if(flag == 3 || (flag == 2 && inside_file_names[x][0] != '.'))
                        {    
                            if(S_ISBLK(st.st_mode))
                                printf("b");
                            else if(S_ISCHR(st.st_mode))
                                printf("c");
                            else if(S_ISFIFO(st.st_mode))
                                printf("p");
                            else if(S_ISLNK(st.st_mode))
                                printf("l");
                            else if(S_ISSOCK(st.st_mode))
                                printf("s");
                            else if(S_ISDIR(st.st_mode))
                                printf("d");
                            else
                                printf("-");
                            if(st.st_mode & S_IRUSR)
                                printf("r");
                            else
                                printf("-");
                            if(st.st_mode & S_IWUSR)
                                printf("w");
                            else
                                printf("-");
                            if(st.st_mode & S_IXUSR)
                                printf("x");
                            else
                                printf("-");
                            if(st.st_mode & S_IRGRP)
                                printf("r");
                            else
                                printf("-");
                            if(st.st_mode & S_IWGRP)
                                printf("w");
                            else
                                printf("-");
                            if(st.st_mode & S_IXGRP)
                                printf("x");
                            else
                                printf("-");
                            if(st.st_mode & S_IROTH)
                                printf("r");
                            else
                                printf("-");
                            if(st.st_mode & S_IWOTH)
                                printf("w");
                            else
                                printf("-");
                            if(st.st_mode & S_IXOTH)
                                printf("x ");
                            else
                                printf("- ");
                            char str_nlink[4096];
                            sprintf(str_nlink, "%ld", st.st_nlink);
                            printf("%*s ", max[0], str_nlink);
                            struct passwd *u_det;
                            if (NULL == (u_det = getpwuid(st.st_uid)))
                                fprintf(stderr, "\n%sUnexpected error occurred\n%s", COLOR_YELLOW, COLOR_RESET);
                            else
                                printf("%*s ", max[1],u_det->pw_name);
                            struct group *grp_det;
                            if ((grp_det = getgrgid(st.st_gid)) == NULL)
                                fprintf(stderr, "\n%sUnexpected error occurred\n%s", COLOR_YELLOW, COLOR_RESET);
                            else
                                printf("%*s ", max[2],grp_det->gr_name);
                            char str_size[4096];
                            sprintf(str_size, "%ld", st.st_size);
                            printf("%*s ", max[3], str_size);
                            char lastmodif[4096];
                            strcpy(lastmodif, ctime(&st.st_mtime));
                            char *date_time[4096];
                            date_time[0] = strtok(lastmodif, " \t");
                            int date_time_counter = 0;
                            while(date_time[date_time_counter] != NULL)
                            {
                                date_time_counter++;
                                date_time[date_time_counter] = strtok(NULL, " \t");
                            }
                            time_t time_now;
                            time(&time_now);
                            printf("%s ", date_time[1]);
                            printf("%*s", 2, date_time[2]);
                            date_time[4][strlen(date_time[4]) - 1] = '\0';
                            if(difftime(time_now, st.st_mtim.tv_sec) > 15552000LL)
                                printf("%*s ", 6, date_time[4]);
                            else
                            {
                                date_time[3][5] = '\0';
                                printf(" %s ", date_time[3]);
                            }
                            if(S_ISDIR(st.st_mode))
                                printf("%s%s%s  ", COLOR_RED, inside_file_names[x], COLOR_RESET);
                            else if((st.st_mode & S_IXUSR != 0) || (st.st_mode & S_IXGRP != 0) || (st.st_mode & S_IXOTH != 0))
                                printf("%s%s%s  ", COLOR_GREEN, inside_file_names[x], COLOR_RESET);
                            else
                                printf("%s  ", inside_file_names[x]);
                            printf("\n");
                        }
                    }
                }
            } 
        }
        if(another_counter != 0)
            printf("\n");
    }
    else
    {   
        int max[4] = {0};
        for(int i=0;i<file_count;i++)
        {
            struct stat st;
            stat(file_path[i], &st);
            long int temp_link = st.st_nlink;
            long int temp_max = 1;
            temp_link/=10;
            while(temp_link != 0)
            {
                temp_max++;
                temp_link/=10;
            }
            if(max[0] < temp_max)
                max[0] = temp_max;
            struct passwd *new_det = getpwuid(st.st_uid);
            temp_max = strlen(new_det->pw_name); 
            if(max[1] < temp_max)
                max[1] = temp_max;
            struct group *new_grp_det = getgrgid(st.st_gid);
            temp_max = strlen(new_grp_det->gr_name);
            if(max[2] < temp_max)
                max[2] = temp_max;
            long int temp_size = st.st_size;
            temp_max = 1;
            temp_size/=10;
            while(temp_size != 0)
            {
                temp_max++;
                temp_size/=10;
            }
            if(max[3] < temp_max)
                max[3] = temp_max;

        }
        for(int i=0;i<file_count;i++)
        {   
            struct stat st;
            if(flag == 0 || flag == 1)
            {   
                stat(file_path[i], &st);
                if((st.st_mode & S_IXUSR != 0) || (st.st_mode & S_IXGRP != 0) || (st.st_mode & S_IXOTH != 0))
                    printf("%s%s%s  ", COLOR_GREEN, og_file_names[i], COLOR_RESET);
                else
                    printf("%s  ", file_path[i]);
            }
            else
            {
                if(stat(file_path[i], &st) == 0)
                {   
                    {    
                        if(S_ISBLK(st.st_mode))
                            printf("b");
                        else if(S_ISCHR(st.st_mode))
                            printf("c");
                        else if(S_ISFIFO(st.st_mode))
                            printf("p");
                        else if(S_ISLNK(st.st_mode))
                            printf("l");
                        else if(S_ISSOCK(st.st_mode))
                            printf("s");
                        else if(S_ISDIR(st.st_mode))
                            printf("d");
                        else
                            printf("-");
                        if(st.st_mode & S_IRUSR)
                            printf("r");
                        else
                            printf("-");
                        if(st.st_mode & S_IWUSR)
                            printf("w");
                        else
                            printf("-");
                        if(st.st_mode & S_IXUSR)
                            printf("x");
                        else
                            printf("-");
                        if(st.st_mode & S_IRGRP)
                            printf("r");
                        else
                            printf("-");
                        if(st.st_mode & S_IWGRP)
                            printf("w");
                        else
                            printf("-");
                        if(st.st_mode & S_IXGRP)
                            printf("x");
                        else
                            printf("-");
                        if(st.st_mode & S_IROTH)
                            printf("r");
                        else
                            printf("-");
                        if(st.st_mode & S_IWOTH)
                            printf("w");
                        else
                            printf("-");
                        if(st.st_mode & S_IXOTH)
                            printf("x ");
                        else
                            printf("- ");
                        char str_nlink[4096];
                        sprintf(str_nlink, "%ld", st.st_nlink);
                        printf("%*s ", max[0], str_nlink);
                        struct passwd *u_det;
                        if (NULL == (u_det = getpwuid(st.st_uid)))
                            fprintf(stderr, "\n%sUnexpected error occurred\n%s", COLOR_YELLOW, COLOR_RESET);
                        else
                            printf("%*s ", max[1],u_det->pw_name);
                        struct group *grp_det;
                        if ((grp_det = getgrgid(st.st_gid)) == NULL)
                            fprintf(stderr, "\n%sUnexpected error occurred\n%s", COLOR_YELLOW, COLOR_RESET);
                        else
                            printf("%*s ", max[2],grp_det->gr_name);
                        char str_size[4096];
                        sprintf(str_size, "%ld", st.st_size);
                        printf("%*s ", max[3], str_size);
                        char lastmodif[4096];
                        strcpy(lastmodif, ctime(&st.st_mtime));
                        char *date_time[4096];
                        date_time[0] = strtok(lastmodif, " \t");
                        int date_time_counter = 0;
                        while(date_time[date_time_counter] != NULL)
                        {
                            date_time_counter++;
                            date_time[date_time_counter] = strtok(NULL, " \t");
                        }
                        time_t time_now;
                        time(&time_now);
                        printf("%s ", date_time[1]);
                        printf("%*s", 2, date_time[2]);
                        date_time[4][strlen(date_time[4]) - 1] = '\0';
                        if(difftime(time_now, st.st_mtim.tv_sec) > 15552000LL)
                            printf("%*s ", 6, date_time[4]);
                        else
                        {
                            date_time[3][5] = '\0';
                            printf(" %s ", date_time[3]);
                        }
                        printf(" %s ", date_time[3]);
                        if((st.st_mode & S_IXUSR != 0) || (st.st_mode & S_IXGRP != 0) || (st.st_mode & S_IXOTH != 0))
                            printf("%s%s%s ", COLOR_GREEN, og_file_names[i], COLOR_RESET);
                        else
                            printf("%s ", file_path[i]);
                        printf("\n");
                    }
                }
            }
        }
        if(file_count != 0)
        {
            max[0] = 0;
            max[1] = 0;
            max[2] = 0;
            max[3] = 0;
            printf("\n");
            if(count!=0 && flag != 2 && flag != 3)
                printf("\n");
        }
        for(int i=0;i<count;i++)
        {   
            int new_flag = 0;
            struct dirent *curr;
            printf("%s%s%s : \n", COLOR_RED, og_folder_names[i], COLOR_RESET);
            struct stat st;
            int testing;
            char temp_path[4096];
            strcpy(temp_path, dir_path[i]);
            if(temp_path[strlen(temp_path) - 1] != '/')
                strcat(temp_path, "/");
            int total_first = 0;
            int this_is_counter = 0;
            while(curr = readdir(dr_list[i]))
            {   
                char temp_temp_path[4096];
                strcpy(temp_temp_path, temp_path);
                strcat(temp_temp_path, curr->d_name);
                struct stat st;
                // printf("%s\n", temp_temp_path);
                stat(temp_temp_path, &st);
                long int temp_link = st.st_nlink;
                long int temp_max = 1;
                temp_link/=10;
                while(temp_link != 0)
                {
                    temp_max++;
                    temp_link/=10;
                }
                if(max[0] < temp_max)
                    max[0] = temp_max;
                struct passwd *new_det = getpwuid(st.st_uid);
                temp_max = strlen(new_det->pw_name); 
                if(max[1] < temp_max)
                    max[1] = temp_max;
                struct group *new_grp_det = getgrgid(st.st_gid);
                temp_max = strlen(new_grp_det->gr_name);
                if(max[2] < temp_max)
                    max[2] = temp_max;
                long int temp_size = st.st_size;
                temp_max = 1;
                temp_size/=10;
                while(temp_size != 0)
                {
                    temp_max++;
                    temp_size/=10;
                }
                if(max[3] < temp_max)
                    max[3] = temp_max;
                strcpy(inside_file_names[this_is_counter++], curr->d_name);
                if(flag == 3 || (flag == 2 && curr->d_name[0] != '.'))
                {   
                    total_first += st.st_blocks;
                }
            }
            qsort(inside_file_names, this_is_counter, sizeof(char *), cmpfunc);
            if(flag == 2 || flag == 3)
                printf("total %d\n", total_first/2);
            for(int x =0;x<this_is_counter;x++)
            {   
                char temp_temp_path[4096];
                strcpy(temp_temp_path, temp_path);
                strcat(temp_temp_path, inside_file_names[x]);
                if(flag == 0)
                {   
                    stat(temp_temp_path, &st);
                    if(inside_file_names[x][0] != '.')
                    {   
                        if(S_ISDIR(st.st_mode))
                            printf("%s%s%s ", COLOR_RED, inside_file_names[x], COLOR_RESET);
                        else if((st.st_mode & S_IXUSR != 0) || (st.st_mode & S_IXGRP != 0) || (st.st_mode & S_IXOTH != 0))
                            printf("%s%s%s ", COLOR_GREEN, inside_file_names[x], COLOR_RESET);
                        else
                            printf("%s ", inside_file_names[x]);
                    }
                }
                else if(flag == 1)
                {   
                    stat(temp_temp_path, &st);
                    if(S_ISDIR(st.st_mode))
                        printf("%s%s%s ", COLOR_RED, inside_file_names[x], COLOR_RESET);
                    else if((st.st_mode & S_IXUSR != 0) || (st.st_mode & S_IXGRP != 0) || (st.st_mode & S_IXOTH != 0))
                        printf("%s%s%s ", COLOR_GREEN, inside_file_names[x], COLOR_RESET);
                    else
                        printf("%s ", inside_file_names[x]);
                }
                else
                {   
                    struct stat st;
                    if(stat(temp_temp_path, &st) == 0)
                    {  
                        if(flag == 3 || (flag == 2 && inside_file_names[x][0] != '.'))
                        {    
                            if(S_ISBLK(st.st_mode))
                                printf("b");
                            else if(S_ISCHR(st.st_mode))
                                printf("c");
                            else if(S_ISFIFO(st.st_mode))
                                printf("p");
                            else if(S_ISLNK(st.st_mode))
                                printf("l");
                            else if(S_ISSOCK(st.st_mode))
                                printf("s");
                            else if(S_ISDIR(st.st_mode))
                                printf("d");
                            else
                                printf("-");
                            if(st.st_mode & S_IRUSR)
                                printf("r");
                            else
                                printf("-");
                            if(st.st_mode & S_IWUSR)
                                printf("w");
                            else
                                printf("-");
                            if(st.st_mode & S_IXUSR)
                                printf("x");
                            else
                                printf("-");
                            if(st.st_mode & S_IRGRP)
                                printf("r");
                            else
                                printf("-");
                            if(st.st_mode & S_IWGRP)
                                printf("w");
                            else
                                printf("-");
                            if(st.st_mode & S_IXGRP)
                                printf("x");
                            else
                                printf("-");
                            if(st.st_mode & S_IROTH)
                                printf("r");
                            else
                                printf("-");
                            if(st.st_mode & S_IWOTH)
                                printf("w");
                            else
                                printf("-");
                            if(st.st_mode & S_IXOTH)
                                printf("x ");
                            else
                                printf("- ");
                            char str_nlink[4096];
                            sprintf(str_nlink, "%ld", st.st_nlink);
                            printf("%*s ", max[0], str_nlink);
                            struct passwd *u_det;
                            if (NULL == (u_det = getpwuid(st.st_uid)))
                                fprintf(stderr, "\n%sUnexpected error occurred\n%s", COLOR_YELLOW, COLOR_RESET);
                            else
                                printf("%*s ", max[1],u_det->pw_name);
                            struct group *grp_det;
                            if ((grp_det = getgrgid(st.st_gid)) == NULL)
                                fprintf(stderr, "\n%sUnexpected error occurred\n%s", COLOR_YELLOW, COLOR_RESET);
                            else
                                printf("%*s ", max[2],grp_det->gr_name);
                            char str_size[4096];
                            sprintf(str_size, "%ld", st.st_size);
                            printf("%*s ", max[3], str_size);
                            char lastmodif[4096];
                            strcpy(lastmodif, ctime(&st.st_mtime));
                            char *date_time[4096];
                            date_time[0] = strtok(lastmodif, " \t");
                            int date_time_counter = 0;
                            while(date_time[date_time_counter] != NULL)
                            {
                                date_time_counter++;
                                date_time[date_time_counter] = strtok(NULL, " \t");
                            }
                            time_t time_now;
                            time(&time_now);
                            printf("%s ", date_time[1]);
                            printf("%*s", 2, date_time[2]);
                            date_time[4][strlen(date_time[4]) - 1] = '\0';
                            if(difftime(time_now, st.st_mtim.tv_sec) > 15552000LL)
                                printf("%*s ", 6, date_time[4]);
                            else
                            {
                                date_time[3][5] = '\0';
                                printf(" %s ", date_time[3]);
                            }
                            if(S_ISDIR(st.st_mode))
                                printf("%s%s%s ", COLOR_RED, inside_file_names[x], COLOR_RESET);
                            else if((st.st_mode & S_IXUSR != 0) || (st.st_mode & S_IXGRP != 0) || (st.st_mode & S_IXOTH != 0))
                                printf("%s%s%s ", COLOR_GREEN, inside_file_names[x], COLOR_RESET);
                            else
                                printf("%s ", inside_file_names[x]);
                            printf("\n");
                        }
                    }
                }
            }
            if(flag == 1 || flag == 0)
                printf("\n");
            if(i != count - 1)
                printf("\n");
        }
    }
    for(int i=0;i<4096;i++)
    {
        free(file_path[i]);
        free(dir_path[i]);
        free(file_names[i]);
        free(folder_names[i]);
        free(inside_file_names[i]);
        free(og_folder_names[i]);
        free(og_file_names[i]);
    }
return 0;
}