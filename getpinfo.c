#include "headers.h"

extern char* og_wd;

void getpinfo(char **input, int num)
{
    pid_t proc_id;
    if(num == 1)
        proc_id = getpid();
    else
        proc_id = (pid_t) atoi(input[1]);
    char proc_loc[PATH_MAX];
    sprintf(proc_loc, "/proc/%d/stat", proc_id);
    FILE *f = fopen(proc_loc, "r");
    if(f == NULL)
    {
        fprintf(stderr, "%sProcess not found%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    int proc, ppid, pgrp, session, tty_nr, tgpid;
    char unused[PATH_MAX], state;
    unsigned int flags;
    unsigned long int minflt, cminflt, majflt, cmajflt, utime, stime, vsize;
    long int cutime, cstime, priority, nice, num_threads, itrealvalue;
    unsigned long long int starttime;
    fscanf(f, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu", &proc, unused, &state, &ppid, &pgrp, &session, &tty_nr, &tgpid, &flags, &minflt, &cminflt, &majflt, &cmajflt, &utime, &stime, &cutime, &cstime, &priority, &nice, &num_threads, &itrealvalue, &starttime, &vsize);
    char proc_file_path[4096];
    sprintf(proc_file_path, "/proc/%d/exe", proc_id);
    char buff[PATH_MAX];
    ssize_t len = readlink(proc_file_path, buff, sizeof(buff)-1);
    if (len != -1)
      buff[len] = '\0';
    printf("pid : %d\n", proc_id);
    printf("Process state : %c", state);
    if(tgpid == pgrp)
        printf("+");
    printf("\n");
    printf("Memory : %lu {Virtual Memory}\n", vsize);
    if(strncmp(og_wd, buff, strlen(og_wd)) == 0 && (buff[strlen(og_wd)] == '/' || strlen(buff) == strlen(og_wd)))
        printf("Executable path : ~%s\n", &buff[strlen(og_wd)]);
    else
        printf("Executable path : %s\n", buff);
return;
}