#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void);
void print_environ(void);
int parses_input(char *input, char **args);
char *command_exists(char *command, char *path);
void command_execution(char *command_path, char **args);

extern char **environ_var;

#endif /*SHELL_H*/
