#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <stdio.h>
#include <stdlib.h>

#include "init_server.h"
#include "client_manager.h"

int idshm;
int idsem;
void* shmmem;

int main()
{
	init_server();

	client_manager();

	return 0;
}

