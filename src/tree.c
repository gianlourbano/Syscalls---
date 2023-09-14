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

int print_dir(int dfd, DIR* d)
{
	struct stat statbuf;
	struct dirent *dp;
	int ffd;
	while ((dp = readdir(d)) != NULL)
	{
		if (dp->d_name[0] == '.')
			continue;
		/* there is a possible race condition here as the file
		 * could be renamed between the readdir and the open */
		if ((ffd = openat(dfd, dp->d_name, O_RDONLY)) == -1)
		{
			perror(dp->d_name);
			continue;
		}

		fstat(ffd, &statbuf);

		if (S_ISDIR(statbuf.st_mode))
		{
			printf("%s\n", dp->d_name);
			print_dir(ffd, d);
		}
		else
		{
			printf("%s\n", dp->d_name);
		}
		close(ffd);
	}
	closedir(d); // note this implicitly closes dfd
	return 0;
}

int tree(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <pathname>\n", argv[0]);
	}

	
	const char *path = argv[1];

	DIR* d;
	int dfd;
	if ((d = fdopendir((dfd = open(path, O_RDONLY)))) == NULL)
	{
		fprintf(stderr, "Cannot open %s directory\n", path);
		exit(1);
	}

	print_dir(dfd, d);

	return EXIT_SUCCESS;
}