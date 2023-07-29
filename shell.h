#ifndef SHELL_H
#define SHELL_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_LINE_LENGTH 1024
#define INTERACTIVE 1
#define NOT_INTERACTIVE 0

extern char **environ;

int run_shell(char *shname, int is_interactive);
int interpret_line(char *shname, char **lines, int lines_count, int is_interactive);
int is_builtin(char **argv, char **lines, int *exit_status);
void exit_shell(int exit_status, char **argv);
int print_envs(void);
int execute_program(char **argv, char *shname, int is_interactive, int is_exist);
int end_line(char *line, int new_line_pos);
char **split_command(char *cmd);
int word_count(char *cmd, char delim[]);
int is_path(char *prg_name);
char *get_program_path(char *prg_name);
int is_alias(char **argv);
void add_alias(char **argv);
void print_aliases(void);

#endif

