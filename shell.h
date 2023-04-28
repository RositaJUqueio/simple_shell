#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>

/*constants for commands*/
#define EXT_COMMAND 1 /*external*/
#define INT_COMMAND 2  /*internal*/
#define PATH_COMMAND 3
#define INVALID_COMMAND -1

/**
 *struct map - a struct that maps a command name to a function
 *
 *@command_name: name of the command
 *@func: the function that executes the command
 */
typedef struct map
{
	char *command_name;
	void (*func)(char **command);
} function_map;

extern char **environ;
extern char *line;
extern char **commands;
extern char *shell_name;
extern int status;

#define min(x, y) (((x) < (y)) ? (x) : (y))

extern char **commands;
extern char *line;
extern char *shell_name;
extern int status;

/*old and new functions*/
void print(char *, int);
char **tokenizer(char *, char *);
void rm_newline(char *);
int _strlen(char *);
void _strcpy(char *, char *);
int _strcmp(char *, char *);
char *_strcat(char *, char *);
int _strspn(char *, char *);
int _strcspn(char *, char *);
char *_strchr(char *, char);


/*more functions / prototypes*/
char *_strtok_r(char *, char *, char **);
int _atoi(char *);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
void handles_ctrl_c(int);
void rm_comment(char *input);
int parse_command(char *);
void execute_command(char **, int);
char *check_path(char *);
void (*get_func(char *))(char **);
char *_getenv(char *);
extern char **environ;

/*implementing builtins*/
void env(char **);
void quit(char **);

/*shell entry point*/
extern void none_interactive_mode(void);
extern void initializer(char **current_command, int type_command);

#endif /*SHELL_H*/
