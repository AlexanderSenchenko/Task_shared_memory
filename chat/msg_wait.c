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

extern WINDOW* win_text;
extern WINDOW* win_user;
extern WINDOW* win_msg;

void* msg_wait(void* ptr)
{
	char name[16];
	struct clientctl* msgcon = (struct clientctl*) shmmem;

	struct sembuf wait_unlock_sem2 = {1, 0, 0};
	struct sembuf lock_sem3 = {2, 1, 0};
	struct sembuf wait_unlock_sem4 = {3, 0, 0};

	semop(idsem, &wait_unlock_sem2, 1); // ожидание 1 разрешения
	semop(idsem, &lock_sem3, 1);
	semop(idsem, &wait_unlock_sem4, 1); // ожидание 2 разрешения

	strncpy(name, msgcon->name, 16);

	semop(idsem, &lock_sem3, 1);

	wmove(win_user, 0, 0);
	wprintw(win_user, "%s", name);
	wrefresh(win_user);
}
