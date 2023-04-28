#include "shell.h"

/**
 * none_interactive_mode - Executes the shell in non-interactive mode.
 *
 * Return: void
 */
void none_interactive_mode(void)
{
	int status = 0;
	char *line = NULL;
	char **commands = NULL;
	char **current_command = NULL;
	int i, type_command = 0;
	size_t n = 0;

	if (!(isatty(STDIN_FILENO)))
	{
		while (getline(&line, &n, stdin) != -1)
		{
			rm_newline(line);
			rm_comment(line);
			commands = tokenizer(line, ";");

			for (i = 0; commands[i] != NULL; i++)
			{
				current_command = tokenizer(commands[i], " ");
				if (current_command[0] == NULL)
				{
					free(current_command);
					break;
				}
				type_command = parse_command(current_command[0]);
				initializer(current_command, type_command);
				free(current_command);
			}
			free(commands);
		}
		free(line);
		exit(status);
	}
}

