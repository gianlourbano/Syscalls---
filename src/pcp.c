#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "ex.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>

#define assert(cond, msg) \
	if(!(cond)) { \
		perror(msg); \
		return EXIT_FAILURE; \
	} \

int copy_file_seg(int fd1, int fd2, ssize_t start, ssize_t end, int i)
{
	printf("start: %ld, end: %ld, part: %d\n", start, end, i);
	assert(lseek(fd1, start, SEEK_SET) != -1, "lseek");

	char buf;
	ssize_t nread;
	ssize_t total = 0;

	while ((total < (end - start)) && (nread = read(fd1, &buf, 1)) > 0)
	{
		lseek(fd2, start + total, SEEK_SET);
		if (write(fd2, &buf, nread) == -1)
		{
			perror("write");
			return EXIT_FAILURE;
		}	
		total += nread;
	}
	free(buf);

	if (nread == -1)
	{
		perror("read");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/**
 * 	Fare un programma di copia parallelo di file.
	pcp file1 file2
	pcp deve creare due processi figli; mentre un processo copia la prima meta’ del file, l'altro deve
	copiare l’altra meta’
*/
int pcp(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "Uso: %s <file1> <file2>\n", argv[0]);
		return EXIT_FAILURE;
	}

	int opt;
	int i;
	int parallelism_level = 2;
	while ((opt = getopt(argc, argv, "j:")) != -1)
	{
		switch (opt)
		{
		case 'j':
			parallelism_level = atoi(optarg);
			break;
		case '?':
			fprintf(stderr, "Usage: %s [-j] arg]\n", argv[0]);
			return EXIT_FAILURE;
		}
	}
	printf("parallelism_level: %d\n", parallelism_level);

	char *file1 = argv[optind];
	char *file2 = argv[optind + 1];

	int fd1 = open(file1, O_RDONLY);
	if (fd1 == -1) {
		perror(file1);
		return EXIT_FAILURE;
	}

	int fd2 = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd2 == -1) {
		perror(file2);
		return EXIT_FAILURE;
	}
	

	off_t size = lseek(fd1, 0, SEEK_END);
	if (size == -1) {
		perror("lseek");
		return EXIT_FAILURE;
	}
	lseek(fd2, size, SEEK_SET);
	lseek(fd2, 0, SEEK_SET);
	printf("size: %ld\n", size);

	off_t chunk = size / parallelism_level;

	int j;
	for(j = 0; j < parallelism_level ; j++) {
		pid_t pid = fork();
		if(!pid) {
			return copy_file_seg(fd1, fd2, j*chunk, (j+1)*chunk, j);
		} else if (pid == -1) {
			perror("fork");
			return EXIT_FAILURE;
		}
	}

	for(j = 0; j < parallelism_level ; j++) {
		wait(NULL);
	}

	if((parallelism_level)*chunk < size) {
		copy_file_seg(fd1, fd2, (parallelism_level)*chunk, size, parallelism_level);
	}

	if (close(fd1) == -1) {
		perror("close");
		return EXIT_FAILURE;
	}

	if (close(fd2) == -1) {
		perror("close");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}