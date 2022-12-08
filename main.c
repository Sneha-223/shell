#include "functions.h"

int pipecheck(char *token[], long long int arg_count)
{
    int pipe_flag = 0;

    for (long long int i = 0; token[i] != NULL; i++)
    {
        if (strcmp(token[i], "|") == 0)
        {
            pipe_flag = 1;
            break;
        }
    }

    return pipe_flag;
}

int redirection_check(char *token[], long long int arg_count, long long int *redirection_position)
{
    //long long int redirection_position = 0;
    int redirection_flag = 0;

    int i = 0;

    while (token[i] != NULL && i < arg_count)
    {
        //checking for redirection
        if (strcmp(token[i], "<") == 0)
        {
            redirection_flag = 1;
            *redirection_position = i;
            break;
        }
        else if (strcmp(token[i], ">") == 0)
        {
            redirection_flag = 2;
            *redirection_position = i;
            break;
        }
        else if (strcmp(token[i], ">>") == 0)
        {
            redirection_flag = 3;
            *redirection_position = i;
            break;
        }

        i++;
    }

    return redirection_flag;
}


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

    //checking for redirection_flag
    long long int redirection_position;
    int redirection_flag = redirection_check(token, arg_count, &redirection_position);

    int pipe_flag = pipecheck(token, arg_count);

    if (redirection_flag > 0 && pipe_flag == 0)
    {
        //printf("redir flag: %d, position: %lld\n", redirection_flag, redirection_position);
        redirection(token, arg_count, redirection_flag, redirection_position);
        return;
    }

    if (pipe_flag > 0)
    {
        piping(token, arg_count);

        return;
    }


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
        echo(token, arg_count);
    }

    else if (strcmp(token[0], "ls") == 0)
    {
        ls(token, arg_count);
    }
    else if (strcmp(token[0], "pinfo") == 0)
    {
        pinfo(token[1], arg_count);
    }
    else if (strcmp(token[0], "repeat") == 0)
    {

        int numOfRepeats = atoi(token[1]);

        char *repeatcommand[MAX_TOKEN_NUM];

        long long int l = 0;
        for (long long int j = 2; j < arg_count; j++, l++)
        {
            repeatcommand[l] = token[j];
        }

        for (int k = 0; k < numOfRepeats; k++)
        {
            execute_command(repeatcommand, l); 
        }
    }
    else if (strcmp(token[arg_count - 1], "&") == 0)
    {
        char *bgcommand[MAX_TOKEN_NUM] = {NULL};

        for (long long int j = 0; j < arg_count - 1; j++)
        {
            bgcommand[j] = token[j];
            //printf("%s\n", bgcommand[j]);
        }

        backgroundProcess(bgcommand);
    }
    else if (strcmp(token[0], "jobs") == 0)
    {
        jobs(token, arg_count);
    }
    else if (strcmp(token[0], "sig") == 0)
    {
        sig(token, arg_count);
    }
    else if (strcmp(token[0], "fg") == 0)
    {
        fg(token, arg_count);
    }
    else if (strcmp(token[0], "bg") == 0)
    {
        bg(token, arg_count);
    }
    else if (strcmp(token[0], "exit") == 0)
    {
        exit_status = 1;
    }
    else
    {
        foregroundProcess(token);
    }
    
    // //testing
    // printf("%s  ", token[0]);
    // for(int i=1;i<arg_count;i++)
    // {
    //     printf("'%s' ", token[i]);

    // }
}

void ctrl_z_handler(int signal_number)
{
    pid_t curr_pid = getpid();
    if (curr_pid != shellpid)
    {
        return;
    }
    if (current_fg.pid != -1)   //if fg exists
    {
        kill(current_fg.pid, SIGTTIN);          //signal is sent to say the process won't get input now because its not in the fg
        kill(current_fg.pid, SIGTSTP);          //process is stopped

        bg_processes[numOfbgProcesses].pid = current_fg.pid;
        strcpy(bg_processes[numOfbgProcesses].processName, current_fg.processName);
        bg_processes[numOfbgProcesses].job_num = numOfbgProcesses + 1;

        numOfbgProcesses++;
        //printf("numogbg: %d\n", numOfbgProcesses);
        //printf("job num: %d pid: %d %s\n", bg_processes[numOfbgProcesses - 1].job_num, bg_processes[numOfbgProcesses - 1].pid, bg_processes[numOfbgProcesses - 1].processName);

        return;
    }

    //Reseting handler to catch SIGTSTP next time
    signal(SIGTSTP, ctrl_z_handler);

    return;
}

void ctrl_c_handler(int signal_number)
{
    pid_t curr_pid = getpid();
    if (curr_pid != shellpid)
        return;
    if (current_fg.pid != -1)
    {
        kill(current_fg.pid, SIGINT);
    }

    //Reseting handler to catch SIGINT next time
    signal(SIGINT, ctrl_c_handler);

    return;
}

//setting global vars
char shell_root_dir[] = "";
int exit_status = 0;
int numOfbgProcesses = 0;

int main()
{
    //initializing the shell's root dir
    getcwd(shell_root_dir, sizeof(shell_root_dir));
    current_fg.pid = -1;
    shellpid = getpid();

    signal(SIGTSTP, ctrl_z_handler);
    signal(SIGINT, ctrl_c_handler);
    signal(SIGCHLD, handler);       //to handle bg processes that have gone from stopped -> running (i.e bg <job_num>)

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