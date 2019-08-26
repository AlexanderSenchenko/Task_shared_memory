PTHREAD := -lpthread
NCURSES_CFLAGS := -D_GNU_SOURSE -I/usr/include/ncursesw
NCURSES_LIBS := -lncursesw -ltinfo

all:
	gcc server.c -c -g
	gcc init_server.c -c -g
	gcc free_resources.c -c -g
	gcc client_manager.c -c -g
	gcc wait_client.c -c -g
	gcc list.c -c -g
	gcc server.o init_server.o free_resources.o client_manager.o wait_client.o list.o -o server $(PTHREAD)
	gcc client.c -c -g $(NCURSES_CFLAGS)
	gcc init_client.c -c -g
	gcc act.c -c -g $(NCURSES_CFLAGS)
	gcc init_chat.c -c -g $(NCURSES_CFLAGS)
	gcc msg_wait.c -c -g $(NCURSES_CFLAGS) 
	gcc client.o init_client.o act.o list.o init_chat.o msg_wait.o -o client $(NCURSES_LIBS) $(PTHREAD)
