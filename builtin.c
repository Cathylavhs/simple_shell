#include "shell.h"

/**
 * print_envs - Print environment variables.
 * 
 * Return: 1 on success, -1 on failure.
 */
int print_envs(void)
{
	char **env_var = environ;

	while (*env_var)
	{
		printf("%s\n", *env_var);
		env_var++;
	}
	return (1);
}

/**
 * exit_shell - Exit the shell.
 * @exit_status: The exit status to be used when exiting the shell.
 * @argv: Array of strings representing command and its arguments.
 */
void exit_shell(int exit_status, char **argv)
{
	free(argv);
	exit(exit_status);
}


/**
 * is_alias - Check if the command is an alias.
 * @argv: Array of strings representing the command and its arguments.
 * 
 * Return: 1 if the command is an alias, 0 otherwise.
 */
int is_alias(char **argv)
{
	/*Check if the first argument is "alias"*/
	return (strcmp(argv[0], "alias") == 0);
}

/**
 * add_alias - Add an alias to the list of aliases.
 * @argv: Array of strings representing the command and its arguments.
 */
void add_alias(char **argv)
{
	char alias_str[MAX_LINE_LENGTH];

	if (argv[1] == NULL || argv[2] == NULL)
	{
		printf("Usage: alias <name> <command>\n");
		return;
	}

	/*Save the alias in a custom format, e.g., "alias_name=command"*/
	snprintf(alias_str, MAX_LINE_LENGTH, "alias_%s=%s", argv[1], argv[2]);

	/* Store the alias as an environment variable*/
	if (setenv(alias_str, argv[2], 1) != 0)
	{
		perror("setenv");
	}
}

/**
 * print_aliases - Print all the defined aliases.
 */
void print_aliases(void)
{
	char **env_var = environ;

	while (*env_var)
	{
		if (strncmp(*env_var, "alias_", 6) == 0)
		{
			printf("%s\n", *env_var + 6);
		}
		env_var++;
	}
}
