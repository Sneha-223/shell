#ifndef FUNC_HEADERS
#define FUNC_HEADERS

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <wait.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_INPUT_CMD 10000      //max length of input a user can enter for a comand
#define MAX_TOKEN_NUM 10000
#define MAX_DIR_PATH 10000
#define MAX_NUM_OF_BG_PROCESSES 10

//global var
char shell_root_dir[MAX_DIR_PATH];
char prevDir[MAX_DIR_PATH];
int exit_status;
int shellpid;

//functions
void shellPrompt();
void input(char *input);
void execute_command(char* token[], long long int arg_count);
void cd(char* token[], long long int arg_count);
void pwd();
void echo(char* token[], long long int arg_count);


#endif