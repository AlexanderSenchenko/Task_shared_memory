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
#include <pthread.h>

#include "init_client.h"
#include "act.h"
#include "init_chat.h"
#include "msg_wait.h"

int idshm;
int idsem;
void* shmmem;

WINDOW* win_user;
WINDOW* win_text;
WINDOW* win_msg;

int main(int argc, char** argv)
{
	int act;
	pthread_t tid;

	if (argc != 2) {
		printf("Name not specified\n");
		return -1;
	}

	init_client();

	connect(argv[1]);

	init_chat(&win_text, &win_user, &win_msg);

	// pthread_create(&tid, NULL, msg_wait, NULL);

	while ((act = wgetch(stdscr)) != KEY_F(10)) {

	}

	pthread_cancel(tid);

	disconnect(argv[1]);

	endwin();

	return 0;
}

