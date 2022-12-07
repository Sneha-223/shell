#include "functions.h"

void input(char *input) //takes user input
{
    size_t length = strlen(input);

    char *commands[MAX_INPUT_CMD]; //array of strings
    char *token[MAX_TOKEN_NUM];
    long long int i = 0;
    char *inputStrPtr;
    char *cmdStrPtr;

    //parsing input by ; or \n
    commands[0] = strtok_r(input, ";\n", &inputStrPtr);

    while (commands[i] != NULL)
    {
        i++;
        commands[i] = strtok_r(NULL, ";\n", &inputStrPtr);
    }

    //parsing the arguments by ' ' or by \t
    for (long long j = 0; j < i; j++)
    {
        //name of command eg: 'cd' or 'ls'
        token[0] = strtok_r(commands[j], " \t", &cmdStrPtr);

        long long int arg_count = 0;

        //token -> arguments eg: '../' or '-l'
        while (token[arg_count] != NULL)
        {
            //splitting and storing the token
            arg_count++;
            token[arg_count] = strtok_r(NULL, " \t", &cmdStrPtr);
        }

        execute_command(token, arg_count); 
    }
}

void execute_command(char *token[], long long int arg_count) //check the command and call the respective function
{

    if (strcmp(token[0], "cd") == 0)
    {
        cd(token, arg_count);
    }

    else if (strcmp(token[0], "pwd") == 0)
    {
        pwd();
    }

    else if (strcmp(token[0], "echo") == 0)
    {
        //printf("inside echo\n");
        echo(token, arg_count);
    }
    else if (strcmp(token[0], "exit") == 0)
    {
        exit_status = 1;
    }
    
    // //testing
    // printf("%s  ", token[0]);
    // for(int i=1;i<arg_count;i++)
    // {
    //     printf("'%s' ", token[i]);

    // }
}

//setting global vars
char shell_root_dir[] = "";
int exit_status = 0;
int numOfbgProcesses = 0;

int main()
{
    //initializing the shell's root dir
    getcwd(shell_root_dir, sizeof(shell_root_dir));

    shellpid = getpid();

    char* line;
    do
    {
        shellPrompt(); //displays the shell prompt (username, etc)

        char inputBuffer[MAX_INPUT_CMD];
        line = fgets(inputBuffer, MAX_INPUT_CMD, stdin); //gets entire line along with '/n'

        //if ctrl + D is pressed log out of shell
        if(line == NULL)
        {
            printf("\n");
            exit_status = 1;
            continue;
        }

        input(inputBuffer);

    } while (!exit_status);
}