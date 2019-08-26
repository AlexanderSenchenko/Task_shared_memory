#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <termios.h>
#include <sys/ioctl.h>
#include <ncurses.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "msg_wait.h"
#include "conf.h"

extern int idshm;
extern int idsem;
extern void* shmmem;

void* msg_wait(void* ptr)
{
	char name[16];
	struct clientctl* msgcon = (struct clientctl*) shmmem;
	int val;

	struct sembuf wait_unlock_sem2 = {1, 0, 0};
	struct sembuf lock_sem3 = {2, 1, 0};
	struct sembuf wait_unlock_sem4 = {3, 0, 0};

	val = semctl(idsem, 1, GETVAL);
	semop(idsem, &wait_unlock_sem2, 0); // ожидание 1 разрешения
	val = semctl(idsem, 1, GETVAL);
	val = semctl(idsem, 2, GETVAL);
	semop(idsem, &lock_sem3, 0);
	val = semctl(idsem, 2, GETVAL);
	val = semctl(idsem, 3, GETVAL);
	semop(idsem, &wait_unlock_sem4, 0); // ожидание 2 разрешения
	val = semctl(idsem, 3, GETVAL);

	// read new user
	strncpy(name, msgcon->name, 16);
	printf("%s\n", name);

	semop(idsem, &lock_sem3, 0);
	printf("End read\n");
}
