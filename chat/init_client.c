#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <stdio.h>
#include <stdlib.h>

#include "init_client.h"

extern int idshm;
extern int idsem;
extern void* shmmem;

void init_client()
{
	key_t key;

	key = ftok("Makefile", 'q');
	if (key == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	idshm = shmget(key, 0, 0);
	if (idshm == -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}

	idsem = semget(key, 0, 0);
	if (idsem == -1) {
		perror("semget");
		exit(EXIT_FAILURE);
	}

	shmmem = shmat(idshm, NULL, 0);
	if (shmmem == (void*) -1) {
		perror("shmmem");
		exit(EXIT_FAILURE);
	}
}
