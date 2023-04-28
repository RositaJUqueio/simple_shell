#include "shell.h"

/**
 * initializer - starts executing everything
 * @current_command: try to check current token
 * @type_command: parse token
 *
 * Return: void function
 */

void initializer(char **current_command, int type_command)
{
	int status = 0;
	pid_t PID;

	if (type_command == EXT_COMMAND || type_command == PATH_COMMAND)
	{
		PID = fork();
		if (PID == 0)
			execute_command(current_command, type_command);
		else
		{
			waitpid(PID, &status, 0);
			status >>= 8;
		}
	}
	else
		execute_command(current_command, type_command);
}

