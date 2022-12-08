#include "functions.h"

void foregroundProcess(char* token[])
{
    //printf("%s %s\n", token[0], token[1]);

    pid_t child_pid;
    int stats;

    //fork returns the pid of the child process that was created
    child_pid = fork();

    //storing the fg process info
    current_fg.pid = child_pid;
    strcpy(current_fg.processName, token[0]);

    if(child_pid < 0)
    {
        printf("Error: fork failed\n");
    }
    else if(child_pid == 0)      //this will only be executed by the child process
    {
        int result;
        
        result = execvp(token[0], token);

        if(result < 0)
        {
            printf("Error exec failed: command does not exist\n");
        }

        exit(0);
    }
    else                    //this will only be executed by the parent process
    {
        waitpid(child_pid, &stats, WUNTRACED);
    }
}