#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>

#include "init_server.h"
#include "free_resources.h"
#include "list.h"

extern int idshm;
extern int idsem;
extern void* shmmem;

extern pthread_t tid_waitcl;

extern struct list* head;

void sighandler(int sig)
{
	pthread_cancel(tid_waitcl);

	free_list(head);

	delete_sem();

	shmdt(shmmem);
	delete_shm();

	exit(EXIT_SUCCESS);
}

void init_server()
{
	key_t key;
	struct sembuf lock_sem2 = {1, 1, 0};
	struct sembuf lock_sem4 = {3, 1, 0};

	key = ftok("Makefile", 'q');
	if (key == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	idshm = shmget(key, 4096, IPC_CREAT | 0666);
	if (idshm == -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}

	shmmem = shmat(idshm, NULL, 0);
	if (shmmem == (void*) -1) {
		perror("shmat");
		delete_shm();
		exit(EXIT_FAILURE);
	}

	idsem = semget(key, 5, IPC_CREAT | 0666);
	if (idsem == -1) {
		perror("semget");
		delete_shm();
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, sighandler);

	semop(idsem, &lock_sem2, 1);
	semop(idsem, &lock_sem4, 1);
}
