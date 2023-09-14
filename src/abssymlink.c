#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>

int abssymlink(int argc, char **argv) {
	if(argc < 2) {
		fprintf(stderr, "Utilizzo: %s <directory>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *dir = argv[1];

	struct dirent* dir_entry;

	DIR* dir_stream;

	if((dir_stream = opendir(dir)) == NULL) {
		perror("opendir");
		return EXIT_FAILURE;
	}

	while((dir_entry = readdir(dir_stream)) != NULL) {
		char *path = malloc(strlen(dir) + strlen(dir_entry->d_name) + 2);
		sprintf(path, "%s/%s", dir, dir_entry->d_name);

		struct stat statbuf;
		if(lstat(path, &statbuf) == -1) {
			perror("lstat");
			return EXIT_FAILURE;
		}

		if(S_ISLNK(statbuf.st_mode)) {
			char *abspath = realpath(path, NULL);
			unlink(path);
			symlink(abspath, path);
			free(abspath);
		}

		free(path);
	}

	return EXIT_SUCCESS;
}
