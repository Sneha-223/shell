#include "functions.h"

void cd(char *token[], long long int arg_count)
{
    char currentDir[MAX_DIR_PATH];
    getcwd(currentDir, sizeof(currentDir));

    int result = 0;

    if (arg_count > 2)
    {
        printf("Error: too many arguments to cd\n");
    }

    else
    {
        if (token[1] == NULL)                       //no args given
        {
            getcwd(prevDir, sizeof(prevDir));
            result = chdir(shell_root_dir);
        }
        else if (strcmp(token[1], "~") == 0)        //root dir of the shell
        {
            getcwd(prevDir, sizeof(prevDir));
            result = chdir(shell_root_dir);
        }
        else if (strcmp(token[1], "..") == 0)       //parent dir
        {
            getcwd(prevDir, sizeof(prevDir));
            result = chdir("../");
        }
        else if (strcmp(token[1], "-") == 0)        //prev dir that was accessed
        {
            if(strcmp(prevDir, "") == 0)
            {
                printf("Error: there is no previous directory\n");
            }
            else
            {
                char temp[MAX_DIR_PATH];
                getcwd(temp, sizeof(temp));

                printf("%s\n", prevDir);
                chdir(prevDir);

                strcpy(prevDir, temp);
            }
        }
        else                                        //cd <dir path>
        {
            getcwd(prevDir, sizeof(prevDir));
            result = chdir(token[1]);
        }
    }

    //if there was an error in changing directories
    if (result == -1)
    {
        perror("Error: ");
    }

    getcwd(currentDir, sizeof(currentDir));
}