#include "shell.h"

/**
 * end_line - Check if it's the end of the line.
 * @line: The input line.
 * @new_line_pos: The position of the new line character.
 *
 * Return: 1 if end of the line, 0 otherwise.
 */
int end_line(char *line, int new_line_pos)
{
	if (line[new_line_pos] == '\n' || line[new_line_pos] == ' ')
		line[new_line_pos] = '\0';
	else
		return (0);
	return (1);
}

/**
 * split_command - Split the command into individual arguments.
 * @cmd: The input command.
 *
 * Return: Array of strings containing the individual arguments.
 */
char **split_command(char *cmd)
{
	int i = 0;
	char *token, **words;
	char delim[3] = {'\t', ' ', '\n'};
	int index = word_count(cmd, delim);

	if (index == 0)
		return (NULL);

	words = malloc(sizeof(char *) * (index + 1));
	if (words == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	token = strtok(cmd, delim);
	while (token != NULL)
	{
		words[i++] = token;
		token = strtok(NULL, delim);
	}
	words[i] = NULL;

	return (words);
}

/**
 * word_count - Count the number of words in a string using a delimiter.
 * @str: The input string.
 * @delim: The delimiter used to separate words.
 *
 * Return: The number of words in the string.
 */
int word_count(char *str, char delim[])
{
	int count = 0;
	char *word, *new_str = strdup(str);

	if (new_str == NULL)
	{
		perror("strdup");
		exit(EXIT_FAILURE);
	}

	word = strtok(new_str, delim);
	while (word != NULL)
	{
		count++;
		word = strtok(NULL, delim);
	}

	free(new_str);
	return (count);
}

/**
 * get_enval - Get the value of an environment variable by name.
 * @env_name: The name of the environment variable.
 *
 * Return: The value of the environment variable or NULL if not found.
 */
char *get_enval(char *env_name)
{
	char **env_var = environ;
	size_t name_len = strlen(env_name);

	while (*env_var)
	{
		if (strncmp(env_name, *env_var, name_len) == 0 && (*env_var)[name_len] == '=')
		{
			return ((*env_var) + name_len + 1);
		}
		env_var++;
	}

	return (NULL);
}

/**
 * is_path - Check if the program is in the PATH.
 * @prg_name: The name of the program.
 *
 * Return: 1 if the program is in the PATH, 0 otherwise.
 */
int is_path(char *prg_name)
{
	while (*prg_name)
	{
		if (*prg_name == '/')
		{
			return (1);
		}
		prg_name++;
	}
	return (0);
}

