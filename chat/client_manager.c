#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "client_manager.h"
#include "wait_client.h"
#include "list.h"

extern int idshm;
extern int idsem;
extern void* shmmem;

pthread_t tid_waitcl;

struct list* head;

void client_manager()
{
	// pthread_create(&tid_waitcl, NULL, wait_client, NULL);

	while(1) {
		wait_client(NULL);
	}
}
