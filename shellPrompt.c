#include "functions.h"

void shellPrompt()
{
    //gethostname to get computer name, and getlogin_r to get login username
    //_SC_HOST_NAME_MAX (from limits.h) - _SC_ gets configuration information at run time 
    
    char hostname[_SC_HOST_NAME_MAX];
    char username[_SC_LOGIN_NAME_MAX];
    gethostname(hostname, _SC_HOST_NAME_MAX);
    getlogin_r(username, _SC_LOGIN_NAME_MAX);

    char currentDir[MAX_DIR_PATH]; 
    getcwd(currentDir, sizeof(currentDir));

    char dir[MAX_DIR_PATH] = "";    //temp var to hold dir path

    //if cwd is the root dir show relative path '~'
    if(strcmp(currentDir, shell_root_dir) == 0)
    {
        strcpy(dir, "~");    
    }

    //relative path 
    else if(strlen(currentDir)>strlen(shell_root_dir) && strncmp(currentDir, shell_root_dir, strlen(shell_root_dir))==0)
    {
        for(int i = (strlen(shell_root_dir)+1), j = 0; i<strlen(currentDir);i++, j++)
        {
            dir[j] = currentDir[i]; 
        }
    }

    else        //absolute path
    {
        strcpy(dir, currentDir);
    }

    printf("<%s@%s:%s>", username, hostname, dir);
}
