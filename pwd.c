#include "functions.h"

void pwd()
{

    char currentDir[MAX_DIR_PATH]; 
   
    if(getcwd(currentDir, sizeof(currentDir)) == NULL)
    {
        perror("getcwd() error: ");
    }
    else
    {
        printf("%s\n", currentDir);
    }
    
}