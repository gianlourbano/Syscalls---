#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "ex.h"
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

int lancia(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Uso: %s <libso> [argomenti...]\n", argv[0]);
		return EXIT_FAILURE;
	}
	char* path = argv[1];

	if (path == NULL) {
		fprintf(stderr, "Errore: %s non trovato\n", argv[1]);
		return EXIT_FAILURE;
	}

	void* handle = dlopen(path, RTLD_LAZY);
	if(handle == NULL) {
		fputs(dlerror(), stderr);
		return EXIT_FAILURE;
	}

	int (*fun)(int, char**) = dlsym(handle, "main");
	if(fun == NULL) {
		fputs(dlerror(), stderr);
		return EXIT_FAILURE;
	}

	int ret = fun(argc - 2, argv + 2);
	printf("main returned %d\n", ret);


	return EXIT_SUCCESS;
}

int autolancia(int argc, char **argv) {
	if (argc < 2)
	{
		fprintf(stderr, "Uso: %s <libso> [argomenti...]\n", argv[0]);
		return EXIT_FAILURE;
	}
	char *path = argv[1];

	if (path == NULL)
	{
		fprintf(stderr, "Errore: %s non trovato\n", argv[1]);
		return EXIT_FAILURE;
	}
	
	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return EXIT_FAILURE;
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return EXIT_FAILURE;
	}
	else if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		execlp("file", "file", argv[1], NULL);
		perror("execlp");
		exit(EXIT_FAILURE);
	}
	wait(NULL);

	close(pipefd[1]);

	char output[1024];
	ssize_t n = read(pipefd[0], output, sizeof(output));
	if (n == -1)
	{
		perror("read");
		return EXIT_FAILURE;
	}
	close(pipefd[0]);

	if (strstr(output, "shared object") != NULL)
	{
		void *handle = dlopen(path, RTLD_LAZY);
		if (handle == NULL)
		{
			fputs(dlerror(), stderr);
			return EXIT_FAILURE;
		}

		int (*fun)(int, char **) = dlsym(handle, "main");
		if (fun == NULL)
		{
			fputs(dlerror(), stderr);
			return EXIT_FAILURE;
		}

		int ret = fun(argc - 1, argv + 1);
		printf("main returned %d\n", ret);
	}
	else if(strstr(output, "executable") != NULL)
	{
		execvp(path, argv + 1);
	}

	return EXIT_SUCCESS;
}