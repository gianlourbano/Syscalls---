#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "ex.h"
#include <fcntl.h>

void check_half_file(const char *filename, off_t start, off_t end, int *result)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	// Imposta l'offset di inizio per il processo figlio
	if (lseek(fd, start, SEEK_SET) == -1)
	{
		perror("lseek");
		exit(EXIT_FAILURE);
	}

	ssize_t bufsize = end - start;
	char *buffer = malloc(bufsize);
	if (buffer == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	ssize_t bytesRead = read(fd, buffer, bufsize);
	if (bytesRead == -1)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}

	// Chiudi il file dopo la lettura
	close(fd);

	// Controlla la metà del file e imposta il risultato
	for (ssize_t i = 0; i < bytesRead; i++)
	{
		if (buffer[i] != buffer[i + bufsize / 2])
		{
			*result = 1;
			break;
		}
	}

	free(buffer);
}

int pcmp(int argc, char **argv) {

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	const char *file1 = argv[1];
	const char *file2 = argv[2];

	int result1 = 0, result2 = 0;

	pid_t pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid1 == 0)
	{
		// Processo figlio 1
		check_half_file(file1, 0, lseek(open(file1, O_RDONLY), 0, SEEK_END) / 2, &result1);
		exit(result1);
	}

	pid_t pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid2 == 0)
	{
		// Processo figlio 2
		check_half_file(file2, 0, lseek(open(file2, O_RDONLY), 0, SEEK_END) / 2, &result2);
		exit(result2);
	}

	// Processo padre
	int status1, status2;
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);

	if (WEXITSTATUS(status1) || WEXITSTATUS(status2))
	{
		printf("%s %s differ\n", file1, file2);
	}

	return 0;
}
