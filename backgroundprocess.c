#include "functions.h"


void delete_process(int pid)
{
    int process_exists = 0;
    for (int i = 0; i < numOfbgProcesses; i++)
    {
        if (bg_processes[i].pid == pid)
        {
            process_exists = 1;
            for (int j = i; j < numOfbgProcesses-1; j++)
            {
                bg_processes[j] = bg_processes[j + 1];
            }
            numOfbgProcesses--;
        }
    }
    if(process_exists == 0)
    {
        printf("Error: could not be deleted, process doesn't exist\n");
    }
}

//int numOfbgProcesses = 0;

void handler(int sig)
{
    pid_t pid;
    int status;

    pid = waitpid(-1, &status, WNOHANG);

    if (pid > 0)
    {
        for (int i = 0; i < numOfbgProcesses; i++)
        {
            //printf("%d: %s", i, bg_processes[i].processName);

            if (bg_processes[i].pid == pid)
            {
                if (WIFEXITED(status))
                {
                    printf("\n%s with pid %d has exited normally", bg_processes[i].processName, bg_processes[i].pid);
                }
                else
                {
                    printf("\n%s with pid %d has exited abnormally", bg_processes[i].processName, bg_processes[i].pid);
                }
                bg_processes[i].status = 0;
                delete_process(bg_processes[i].pid);

                fflush(stdout);

                break;
            }
        }
    }

    return;
}

void backgroundProcess(char *token[])
{

    pid_t child_pid;
    int stats;

    pid_t back_pid;

    //fork returns the pid of the child process that was created
    child_pid = fork();

    setpgid(0, 0);

    if (child_pid < 0)
    {
        printf("Error: fork failed\n");
    }
    else if (child_pid == 0) //this will only be executed by the child process
    {
        //setpgid(0, 0);

        pid_t p_id = getpid();

        int result = execvp(token[0], token);

        if (result < 0)
        {
            printf("Error exec failed: command does not exist\n");
            //fflush(stdout);
        }
        // if (strcmp(token[0], "vim") == 0)
        // {
        //     kill(p_id, 19);
        // }

        exit(0);
    }
    else //this will only be executed by the parent process
    {
        printf("%d\n", child_pid);

        strcpy(bg_processes[numOfbgProcesses].processName, token[0]);
        bg_processes[numOfbgProcesses].pid = child_pid;
        bg_processes[numOfbgProcesses].status = 1;
        bg_processes[numOfbgProcesses].job_num = numOfbgProcesses + 1;

        numOfbgProcesses++;

        signal(SIGCHLD, handler);
    }
}