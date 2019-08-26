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

#include "act.h"
#include "conf.h"

extern int idshm;
extern int idsem;
extern void* shmmem;

extern WINDOW* win_text;
extern WINDOW* win_user;
extern WINDOW* win_msg;

int size_buf = 0;
int text_row = 0;

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

void send_message(char* buf, char* name)
{
	struct message* msg;
	struct clientctl* ctl_msg;
	struct sembuf lock_sem1[2] = {{0, 0, 0}, {0, 1, 0}};
	struct sembuf unlock_sem1 = {0, 2, 0};
	struct sembuf lock_sem5[2] = {{4, 0, 0}, {4, 1, 0}};
	struct sembuf unlock_sem5 = {4, 2, 0};

	buf[size_buf++] = '\0';

	semop(idsem, lock_sem1, 2);
	
	ctl_msg = (struct clientctl*) shmmem;
	ctl_msg->act = CTL_MSG;

	semop(idsem, lock_sem5, 2);

	#if 1
	msg = (struct message*) (ctl_msg + 1);
	strncpy(msg->name, name, 16);
	strncpy(msg->msg, buf, size_buf);
	msg->size = size_buf;
	#endif

	semop(idsem, &unlock_sem5, 1);
	semop(idsem, &unlock_sem1, 1);

	wmove(win_text, text_row, 0);
	wprintw(win_text, "(ure name): %s", buf);
	wrefresh(win_text);

	text_row++;

	wmove(win_msg, 0, 0);
	whline(win_msg, ' ', size_buf);
	wrefresh(win_msg);

	memset(buf, 0, size_buf);
	size_buf = 0;
}

void add_sym(char* buf, int ch)
{
	if (size_buf == 62)
		return;

	wmove(win_msg, 0, size_buf);
	wprintw(win_msg, "%c", ch);
	wrefresh(win_msg);

	buf[size_buf++] = ch;
}

void del_sym(char* buf)
{
	if (size_buf == 62)
		return;

	wmove(win_msg, 0, size_buf - 1);
	wprintw(win_msg, "%c", ' ');
	wrefresh(win_msg);

	buf[size_buf--] = 0;
}
