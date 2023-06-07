#include "headers.h"

void echo_this(char **input, int num)
{
    if(num == 1)
    {
        printf("\n");
    }
    else
    {
        for(int i=1;i<num;i++)
        {
            if(i!=num-1)
                printf("%s ", input[i]);
            else
                printf("%s\n", input[i]);
        }
    }
    return;
}