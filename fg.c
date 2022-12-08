#include "functions.h"

void fg(char *token[], long long int arg_count)
{
    if (arg_count < 2)
    {
        printf("Error: 'fg' requires an argument\n");
        return;
    }

    int process_exists = 0;
    int job_number = atoi(token[1]);

    for (int i = 0; i < numOfbgProcesses; i++)
    {
        if (bg_processes[i].job_num == job_number)
        {
            signal(SIGTTIN, SIG_IGN); //ignoring SIGTTIN. SIGTTIN is sent to bg processes when they try to read input from terminal
            signal(SIGTTOU, SIG_IGN); //ignoring SIGTTOU. SIGTTOU is sent to bg processes when they try to write output to terminal.
                                      //If the bg is ignoring the SIGTTIN signal, an attempt to read from the controlling terminal fails

            int result;
            result = tcsetpgrp(STDIN_FILENO, bg_processes[i].pid);  //Sets the process group ID (PGID) of the fg process group associated with the terminal (referred to by STDIN_FILENO), to the pgid given (second arg)
            
            if(result == -1)
            {
                perror("tcsetpgrp error");
                exit(EXIT_FAILURE);
            }

            process_exists = 1;

            //setting current_fg
            current_fg.pid = bg_processes[i].pid;
            strcpy(current_fg.processName, bg_processes[i].processName);

            kill(bg_processes[i].pid, SIGCONT);

            delete_process(bg_processes[i].pid);    //remove process from bg process list

            //int status;

            //waitpid(bg_processes[i].pid, &status, WUNTRACED);
            waitpid(-1, NULL, WUNTRACED);           //wait for process to end

            tcsetpgrp(STDIN_FILENO, getpgrp());         //reset the fg process group back to original

            signal(SIGTTIN, SIG_DFL);                   //passes default signal handler, i.e. resets signal handler to default
            signal(SIGTTOU, SIG_DFL);
        }
    }

    if(process_exists == 0)
    {
        printf("Error: that process does not exist\n");
    }
}