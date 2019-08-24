#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	key_t key;
	int idshm;
	int idsem;
	int ret;
	void* shmmem;
	char* str;
	struct sembuf lock[2] = { {0, 0, 0}, {0, 1, 0} };
	struct sembuf unlock = { 0, -2, 0 };

	key = ftok("Makefile", 'q');

	idshm = shmget(key, 4096, IPC_CREAT | IPC_EXCL | 0666);
	if (idshm == -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}

	idsem = semget(key, 1, IPC_CREAT | 0666);
	if (idsem == -1) {
		perror("semget");
		exit(EXIT_FAILURE);
	}

	shmmem = shmat(idshm, NULL, 0);
	if (shmmem == (void*) -1) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	str = (char*) shmmem;

	semop(idsem, lock, 2);
	
	strcpy(str, "Hello\0");

	semop(idsem, &unlock, 1);

	ret = semctl(idsem, 0, IPC_RMID);
	if (ret == -1) {
		perror("semctl");
	}

	ret = shmdt(shmmem);
	if (ret == -1) {
		perror("shmdt");
	}

	ret = shmctl(idshm, IPC_RMID, NULL);
	if (ret == -1)
		perror("shmctl");

	return 0;
}
