#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	key_t key;
	int idshm;
	int idsem;
	int ret;
	void* shmmem;
	char* ptr;
	char str[10];
	struct sembuf lock[2] = { {0, -1, 0}, { 0, 0, 0} };
	struct sembuf unlock[2] = { {0, 0, 0}, {0, 2, 0} };

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

	shmmem = shmat(idshm, NULL, SHM_RDONLY);
	if (shmmem == (void*) -1) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	ptr = (char*) shmmem;

	semop(idsem, lock, 2);

	strncpy(str, ptr, 10);

	semop(idsem, unlock, 2);

	printf("%s\n", str);

	ret = shmdt(shmmem);
	if (ret == -1) {
		perror("shmdt");
		exit(EXIT_FAILURE);
	}

	return 0;
}
