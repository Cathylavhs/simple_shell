#include "shell.h"

/**
 * main - Entry point of the shell program.
 * @ac: The number of command-line arguments.
 * @av: Array of strings representing the command-line arguments.
 * Return: 0 on successful shell execution.
 */
int main(__attribute__((unused)) int ac, char **av)
{
	if (isatty(STDIN_FILENO))
	{
		run_shell(av[0], INTERACTIVE);
	}
	else
	{
		run_shell(av[0], NOT_INTERACTIVE);
	}

	return (0);
}

