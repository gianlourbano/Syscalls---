#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "ex.h"
#include <string.h>

#define MAX_CMD_LEN 1024
#define MAX_ARG_LEN 128

#define die(e)                      \
	do                              \
	{                               \
		fprintf(stderr, "%s\n", e); \
		exit(EXIT_FAILURE);         \
	} while (0);

void execute_cmds(char *cmds[], int n)
{
	int fd[2];
	pid_t pid;

	if (n == 0)
	{
		// Esecuzione dell'ultimo comando senza pipe
		
		execl("/bin/sh", "sh", "-c", cmds[0], (char *)0);
		perror("execvp");
		exit(1);
	}

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(1);
	}

	if ((pid = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}

	if (pid == 0)
	{
		// Figlio
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);

		execute_cmds(cmds, n - 1);
	}
	else
	{
		// Genitore
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);

		execl("/bin/sh", "sh", "-c", cmds[n], (char *)0);
		perror("execvp");
		exit(1);
	}
}

int stdin2pipe(int argc, char **argv) {
	char *cmds[MAX_CMD_LEN];
	for(int i = 0; i < MAX_CMD_LEN; i++) {
		cmds[i] = malloc(128);
	}
	char line[MAX_CMD_LEN];
	int n = 0;

	while (fgets(line, sizeof(line), stdin))
	{
		strcpy(cmds[n], line);
		n++;
	}

	execute_cmds(cmds, n - 1);

	return 0;
}