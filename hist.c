#include "headers.h"

extern int history_size;
extern char commands[20][4096];

void hist()
{   
    if(history_size <= 10)
    {   
        for(int i=0;i<history_size;i++)
        {
            printf("%s\n", commands[i]);
        }
    }
    else
    {
        for(int i=history_size - 10; i < history_size;i++)
        {
            printf("%s\n", commands[i]);
        }
    }
    return;
}