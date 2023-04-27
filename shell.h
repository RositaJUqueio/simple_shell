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
int executes_command(char **args, char *command_path);

extern char **environ;

#endif /*SHELL_H*/
