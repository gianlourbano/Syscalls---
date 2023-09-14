#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "ex.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int fifotext(int argc, char **argv) {
	if(argc < 2) {
		printf("Usage: %s <pathname>\n", argv[0]);
	}

	const char* pipe_path = argv[1];

	if (mkfifo(pipe_path, 0666) == -1)
	{
		perror("mkfifo");
	}

	int fd;
	char buffer[1024];

	while (1)
	{
		fd = open(pipe_path, O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			continue;
		}

		ssize_t bytes_read;
		while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
		{
			buffer[bytes_read] = '\0';
			if (strcmp(buffer, "FINE\n") == 0)
			{
				close(fd);
				unlink(pipe_path); // Remove the FIFO
				return;
			}
			printf("%s", buffer);
		}

		close(fd);
	}

	return EXIT_SUCCESS;
}

int fifosig(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <pathname>\n", argv[0]);
	}

	const char *pipe_path = argv[1];

	if (mkfifo(pipe_path, 0666) == -1)
	{
		perror("mkfifo");
	}

	int fd;
	char buffer[1024];

	while (1)
	{
		fd = open(pipe_path, O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			continue;
		}

		ssize_t bytes_read;
		while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
		{
			buffer[bytes_read] = '\0';
			if (strcmp(buffer, "FINE\n") == 0)
			{
				close(fd);
				unlink(pipe_path); // Remove the FIFO
				return;
			}

			int pid, sig;
			if(sscanf(buffer, "%d %d", &pid, &sig) == 2) {
				if(kill(pid, sig) == -1) {
					perror("kill");
				} else {
					printf("Sent signal %d to process %d\n", sig, pid);
				}
			} else {
				printf("Invalid input: %s\n", buffer);
			}
		}

		close(fd);
	}

	return EXIT_SUCCESS;
}