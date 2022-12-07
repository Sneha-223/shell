#include "functions.h"

void echo(char *token[], long long int arg_count)
{
    //initializing output string
    char string[MAX_TOKEN_NUM] = "";

    //from i = 1 because token[0] is the command (echo)
    for (long long int i = 1; i < arg_count; i++)
    {
        strcat(string, token[i]);
        strcat(string, " ");
    }

    printf("%s\n", string);

    return;
}