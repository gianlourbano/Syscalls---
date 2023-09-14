#include "ex.h"

#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h> // library for fcntl function
#include <dirent.h>

#define MAX_EVENTS 1024							  /* Maximum number of events to process*/
#define LEN_NAME 16								  /* Assuming that the length of the filename \
							  won't exceed 16 bytes*/
#define EVENT_SIZE (sizeof(struct inotify_event)) /*size of one event*/
#define BUF_LEN (MAX_EVENTS * (EVENT_SIZE + LEN_NAME))
/*buffer to store the data of events*/

int fd, wd;

void sig_handler(int sig)
{

	/* Step 5. Remove the watch descriptor and close the inotify instance*/
	inotify_rm_watch(fd, wd);
	close(fd);
	exit(0);
}

int check_empty_dir(char *path)
{
	DIR *dir = opendir(path);
	struct dirent *entry;
	int count = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		++count;
	}
	closedir(dir);
	return count <= 2;
}

int copy_file(FILE *f1, FILE *f2)
{
	char c;
	if (f1 == NULL || f2 == NULL)
	{
		printf("Error opening file\n");
		return 0;
	}
	c = fgetc(f1);
	while (c != EOF)
	{
		fputc(c, f2);
		c = fgetc(f1);
	}
	return 1;
}

void printBits(size_t const size, void const *const ptr)
{
	unsigned char *b = (unsigned char *)ptr;
	unsigned char byte;
	int i, j;

	for (i = size - 1; i >= 0; i--)
	{
		for (j = 7; j >= 0; j--)
		{
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}
	puts("");
}

int dircat(int argc, char **argv)
{
	if (argc < 3)
	{
		printf("Usage: %s <dir> <file>\n", argv[0]);
		exit(1);
	}

	char *path_to_be_watched;
	signal(SIGINT, sig_handler);

	path_to_be_watched = argv[1];

	if (!check_empty_dir(path_to_be_watched))
	{
		printf("Directory is not empty\n");
		exit(1);
	}

	/* Step 1. Initialize inotify */
	fd = inotify_init();

	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) // error checking for fcntl
		exit(2);

	/* Step 2. Add Watch */
	wd = inotify_add_watch(fd, path_to_be_watched, IN_ALL_EVENTS);

	if (wd == -1)
		printf("Could not watch : %s\n", path_to_be_watched);
	else
		printf("Watching : %s\n", path_to_be_watched);

	while (1)
	{
		int i = 0, length;
		char buffer[BUF_LEN];

		/* Step 3. Read buffer*/
		length = read(fd, buffer, BUF_LEN);

		/* Step 4. Process the events which has occurred */
		while (i < length)
		{

			struct inotify_event *event = (struct inotify_event *)&buffer[i];

			
			if (event->len)
			{
				printf("event->mask: 0x%x\n", event->mask);
				if (event->mask & IN_MODIFY)
				{

					printf("The file %s was moved.\n", event->name);
				}
			}
			i += EVENT_SIZE + event->len;
		}
	}

	return EXIT_SUCCESS;
}