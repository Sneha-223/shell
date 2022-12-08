#include "functions.h"

void bg(char *token[], long long int arg_count)
{
    if (arg_count < 2)
    {
        printf("Error: 'bg' requires an argument\n");
        return;
    }

    int job_number = atoi(token[1]);
    int process_exists = 0;

    for (int i = 0; i < numOfbgProcesses; i++)
    {
        if (bg_processes[i].job_num == job_number)
        {
            kill(bg_processes[i].pid, SIGTTIN);     
            kill(bg_processes[i].pid, SIGCONT);
            process_exists = 1;
            //break;
        }
    }

    if (process_exists == 0)
    {
        printf("Error: process does not exist\n");
        return;
    }
}