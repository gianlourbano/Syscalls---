#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>

void sig_handler_fifosig(int signo)
{
    printf("Received signal %d\n", signo);
    exit(0);
}

int fifosig_test(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <pipe_path>\n", argv[0]);
        return 1;
    }

    const char *pipe_path = argv[1];

    signal(SIGUSR1, sig_handler_fifosig);

    int fd = open(pipe_path, O_WRONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    int pid = getpid();
    dprintf(fd, "%d %d\n", pid, SIGUSR1);
    close(fd);

    while(1);

    return 0;
}