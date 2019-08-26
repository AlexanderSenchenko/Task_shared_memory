#ifndef CONF_H
#define CONF_H

#define CTL_CON 10
#define CTL_DISCON 11
#define CTL_MSG 12

struct clientctl {
	int act;
	char name[16];
};

struct message {
	char name[16];
	char msg[64];
	int size;
};

#endif
