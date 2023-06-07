#include "prompt.h"
#include "headers.h"

extern char* og_wd;
extern char* curr_wd;
extern int time_flag;
extern double time_elapsed;
void prompt() {
    if(time_flag == 1 && time_elapsed < 1)
        time_flag = 0;
    // char *username = malloc(4096*sizeof(char));
    char *systemname = malloc(4096*sizeof(char));
    getcwd(curr_wd, 4096);
    int hostflag = gethostname(systemname, 4096);
    // int userflag = getlogin_r(username, 4096);

    uid_t uid = getuid();
    struct passwd *p = getpwuid(uid);
    if(p == NULL)
    {
        fprintf(stderr, "%sError getting username and systemname.%s\n", COLOR_YELLOW, COLOR_RESET);
        exit(1);
    }
    int len = strlen(og_wd);
    char *newptr = "";
    if(len < strlen(curr_wd))
        newptr = &curr_wd[len];
    if(hostflag == 0 && (strncmp(og_wd, curr_wd, strlen(og_wd)) == 0) && (curr_wd[strlen(og_wd)] == '/' || strlen(curr_wd) == strlen(og_wd)) && time_flag == 0)
    {
        printf("<%s%s@%s%s:%s~%s%s>", COLOR_GREEN, p->pw_name, systemname, COLOR_RESET, COLOR_RED, newptr, COLOR_RESET);
    }
    else if(hostflag == 0 && time_flag == 0)
    {
        printf("<%s%s@%s:%s%s%s>", COLOR_GREEN, p->pw_name, systemname, COLOR_RED, curr_wd, COLOR_RESET);
    }
    if(hostflag == 0 && (strncmp(og_wd, curr_wd, strlen(og_wd)) == 0) && (curr_wd[strlen(og_wd)] == '/' || strlen(curr_wd) == strlen(og_wd)) && time_flag == 1)
    {
        printf("<%s%s@%s%s:%s~%s took %lf seconds%s>", COLOR_GREEN, p->pw_name, systemname, COLOR_RESET, COLOR_RED, newptr, time_elapsed, COLOR_RESET);
    }
    else if(hostflag == 0 && time_flag == 1)
    {
        printf("<%s%s@%s:%s%s took %lf seconds%s>", COLOR_GREEN, p->pw_name, systemname, COLOR_RED, curr_wd, time_elapsed,COLOR_RESET);
    }
    free(systemname);
return;
}
