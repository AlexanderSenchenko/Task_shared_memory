#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "act.h"
#include "conf.h"

extern int idshm;
extern int idsem;
extern void* shmmem;

void connect(char* name)
{
	struct clientctl* msgcon = (struct clientctl*) shmmem;
	struct sembuf lock[2] = {{0, 0, 0}, {0, 1, 0}};
	struct sembuf unlock = {0, 2, 0};

	semop(idsem, lock, 2); // 0 +1

	msgcon->act = CTL_CON;
	strncpy(msgcon->name, name, 16);

	semop(idsem, &unlock, 1); // 1 +2
}

void disconnect(char* name)
{
	struct clientctl* msgdiscon = (struct clientctl*) shmmem;
	struct sembuf lock[2] = {{0, 0, 0}, {0, 1, 0}};
	struct sembuf unlock = {0, 2, 0};

	semop(idsem, lock, 2);

	msgdiscon->act = CTL_DISCON;
	strncpy(msgdiscon->name, name, 16);

	semop(idsem, &unlock, 1);
}
