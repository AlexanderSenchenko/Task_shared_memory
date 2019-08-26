#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wait_client.h"
#include "conf.h"
#include "list.h"

extern int idshm;
extern int idsem;
extern void* shmmem;
extern struct list* head;

void* wait_client(void* ptr)
{
	struct clientctl* ptr1 = (struct clientctl*) shmmem;

	struct sembuf lock = {0, -2, 0};
	struct sembuf unlock = {0, -1, 0};

	struct list* node;

	int count_node;

	while (1) {
		semop(idsem, &lock, 1); // 3 -2

		switch (ptr1->act) {
			case CTL_CON:
				// count_node = get_count_node();

				add_node(&head, ptr1->name, -1);

				// count_node = get_count_node();

				// if (count_node != 0)
				//	sig_add_new_user(count_node);
				break;
			case CTL_DISCON:
				del_node_name(&head, ptr1->name);

				count_node = get_count_node();
				break;
			case CTL_MSG:
				recive_message();
				break;
			default:
				break;
		}

		semop(idsem, &unlock, 1); // 1 -1

		count_node = get_count_node();

		printf("List users(%d):\n", count_node);
		node = head;
		while (node != NULL) {
			printf("%s\n", node->name);
			node = node->next;
		}
	}
}

void sig_add_new_user(int count_node)
{
	struct sembuf unlock_sem2 = {1, -1, 0};
	struct sembuf lock_sem2 = {1, 1, 0};
	struct sembuf wait_unlock_sem3 = {2, -count_node, 0};
	struct sembuf unlock_sem4 = {3, -1, 0};
	struct sembuf lock_sem4 = {3, 1, 0};

	semop(idsem, &unlock_sem2, 1); // говорю пользователям,
				       // что можно начать добавление нового пользователя
	semop(idsem, &wait_unlock_sem3, 1); // жду когда все пользователи будут готовы
	semop(idsem, &lock_sem2, 1); // блокирую семафор,
				     // чтобы пользоваатель не добавил
				     // одного и тогоже клиента дважды
	semop(idsem, &unlock_sem4, 1); // говорю, что можно начать чтение
	semop(idsem, &wait_unlock_sem3, 1); // жду пока все дочитают
	semop(idsem, &lock_sem4, 1); // блокирю семафор

	printf("End read new user\n");
}

void recive_message()
{
	struct clientctl* ptr = (struct clientctl*) shmmem;
	struct message* msg = (struct message*) (ptr + 1);
	char name[16];
	char buf[64];
	struct sembuf lock_sem5 = {4, -2, 0};
	struct sembuf unlock_sem5 = {4, -1, 0};

	semop(idsem, &lock_sem5, 1);

	#if 1
	strncpy(name, msg->name, 16);
	strncpy(buf, msg->msg, msg->size);
	#endif

	semop(idsem, &unlock_sem5, 1);

	printf("%s: %s\n", name, buf);
}
