#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int rilancia(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Utilizzo: %s <comando> [parametri...]\n", argv[0]);
		return 1;
	}

	int status;
	pid_t child_pid;
	clock_t start_time, end_time;
	double elapsed_time;

	do
	{
		start_time = clock(); // Registra il tempo di inizio
		child_pid = fork();   // Creiamo un nuovo processo figlio
		if (child_pid == -1)
		{
			perror("Errore nella creazione del processo figlio");
			exit(1);
		}
		else if (child_pid == 0)
		{
			// Questo è il processo figlio
			execvp(argv[1], &argv[1]); // Eseguiamo il comando con i relativi parametri
			perror("Errore nell'esecuzione del comando");
			exit(1);
		}
		else
		{
			// Questo è il processo padre
			wait(&status);      // Aspettiamo che il processo figlio termini
			end_time = clock(); // Registra il tempo di fine
			elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

			if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			{
				// Il processo figlio è terminato senza errori
				printf("Il comando è stato eseguito con successo.\n");
				if (elapsed_time < 1.0)
				{
					printf("Tempo di esecuzione inferiore a 1 secondo. Non verrà riattivato.\n");
					break;
				}
			}
			else
			{
				printf("Il comando ha restituito un errore o è stato interrotto da un segnale.\n");
				break;
			}
		}
	} while (1);

	return 0;
}
