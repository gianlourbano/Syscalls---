#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PATH_MAX 1024

ino_t in_count;
void __search__link(char *filename, char *directory, char *path)
{
	struct stat st;

	if (lstat(path, &st) == -1)
	{
		perror("lstat");
		exit(EXIT_FAILURE);
	}

	if (S_ISLNK(st.st_mode))
	{
		char target[PATH_MAX];
		ssize_t len = readlink(path, target, sizeof(target) - 1);
		if (len != -1)
		{
			target[len] = '\0';
			if (strcmp(target, filename) == 0)
			{
				printf("symlink %s\n", path);
			}
		}
	}
	else if (S_ISREG(st.st_mode) && st.st_ino == in_count && strcmp(path, filename) != 0)
	{
		printf("link %s\n", path);
	}
	else if (S_ISDIR(st.st_mode))
	{
		DIR *dir;
		struct dirent *entry;
		if ((dir = opendir(path)) == NULL)
		{
			perror("opendir");
			exit(EXIT_FAILURE);
		}

		while ((entry = readdir(dir)) != NULL)
		{
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
			{
				char newpath[PATH_MAX];
				snprintf(newpath, sizeof(newpath), "%s/%s", path, entry->d_name);
				__search__link(filename, directory, newpath);
			}
		}
		closedir(dir);
	}
}

int searchlink(int argc, char **argv)
{

	if (argc < 3)
	{
		fprintf(stderr, "Utilizzo: %s <filename> <dir>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char *filename = argv[1];
	char *dir = argv[2];

	struct stat filename_stat;
	lstat(filename, &filename_stat);
	in_count = filename_stat.st_ino;

	__search__link(filename, dir, dir);

	return EXIT_SUCCESS;
}
