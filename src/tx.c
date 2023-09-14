#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "ex.h"
#include <signal.h>

int tx(int argc, char **argv) {
	if(argc != 3) {
		printf("Usage: %s <pid> <msg>\n", argv[0]);
	}

	int rxpid = atoi(argv[1]),
		base = 0,
		i,
		retsig;
	long msg;
	union sigval val;

	sigset_t ret_set;
	sigemptyset(&ret_set);
	sigaddset(&ret_set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &ret_set, NULL);
	do
	{
		msg = 0;
		for (i = base; i < base + 8 && argv[2][i] != 0; i++)
			msg += (long)argv[2][i] << 8 * (i % 8);
		val.sival_ptr = (void *)msg;
		sigqueue(rxpid, SIGUSR1, val);
		sigwait(&ret_set, &retsig);
		base += 8;
	} while (argv[2][i] != 0);
	val.sival_ptr = 0;
	sigqueue(rxpid, SIGUSR1, val);
	return 0;
}