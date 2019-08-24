#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int idshm;
int idsem;

void* shmmem;

void sighandler(int);

void init_server();

void delete_shm();
void delete_sem();

int main()
{
	init_server();

	signal(SIGINT, sighandler);

	while(1);

	return 0;
}

void sighandler(int sig)
{
	delete_sem();

	shmdt(shmmem);
	delete_shm();

	exit(EXIT_SUCCESS);
}

void init_server()
{
	key_t key;

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
}

void delete_shm()
{
	int ret;
	ret = shmctl(idshm, IPC_RMID, NULL);
	if (ret == -1) {
		perror("shmctl");
		exit(EXIT_FAILURE);
	}
}

void delete_sem()
{
	int ret;
	ret = semctl(idsem, 0, IPC_RMID);
	if (ret == -1) {
		perror("semctl");
		exit(EXIT_FAILURE);
	}
}
