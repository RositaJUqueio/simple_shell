/* Authors: Rosita J Uqueio */

#include "shell.h"
#include <stdlib.h>
#define MAX_INPUT_LEN 256
#define MAX_ARGS 16


/**
* print_environ - prints out the current
* environment variables.
*/
void print_environ(void)
{
	char **env = environ;

	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
}

/**
 * parses_input - function parses the user input into arguments
 * @input: string to be input by user
 * @args: array of string pointers to store the parsed arguments
 *
 * Return: number of arguments
 */
int parses_input(char *input, char **args)
{
    int args_count = 0;
    char *arguments = strtok(input, " \t\n");
    char *special_char = NULL;

    while (arguments != NULL && args_count < MAX_ARGS)
    {
        if ((special_char = strchr(arguments, '>')) != NULL ||
            (special_char = strchr(arguments, '<')) != NULL ||
            (special_char = strchr(arguments, '|')) != NULL ||
            (special_char = strchr(arguments, '&')) != NULL)
        {
            /* special character found */
            args[args_count++] = special_char;
            *special_char = '\0'; /* terminate the argument before the special character */
        }

        args[args_count++] = arguments;
        arguments = strtok(NULL, " \t\n");
    }

    args[args_count] = NULL;

    return (args_count);
}

/**
 * command_exists - checks if the specified command exists in the system PATH
 * @command: the command to search for in the PATH
 * @path: the system PATH environment variable
 *
 * Return: a dynamically allocated string containing the path to the command
 *         NULL otherwise
 */

char *command_exists(char *command, char *path)
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
				return (strdup(command_path));
			}
			path_token = strtok(NULL, ":");
		}
	}
	return (NULL);
}

/**
* executes_command - function executes the
* command specified by the user.
* @args: array of string containing command its arguments
* @command_path: string containing the PATH environment variable
*
*/
int executes_command(char **args, char *command_path)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return (EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                fprintf(stderr, "cd: missing argument\n");
                return (EXIT_FAILURE);
            }
            if (chdir(args[1]) != 0)
            {
                perror("chdir");
                return (EXIT_FAILURE);
            }
        }
        else if (strcmp(args[0], "echo") == 0)
        {
		int i;

            for (i = 1; args[i] != NULL; i++)
            {
                printf("%s ", args[i]);
            }
            printf("\n");
        }
        else
        {
            if (execve(command_path, args, environ) == -1)
            {
                perror("execve");
                return (EXIT_FAILURE);
            }
        }
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            return (WEXITSTATUS(status));
        }
    }

    return (EXIT_FAILURE);
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
    int args_count = 0;
    char *command_path = NULL;

    while (1)
    {
        printf("$");
        fflush(stdout);

        if (fgets(input, MAX_INPUT_LEN, stdin) == NULL)
        {
            printf("\n");
            break;
        }
        input[strcspn(input, "\n")] = '\0';

        args_count = parses_input(input, args);

	if (args_count == 0)/*checks if ther are arguments*/
	{
		continue;
	}
        else if (strcmp(args[0], "exit") == 0)
        {
            break;
        }
        else if (strcmp(args[0], "env") == 0)
        {
            print_environ();
            continue;
        }

        command_path = command_exists(args[0], path);

        if (command_path == NULL)
        {
            printf("Command '%s' not found\n", args[0]);
            continue;
        }

        executes_command(args, command_path);
        free(command_path);
    }

    return (0);
}
