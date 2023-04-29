#include "shell.h"
#define BUFFER_SIZE 1024

/**
 * custom_getline - reads input from stdin.
 *
 * Return: dynamically allocated string.
 */
char *custom_getline(void)
{
	static char buffer[BUFFER_SIZE];
	static int buffer_index;
	static int bytes_read;
	char *input_line = NULL;
	int line_index = 0;

	while (1)
	{
		/*If buffer is empty, read more input*/
		if (buffer_index >= bytes_read)
		{
			bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
			buffer_index = 0;
			if (bytes_read <= 0)
			{
				break;
			}
		}
		/*Copy chars from buffer to line until newline or end of buffer*/
		while (buffer_index < bytes_read)
		{
			if (buffer[buffer_index] == '\n')
			{
				buffer_index++;
				input_line[line_index] = '\0';
				return (input_line);
			}
			input_line = realloc(input_line, line_index + 1);
			input_line[line_index] = buffer[buffer_index];
			line_index++;
			buffer_index++;
		}
	}
	/*If no input left, return NULL*/
	if (input_line != NULL)
	{
		input_line[line_index] = '\0';
	}
	return (input_line);
}
