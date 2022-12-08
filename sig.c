#include "functions.h"

void sig(char *token[], long long int arg_count)
{
    if (arg_count < 3)
    {
        printf("Error: 'sig' requires 2 arguments\n");
        return;
    }
    else
    {
        int job_number = atoi(token[1]);
        int signal_number = atoi(token[2]);
        int process_exists = 0;

        //printf("%d %d   %d\n", job_number, signal_number, numOfbgProcesses);

        for (int i = 0; i < numOfbgProcesses; i++)
        {
            if (bg_processes[i].job_num == job_number)
            {
                process_exists = 1;
                kill(bg_processes[i].pid, signal_number);   //sends signal_number to the process with the given job_number
                //break;
            }
        }

        if(process_exists == 0)
        {
            printf("Error: process does not exist\n");
            return;
        }
    }
}