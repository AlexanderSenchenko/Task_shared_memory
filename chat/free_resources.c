#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <stdio.h>
#include <stdlib.h>

#include "free_resources.h"

extern int idshm;
extern int idsem;

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
