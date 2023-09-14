#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "ex.h"
#include <signal.h>
#include <sys/types.h>


void recv_handler(int sig, siginfo_t *info, void *ucontext) {
	long msg_long = (long)info->si_value.sival_ptr;
	if (msg_long == 0)
		printf("\n");
	else
	{
		char msg;
		for (int i = 0, last = 0; i < 8; i++)
		{
			last = msg_long % 256;
			msg_long = msg_long >> 8;
			printf("%c", last);
		}
	}
	union sigval v;
	sigqueue(info->si_pid, SIGUSR1, v);
}

void sisegvhandler(int sig) {
	printf("SIGSEGV\n");
	exit(1);
}

int rx(int argc, char **argv) {

	printf("%d\n", getpid());
	struct sigaction act;
	act.sa_sigaction = recv_handler;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	while (1)
		sleep(20);

	return EXIT_SUCCESS;
}