#include "headers.h"

extern int *process_id_list;
extern int bg_process_count;

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int auto_completion(char *inp, int size)
{   
    char *curr_work_dir;
    curr_work_dir = malloc(sizeof(char)*4096);
    getcwd(curr_work_dir, 4096);
    // printf("\n%s %d\n", inp, size);
    if(size == 0)
    {   
        DIR *curr_dir = opendir(curr_work_dir);
        printf("\n");
        struct dirent *curr;
        while(curr = readdir(curr_dir))
        {   
            struct stat st;
            stat(curr->d_name, &st);
            if(S_ISDIR(st.st_mode))
                printf("%s/\n", curr->d_name);
            else
                printf("%s\n", curr->d_name);
        }
        prompt();
    }
    else
    {
        int start = 0;
        for(int i=strlen(inp)-1;i>=0;i--)
        {
            if(inp[i] == ' ')
            {
                start = i+1;
                break;
            }
        }
        if(start == strlen(inp))
        {   
            printf("\n");
            DIR *curr_dir = opendir(curr_work_dir);
            struct dirent *curr;
            while(curr = readdir(curr_dir))
            {   
                struct stat st;
                stat(curr->d_name, &st);
                if(S_ISDIR(st.st_mode))
                    printf("%s/\n", curr->d_name);
                else
                    printf("%s\n", curr->d_name);
            }
            prompt();
            printf("%s", inp);
        }
        else
        {
            // printf("%s", &inp[start]);
            DIR *curr_dir = opendir(curr_work_dir);
            struct dirent *curr;
            char *names[4096];
            int isdir[4096];
            for(int i=0;i<4096;i++)
            {
                names[i] = malloc(sizeof(char)*4096);
            }
            int count = 0;
            while(curr = readdir(curr_dir))
            {   
                struct stat st;
                stat(curr->d_name, &st);
                if(strncmp(&inp[start], curr->d_name, strlen(&inp[start])) == 0)
                {   
                    isdir[count] = S_ISDIR(st.st_mode);
                    strcpy(names[count++], curr->d_name);
                }
            }
            // printf("\n%d\n", count);
            if(count == 1)
            {   
                int len = strlen(inp);
                strcpy(&inp[start], names[0]);
                if(isdir[0])
                    strcat(inp, "/");
                strcat(inp, " ");
                for(int i=0;i<len;i++)
                {
                    printf("\b");
                }
                printf("%s", inp);
            }
            else if(count == 0)
            {
                //do nothing
            }
            else
            {   
                printf("\n");   
                for(int i=0;i<count;i++)
                {
                    printf("%s\n", names[i]);
                }
                char* complete;
                complete = malloc(sizeof(char)*4096);
                strcpy(complete, &inp[start]);
                // printf("%s\n", complete);
                int min_size = 4096;
                for(int j=0;j<count;j++)
                {
                    if(strlen(names[j]) < min_size)
                        min_size = strlen(names[j]);
                }
                int strflag = 0;
                // printf("COMPLETE : %s\n", complete);
                while(1)
                {
                    for(int j=0;j<count-1;j++)
                    {   
                        // printf("%s %s %d\n", names[j], names[j+1], strlen(complete) + 1);
                        if(strncmp(names[j], names[j+1], strlen(complete) + 1) != 0)
                        {
                            strflag = 1;
                            break;
                        }
                    }
                    if(strflag == 1)
                        break;
                    int strnsize = strlen(complete) + 1;
                    strncpy(complete, names[0], strnsize);
                    // printf("COMPLETE : %s\n", complete);
                }
                strcpy(&inp[start], complete);
                prompt();
                printf("%s", inp);
                free(complete);
            }
            for(int i=0;i<4096;i++)
            {
                free(names[i]);
            }
        }
    }
    free(curr_work_dir);
    return strlen(inp);
}

char* takeInput() {
    // printf("TAKING INPUT\n");
    char *inp = malloc(sizeof(char) * 4096);
    char c;
    while (1) {
        setbuf(stdout, NULL);
        enableRawMode();
        memset(inp, '\0', 4096);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            if (iscntrl(c)) {
                if (c == 10)
                {
                    printf("\n");
                    disableRawMode();
                    return inp;
                }
                if (c == 27) {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
                        // printf("%s", buf);
                    }
                } else if (c == 127) { // backspace
                    if (pt > 0) {
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                } else if (c == 9) { // TAB character
                        pt = auto_completion(inp, pt);
                        // printf("%d", pt);
                } else if (c == 4) {
                    printf("\n");
                    for(int i=0;i<bg_process_count;i++)
                        kill(process_id_list[i], 9);
                    exit(0);
                } else {
                    printf("%d\n", c);
                }
            } else {
                inp[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();
    }
}