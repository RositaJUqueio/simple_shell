/* Authors: Rosita J Uqueio */

#include "shell.h"
#define MAX_INPUT_LEN 256
#define MAX_ARGS 16

/*environ_var - contains environment variables for current process.*/
char **environ_var;

/**
* print_environ - prints out the current
* environment variables.
*/
void print_environ(void)
{
    char **env = environ_var;
    while (*env != NULL)
    {
        printf("%s\n", *env);
        env++;
    }
}

/**
* parses_input- function parses the user input into arguments
* @input: string to be input by user
* @args: array of string pointers to store the parsed arguments
*
* Return: number of arguments
*/
int parses_input(char *input, char **args)
{
    /*parsing input into arguments*/
    int args_count = 0;
    char *arguments = strtok(input, " ");
    while (arguments != NULL && args_count < MAX_ARGS)
    {
        args[args_count++] = arguments;
        arguments = strtok(NULL, " ");
    }
    args[args_count] = NULL;
    return args_count;
}

/**
 * command_exists - checks if the specified command exists in the system PATH
 * @command: the command to search for in the PATH
 * @path: the system PATH environment variable
 *
 * Return: a dynamically allocated string containing the path to the command
 *         NULL otherwise
 */

char* command_exists(char *command, char *path)
{
    /*checks if command exists in PATH*/
    char command_path[MAX_INPUT_LEN];
    if (path != NULL)
    {
        char *path_token = strtok(path, ":");
        while (path_token != NULL)
        {
            sprintf(command_path, "%s/%s", path_token, command);
            if (access(command_path, X_OK) == 0)
            {
                return strdup(command_path);
            }
            path_token = strtok(NULL, ":");
        }
    }
    return NULL;
}

/**
* command_execution - function executes the command specified by the user
* @args: array of string pointers containing the command and its arguments
* @path: string containing the PATH environment variable
*
* Return: void
*/
void command_execution(char *command_path, char **args)
{ 
    /*creates a child process using fork()*/
    pid_t pid = fork();
    if (pid < 0) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        /*child process*/
        execv(command_path, args);
        /*execv will only return if there is an error*/
        perror("execv");
        exit(EXIT_FAILURE);
    }
    else
    {
        /*waitpid() is called to wait for the child process to complete*/
        int status;
        waitpid(pid, &status, 0);
        /*child process status is checked using WIFEXITED() & WEXITSTATUS()*/
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            /*command exited with a non-zero status code*/
            printf("Command '%s' failed with status %d\n", args[0], WEXITSTATUS(status));
        }
    }
}

/**
* main - It sets up a loop that reads user input,
* parses it into arguments, and
* executes the corresponding command
*
* Return: returns 0 on successful execution
*/
int main(void) 
{
    char input[MAX_INPUT_LEN];
    char *args[MAX_ARGS];
    char *path = getenv("PATH");
    /*int args_count = parses_input(input, args);*/
    char *command_path = command_exists(args[0], path);

    while (1)
    {
        printf("simple_shell~$ ");
        fflush(stdout);  /*flush stdout buffer to display prompt*/

        if (fgets(input, MAX_INPUT_LEN, stdin) == NULL)
        {
            printf("\n");
            break;  /*exit on end-of-file (Ctrl+D)*/
        }
        /*remove trailing newline character from input*/
        input[strcspn(input, "\n")] = '\0';
       

        if (args[0] == NULL) 
        {
            continue; /*empty command, display prompt again*/
        }
        else if (strcmp(args[0], "exit") == 0)
        {
            /*built-in exit command*/
            break;
        }
        else if (strcmp(args[0], "env") == 0)
        {
            /*built-in env command*/
            print_environ();
            continue;
        }
        /*find command in PATH*/

        if (command_path == NULL)
        {
            /*command not found, display error message*/
            printf("Command '%s' not found\n", args[0]);
            continue;
        }
        /*execute command*/
        command_execution(command_path, args);
        free(command_path);
    }

    return (0);
}
