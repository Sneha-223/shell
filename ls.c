#include "functions.h"

int is_file(char *path)         //checking if path is a file
{

    struct stat path_stat;

    //to obtain information about the named file
    int stat_err = stat(path, &path_stat);

    if (stat_err != 0)
    {
        //perror("Error");
        return -1;
    }
    else
    {
        if (S_ISREG(path_stat.st_mode) != 0)
        {
            //printf("is a file\n");
            return 1;
        }
        else
        {
            //printf("not a file\n");
            return 0;
        }
    }
}

int is_dir(char *path)              //checking is path is a directory
{
    struct stat path_stat;

    int stat_err = stat(path, &path_stat);

    if (stat_err != 0)
    {
        //perror("Error");
        return -1;
    }
    else
    {
        if (S_ISDIR(path_stat.st_mode) != 0)
        {
            //printf("is a dir\n");
            return 1;
        }
        else
        {
            //printf("not a dir\n");
            return 0;
        }
    }
}

void print_longform(char path[], char filename[])       //printing files in long listing format
{

    struct stat file_stats;
    stat(path, &file_stats);

    //permissions
    char permissions[11];

    permissions[0] = S_ISDIR(file_stats.st_mode) ? 'd' : '-';
    permissions[1] = (file_stats.st_mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (file_stats.st_mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (file_stats.st_mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (file_stats.st_mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (file_stats.st_mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (file_stats.st_mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (file_stats.st_mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (file_stats.st_mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (file_stats.st_mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';

    //number of hardlinks
    long int noOfLinks;
    noOfLinks = file_stats.st_nlink;

    //getting owner and group name
    struct passwd *owner = getpwuid(file_stats.st_uid);
    struct group *group = getgrgid(file_stats.st_gid);

    //time of last modification
    char date[100];
    char year_or_time[20];
    time_t seconds;


    //if time of last modification is over 6 months print year instead of time
    time(&seconds);

    if ((seconds - file_stats.st_mtime) > 15778800)
    {
        strftime(year_or_time, 10, "%Y", localtime(&file_stats.st_mtime));
    }
    else
    {
        strftime(year_or_time, 10, "%H:%M", localtime(&file_stats.st_mtime));
    }

    strftime(date, 90, "%b %d", localtime(&file_stats.st_mtime));

    printf("%s %ld %s %s %ld %s %s %s\n", permissions, noOfLinks, owner->pw_name, group->gr_name, file_stats.st_size, date, year_or_time, filename);
}

void execute_ls(char path[], int num_of_paths, int l_flag, int a_flag)
{
    char *path_tokens[MAX_TOKEN_NUM]; //seperating each path from the string 'path'
    char *pathStrPtr;

    path_tokens[0] = strtok_r(path, " ", &pathStrPtr);

    int i = 0;
    while (path_tokens[i] != NULL && i < num_of_paths - 1) 
    {
        i++;
        path_tokens[i] = strtok_r(NULL, " ", &pathStrPtr);
    }

    struct stat path_stat;

    for (int i = 0; i < num_of_paths; i++)
    {
        int stat_err = stat(path_tokens[i], &path_stat);

        if (is_file(path_tokens[i]) == 1) //checking if its a file
        {
            if (l_flag == 1)
            {
                print_longform(path_tokens[i], path_tokens[i]);
            }
            else
            {
                printf("%s\n", path_tokens[i]);
            }
        }

        if (is_dir(path_tokens[i]) == 1) //checking if its a directory
        {
            //opens a directory stream and returns a pointer to it
            DIR *dir_stream = opendir(path_tokens[i]);

            struct dirent *dir_entry; //ptr for an entry in the directory stream

            if (num_of_paths > 1)
            {
                printf("%s:\n", path_tokens[i]);
            }

            ///////////////
            //calculating the total number of blocks in the dir
            DIR *file_stream = opendir(path_tokens[i]);
            struct dirent *fileslist;
            long int block_size = 0;
            while ((fileslist = readdir(file_stream)) != NULL)
            {
                struct stat files;

                if (a_flag == 0 && fileslist->d_name[0] == '.')
                {
                    continue;
                }

                if (l_flag == 1)
                {
                    char relative_path[MAX_DIR_PATH];
                    strcpy(relative_path, path_tokens[i]);
                    strcat(relative_path, "/");
                    strcat(relative_path, fileslist->d_name);

                    stat(relative_path, &files);
                    block_size += files.st_blocks;
                }
            }

            if (l_flag == 1)
            {
                printf("total %ld\n", block_size/2);
            }
            /////////////////

            if (dir_stream == NULL)
            {
                perror("Error");
            }
            else
            {
                while ((dir_entry = readdir(dir_stream)) != NULL) //looping thru the entries (files) in the directory
                {

                    if (a_flag == 0 && dir_entry->d_name[0] == '.')
                    {
                        continue;
                    }

                    if (l_flag == 1)
                    {
                        char relative_path[MAX_DIR_PATH];
                        strcpy(relative_path, path_tokens[i]);
                        strcat(relative_path, "/");
                        strcat(relative_path, dir_entry->d_name);

                        print_longform(relative_path, dir_entry->d_name);
                    }
                    else
                    {
                        printf("%s\n", dir_entry->d_name);
                    }
                }

                if (i < num_of_paths - 1)
                {
                    printf("\n");
                }
            }
        }

    }
}

void ls(char *token[], long long int arg_count)
{
    //printf("argcount: %lld  tok[0] %s  tok[1] %s  tok[2] %s\n", arg_count, token[0], token[1], token[2]);

    int l_flag = 0;
    int a_flag = 0;
    int la_flag = 0;
    int just_ls = 0;
    int root_dir = 0;
    //int parent_dir = 0;
    int error_set = 0;
    int num_of_paths = 0;
    int path_given = 0;

    char path[MAX_DIR_PATH] = "";
    char cwd[MAX_DIR_PATH];
    char parentDir[MAX_DIR_PATH];

    getcwd(cwd, sizeof(cwd));

    //from i = 1 because token[0] is the command (echo)
    for (long long int i = 1; i < arg_count; i++)
    {
        if (strcmp(token[i], "-l") == 0)
        {
            l_flag = 1;
        }
        else if (strcmp(token[i], "-a") == 0)
        {
            a_flag = 1;
        }
        else if (strcmp(token[i], "-la") == 0 || strcmp(token[i], "-al") == 0)
        {
            l_flag = 1;
            a_flag = 1;
        }
        else
        {
            //path_given = 1;

            getcwd(cwd, sizeof(cwd));

            if (token[i] == NULL || strcmp(token[i], ".") == 0)
            {
                just_ls = 1;
                strcat(path, cwd);
                strcat(path, " ");
                num_of_paths++;
            }
            else if (strcmp(token[i], "~") == 0)
            {
                root_dir = 1;
                strcat(path, shell_root_dir);
                strcat(path, " ");
                num_of_paths++;
            }
            else if (strcmp(token[i], "..") == 0)
            {
                //parent_dir = 1;

                strcpy(parentDir, cwd);

                char *lastSlash = strrchr(parentDir, '/');

                if (lastSlash != NULL)
                {
                    *lastSlash = '\0';
                }

                strcat(path, parentDir);
                strcat(path, " ");
                num_of_paths++;
            }
            else
            {
                if (is_file(token[i]) == 1 || is_dir(token[i]) == 1)
                {
                    strcat(path, token[i]);
                    strcat(path, " ");
                    num_of_paths++;
                }
                else if (is_file(token[i]) == -1 || is_dir(token[i]) == -1)
                {
                    perror(token[i]);
                    error_set = 1;
                }

            }
        }
    }

    if (num_of_paths == 0 && error_set == 0)        //if no args were given make path the cwd
    {
        strcpy(path, cwd);
        num_of_paths++;
    }

    execute_ls(path, num_of_paths, l_flag, a_flag);
}