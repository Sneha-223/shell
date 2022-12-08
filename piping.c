#include "functions.h"

void piping(char *token[], long long int arg_count)
{
    char command[10000] = "";

    //creating string of the entire command
    for (int j = 0; j < arg_count; j++)
    {
        strcat(command, token[j]);
        strcat(command, " ");
    }

    //parsing string by '|'
    char *pipe_commands[1000];
    char *inputStrPtr;
    int no_of_pipe_cmds = 0;

    pipe_commands[0] = strtok_r(command, "|\n", &inputStrPtr);


    while (pipe_commands[no_of_pipe_cmds] != NULL)
    {
        no_of_pipe_cmds++;
        pipe_commands[no_of_pipe_cmds] = strtok_r(NULL, "|\n", &inputStrPtr);
    }

    int fd[2], temp_fd = 0;
    pid_t pid;

    for (int m = 0; m < no_of_pipe_cmds; m++)
    {
        char *tokens[1000];
        int k = 0;
        tokens[0] = strtok_r(pipe_commands[m], " \n", &inputStrPtr);

        while (tokens[k] != NULL)       //tokenizing the pipe command
        {
            k++;
            tokens[k] = strtok_r(NULL, " \n", &inputStrPtr);
        }

        // //testing

        // printf("\n");
        // for (int l = 0; l < k; l++)
        // {
        //     printf("tok: %s\n", tokens[l]);
        // }

        //printf("m: %d   token0: %s  token1: %s\n", m, tokens[0], tokens[1]);

        if (pipe(fd) < 0)
        {
            perror("Pipe error");
            return;
        }

        pid = fork();

        if (pid < 0)
            perror("Fork error");

        else if (pid > 0)   // parent process
        {
            wait(NULL);
            close(fd[1]);       //closing write end of the pipe since the parent doesnt write to the pipe
            temp_fd = fd[0];    //making read end '0', i.e STDIN
        }

        else if (pid == 0)      // child process
        {
            dup2(temp_fd, 0);

            if (m < (no_of_pipe_cmds - 1))       //if its not the last pipe command then duplicate write end to STDOUT (so for the last command output will be STDOUT)
            {
                dup2(fd[1], 1);
            }

            close(fd[0]);       //closing read end of the pipe since child doesnt write to the pipe

            execute_command(tokens, k);

            exit(0);
        }
    }

    return;
}
