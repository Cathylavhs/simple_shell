#include "shell.h"

/**
 * change_directory - Change the current working directory of the shell process.
 * @argv: Array of strings representing the command and its arguments.
 * Return:
 * 0 on success (when the directory is successfully changed).
 * -1 on failure (when the specified path is not a valid directory).
 */
int change_directory(char **argv)
{
	if (argv[1] == NULL)
	{
		char *home_dir = get_enval("HOME");
		if (home_dir != NULL)
		{
			if (chdir(home_dir) != 0)
			{
				perror("cd");
				return (-1);
			}
		}
		else
		{
			printf("cd: HOME not set\n");
			return (-1);
		}
	}
	else
	{
		if (chdir(argv[1]) != 0)
		{
			perror("cd");
			return (-1);
		}
	}

	return (0);
}


/**
 * is_builtin - Check if the command is a built-in command.
 * @argv: Array of strings representing the command and its arguments.
 * @lines: Array of strings representing the input lines.
 * @exit_status: The exit status to be used when exiting the shell.
 * 
 * Return: 1 if the command is a built-in command, 0 otherwise.
 */
int is_builtin(char **argv, char **lines, int *exit_status)
{
	char *cmd = argv[0];

	if (strcmp(cmd, "exit") == 0)
	{
		free(lines);
		exit_shell(*exit_status, argv);
	}
	else if (strcmp(cmd, "env") == 0)
	{
		print_envs();
		return (1);
	}
	else if (strcmp(cmd, "alias") == 0)
	{
		print_aliases();
		return (1);
	}
	else if (strcmp(cmd, "cd") == 0)
	{
		if (change_directory(argv) == -1)
		{
			// Update the exit status for the parent process
			*exit_status = -1;
		}
		return (1);
	}

	return (0);
}
