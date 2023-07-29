#include "shell.h"

/**
 * get_program_path - Get the full path of a program in the PATH.
 * @prg_name: The name of the program.
 *
 * Return: The full path of the program or NULL if not found in the PATH.
 */
char *get_program_path(char *prg_name)
{
	struct stat info;
	int paths_count;
	char delim, *path_env_copy, *path_env = get_enval("PATH");
	char *prg_path, *path;

	if (path_env == NULL)
	{
		return (NULL);
	}

	delim = ':';
	paths_count = word_count(path_env, &delim);
	if (paths_count == 0)
	{
		return (NULL);
	}

	path_env_copy = strdup(path_env);
	if (path_env_copy == NULL)
	{
		perror("strdup");
		exit(EXIT_FAILURE);
	}

	path = strtok(path_env_copy, &delim);
	while (path != NULL)
	{
		prg_path = malloc(strlen(path) + strlen(prg_name) + 2);
		if (prg_path == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		sprintf(prg_path, "%s/%s", path, prg_name);
		if (stat(prg_path, &info) == 0)
		{
			free(path_env_copy);
			return (prg_path);
		}

		free(prg_path);
		path = strtok(NULL, &delim);
	}

	free(path_env_copy);
	return (NULL);
}



/**
 * run_shell - Run the shell with interactive or non-interactive mode.
 * @shname: The name of the shell.
 * @is_interactive: Flag to indicate if the
 * shell is running in interactive mode.
 *
 * Return: 0 on success.
 */
int run_shell(char *shname, int is_interactive)
{
	int read_bytes, i = 1, lines_count = 0;
	char buf[MAX_LINE_LENGTH];
	char **lines = NULL, *line = NULL;
	int l = 0;

	if (is_interactive)
	{
		write(STDOUT_FILENO, "$ ", 2);
	}
	while ((read_bytes = read(STDIN_FILENO, buf, MAX_LINE_LENGTH)) > 0)
	{
		if (read_bytes == 1 && is_interactive)
		{
			write(STDOUT_FILENO, "$ ", 2);
			i++;
			continue;
		}
		if (!end_line(buf, read_bytes - 1))
		{
			exit(EXIT_FAILURE);
		}
		lines_count = word_count(buf, "\n");
		if (lines_count > 0)
		{
			lines = malloc(sizeof(char *) * (lines_count + 1));
			if (lines == NULL)
			{
				perror("malloc");
				exit(EXIT_FAILURE);
			}
		}
		line = strtok(buf, "\n");
		while (line != NULL)
		{
			lines[l++] = line;
			line = strtok(NULL, "\n");
		}
		lines[l] = NULL;
		interpret_line(shname, lines, lines_count, is_interactive);
		free(lines);
		if (is_interactive)
		{
			write(STDOUT_FILENO, "$ ", 2);
		}
		i++;
	}
	if (is_interactive)
	{
		write(STDOUT_FILENO, "\n", 1);
	}
	return (0);
}

/**
 * interpret_line - Interpret each line of input and execute the appropriate command.
 * @shname: The name of the shell.
 * @lines: Array of strings representing input lines.
 * @lines_count: The number of input lines.
 * @is_interactive: Flag to indicate if the shell is running in interactive mode.
 *
 * Return: The exit status of the last executed command.
 */
int interpret_line(char *shname, char **lines, int lines_count, int is_interactive)
{
	int status, is_exist = 1, i = 0, exit_status = 0;
	char *prg_path, **argv;

	while (lines[i])
	{
		argv = split_command(lines[i]);
		if (argv != NULL)
		{
			if (is_alias(argv))
			{
				add_alias(argv);
				free(argv);
				continue;
			}

			if (is_builtin(argv, lines, exit_status) == 0)
			{
				if (is_path(argv[0]) == 0)
				{
					prg_path = get_program_path(argv[0]);
					if (prg_path)
					{
						argv[0] = prg_path;
					}
					else
					{
						is_exist = 0;
					}
				}

				if (lines_count > 1)
				{
					if (fork() == 0)
					{
						execute_program(argv, shname, is_interactive, is_exist);
						exit(EXIT_FAILURE);
					}
					else
					{
						wait(&status);
						exit_status = WEXITSTATUS(status);
					}
				}
				else
				{
					if (execute_program(argv, shname, is_interactive, is_exist) == -1)
					{
						free(argv);
						free(lines);
						exit(EXIT_FAILURE);
					}
				}

				free(prg_path);
			}

			free(argv);
		}

		i++;
	}

	return (exit_status);
}

/**
 * execute_program - Execute the specified program with arguments.
 * @argv: Array of strings representing the command and its arguments.
 * @shname: The name of the shell.
 * @is_interactive: Flag to indicate if the shell is running in interactive mode.
 * @is_exist: Flag to indicate if the program exists in the PATH.
 *
 * Return: 0 on success, -1 on failure.
 */
int execute_program(char **argv, char *shname, int is_interactive, int is_exist)
{
	pid_t child_pid = 0;
	int status;

	if (is_interactive && is_exist)
	{
		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Error:");
			exit(EXIT_FAILURE);
		}

		if (child_pid == 0)
		{
			execve(argv[0], argv, environ);
			perror(shname);
			free(argv);
			exit(EXIT_FAILURE);
		}
		else
		{
			if (wait(&status) != -1)
			{
				return (0);
			}
			perror("wait");
		}
	}

	execve(argv[0], argv, environ);
	perror(shname);

	return (-1);
}
