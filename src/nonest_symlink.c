#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

int nonest_symlink(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Usage: %s <directory-path>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	struct dirent *dir_entry;
	DIR *dir = opendir(argv[1]);

	if(dir == NULL) {
		perror("Directory cannot be opened or does not exist\n");
		exit(EXIT_FAILURE);
	}

	while((dir_entry = readdir(dir)) != NULL) {
		if(dir_entry->d_type == 0) {
			printf("Found symbolic link: %s\n", dir_entry->d_name);
		}
	}

	closedir(dir);

	return EXIT_SUCCESS;
}
