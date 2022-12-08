#include "functions.h"

void pinfo(char token[], long long int argcount)
{
    pid_t pid;

    if (token != NULL)
    {
        pid = atoi(token);
    }
    else
    {
        pid = getpid();
    }

    char statfilename[10000];

    sprintf(statfilename, "/proc/%d/stat", pid);

    FILE *fp = fopen(statfilename, "r");

    if (fp == NULL)
    {
        printf("Error: process doesn't exist\n");
    }
    else
    {
        char processState[1000];
        char virtualMemory[1000];

        char *strptr;
        char fileinfo[10005];
        fgets(fileinfo, 10000, fp);

        char *infoblock[100];

        infoblock[0] = strtok_r(fileinfo, " \t", &strptr);

        char processGrpID[1000];
        char fgProcessGrpID[1000];

        long long int count = 0;

        while (infoblock[count] != NULL)
        {
            count++;
            infoblock[count] = strtok_r(NULL, " \t", &strptr);

            if (count == 2)
            {
                strcpy(processState, infoblock[count]);
            }
            if (count == 22)
            {
                strcpy(virtualMemory, infoblock[count]);
            }
            if (count == 4)
            {
                strcpy(processGrpID, infoblock[count]);
            }
            if (count == 7)
            {
                strcpy(fgProcessGrpID, infoblock[count]);
            }
        }

        printf("pid -- %d\n", pid);

        int addplus = 0;

        if (strcmp(processGrpID, fgProcessGrpID) == 0)
        {
            addplus = 1;
        }

        printf("Process Status -- %s", processState);
        if (addplus == 1)
        {
            printf("%c\n", '+');
        }
        else
        {
            printf("\n");
        }

        printf("memory -- %s\n", virtualMemory);

        char execfilename[10000];
        char execpath[10000] = "";

        sprintf(execfilename, "/proc/%d/exe", pid);
        readlink(execfilename, execpath, sizeof(execpath));

        char relativeExecPath[10000];
        strcpy(relativeExecPath, execpath);

        char *lastSlash = strrchr(relativeExecPath, '/');
        char *relative = strrchr(execpath, '/');

        if (lastSlash != NULL)
        {
            *lastSlash = '\0';
        }

        if(strcmp(relativeExecPath, shell_root_dir)==0)
        {
            printf("Executable Path -- ~%s\n", relative);
        }
        else
        {
            printf("Executable Path -- %s\n", execpath);
        }


        fclose(fp);
    }
}