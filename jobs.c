#include "functions.h"

int compare(const void *a, const void *b)
{
    struct bg_process *name1 = (struct bg_process *)a;
    struct bg_process *name2 = (struct bg_process *)b;

    return strcmp(name1->processName, name2->processName);
}

void jobs(char *token[], long long int arg_count)
{
    int r_flag = 0;
    int s_flag = 0;

    //if command is just 'jobs' then print both r and s processes
    if (arg_count == 1)
    {
        r_flag = 1;
        s_flag = 1;
    }

    //from i = 1 because token[0] is the command (jobs)
    for (long long int i = 1; i < arg_count; i++)
    {
        if (strcmp(token[i], "-r") == 0)
        {
            r_flag = 1;
        }
        else if (strcmp(token[i], "-s") == 0)
        {
            s_flag = 1;
        }
        else if (strcmp(token[i], "-rs") == 0 || strcmp(token[i], "-sr") == 0)
        {
            r_flag = 1;
            s_flag = 1;
        }
    }
    //printf("num of bgs: %d\n", numOfbgProcesses);
    qsort(bg_processes, numOfbgProcesses, sizeof(struct bg_process), compare);

    for (int i = 0; i < numOfbgProcesses; i++)
    {
        //printf("%d - pid: %d  process: %s\n", i, bg_processes[i].pid, bg_processes[i].processName);

        char statfilename[10000];

        sprintf(statfilename, "/proc/%d/stat", bg_processes[i].pid);

        FILE *fp = fopen(statfilename, "r");

        if (fp == NULL) //&& bg_processes[i].status == 1)
        {
            printf("Error: could not open stat file\n");
        }
        else
        {
            char processState[1000];

            char *strptr;
            char fileinfo[10005];
            fgets(fileinfo, 10000, fp);

            char *infoblock[100];

            infoblock[0] = strtok_r(fileinfo, " \t", &strptr);

            long long int count = 0;

            while (infoblock[count] != NULL)
            {
                count++;
                infoblock[count] = strtok_r(NULL, " \t", &strptr);

                if (count == 2)
                {
                    if (strcmp(infoblock[count], "T") == 0)
                    {
                        strcpy(processState, "Stopped");
                    }
                    else
                    {
                        strcpy(processState, "Running");
                    }
                    break;
                }
            }

            fclose(fp);

            //handling flags
            if ((strcmp(processState, "Stopped") == 0))
            {
                if (s_flag == 0)
                {
                    continue;
                }
                else if (s_flag == 1)
                {
                    printf("[%d] %s %s [%d]\n", bg_processes[i].job_num, processState, bg_processes[i].processName, bg_processes[i].pid);
                }
            }
            if ((strcmp(processState, "Running") == 0))
            {
                if (r_flag == 0)
                {
                    continue;
                }
                else if (r_flag == 1)
                {
                    printf("[%d] %s %s [%d]\n", bg_processes[i].job_num, processState, bg_processes[i].processName, bg_processes[i].pid);
                }
            }
        }
    }
}