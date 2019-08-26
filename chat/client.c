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
	int ret;
	pthread_t tid;

	if (argc != 2) {
		printf("Name not specified\n");
		return -1;
	}

	init_client();

	#if 0
	ret = pthread_create(&tid, NULL, msg_wait, NULL);
	if (ret != 0) {
		perror("pthread_create");
		disconnect(argv[1]);
		exit(EXIT_FAILURE);
	}
	#endif

	connect(argv[1]);

	init_chat(&win_text, &win_user, &win_msg);

	char buf[64];

	while ((act = wgetch(stdscr)) != KEY_F(10)) {
		switch (act) {
			case 10: // Enter
				send_message(buf, argv[1]);
				break;
			case KEY_BACKSPACE:
				del_sym(buf);
				break;
			default:
				add_sym(buf, act);
				break;
		}
	}

	// pthread_cancel(tid);

	disconnect(argv[1]);

	endwin();

	return 0;
}

