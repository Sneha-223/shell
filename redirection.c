#include "functions.h"

//if we have cat < file1 file2 file3 , file3 will be taken
//and if we have echo "text" > file1 file2 , file1 will be taken

void redirection(char *token[], long long int arg_count, int redirection_flag, long long int redir_position)
{
    int status;

    //saving fds 
    int std_out = dup(STDOUT_FILENO);
    int std_in = dup(STDIN_FILENO);

    int input_type = 0;
    int in_redir_position = 0;
    int output_type = 0;
    int out_redir_position = 0;

    //storing type and position of redirections
    long long int i;
    for (i = 0; i < arg_count; i++)
    {
        //char filepath[10000];
        if (strcmp(token[i], "<") == 0)
        {
            input_type = 1;
            in_redir_position = i;
            //break;
        }
        if (strcmp(token[i], ">") == 0)
        {
            output_type = 1;
            out_redir_position = i;
            //break;
        }
        if (strcmp(token[i], ">>") == 0)
        {
            output_type = 2;
            out_redir_position = i;
            //break;
        }
    }
    //printf("input_type: %d  output_type: %d   input_pos: %d  out_pos: %d\n", input_type, output_type, in_redir_position, out_redir_position);

    char input_file[10000] = "";
    char output_file[10000] = "";

    if (input_type == 1 && output_type == 0) //if there is only input redirection
    {
        if ((arg_count - 1) == in_redir_position)
        {
            printf("Error: Input file not entered\n");
            return;
        }
        strcpy(input_file, token[arg_count - 1]);
        //printf("redirr\n");
    }

    else if (output_type > 0 && input_type == 1) //if there is both input and output redirection
    {
        if (in_redir_position < out_redir_position)
        {
            if (token[out_redir_position - 1] == NULL)
            {
                printf("Error: Input file not entered\n");
                return;
            }
            else
            {
                strcpy(input_file, token[out_redir_position - 1]);
            }
        }
        else if (out_redir_position < in_redir_position)
        {
            if ((arg_count - 1) == in_redir_position)
            {
                printf("Error: Input file not entered\n");
                return;
            }
            else
            {
                strcpy(input_file, token[arg_count - 1]);
            }
        }

        //strcpy(output_file, token[out_redir_position + 1]);
    }

    if (output_type > 0) //if there is only output redirection or both input and output
    {
        if (token[out_redir_position + 1] == NULL)
        {
            printf("Error: Output file not entered\n");
            return;
            //exit(EXIT_SUCCESS);
        }
        else
        {
            strcpy(output_file, token[out_redir_position + 1]);
        }
    }
    //printf("%s\n", token[out_redir_position + 1]);
    //printf("INPUT file: %s   argcount: %lld  redirpos: %lld\n", input_file, arg_count, redir_position);

    //printf("OUTPUT file: %s\n", output_file);

    //saving arguments of the command (eg: ls -l -a) into args
    char *args[redir_position + 1];

    for (long long int i = 0; i < redir_position; i++)
    {
        args[i] = token[i];
        //printf("arg: %s\n", args[i]);
    }

    args[redir_position] = NULL;

    //forking so that redir is done by child process and shell is run in parent process
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork error");
        return;
    }
    if (pid == 0) //this is executed by the child process
    {
        if (input_type == 1) //if input redir '<'
        {
            int input_fd = open(input_file, O_RDONLY);

            if (input_fd < 0)
            {
                perror("could not open file");
                exit(EXIT_SUCCESS);
                //return;
            }

            dup2(input_fd, STDIN_FILENO);       //duplicates input_fd to STDIN 
            close(input_fd);
        }
        if (output_type)
        {
            int output_fd;

            if (output_type == 1)
            {
                output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            else if (output_type == 2)
            {
                output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            }

            if (output_fd < 0)
            {
                perror("could not open file");
                exit(EXIT_SUCCESS);
                //return;
            }

            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        //executing command
        execute_command(args, redir_position); 

        //restoring STDIN and STDOUT
        dup2(std_in, 0);
        close(std_in);

        dup2(std_out, 1);
        close(std_out);

        exit(EXIT_SUCCESS);
    }
    else //only executed by the parent process
    {
        while (wait(&status) != pid)        //when child dies wait returns its pid
            ;
    }

    return;
}