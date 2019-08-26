#ifndef CONF_H
#define CONF_H

#define CTL_CON 10
#define CTL_DISCON 11

struct clientctl {
	int act;
	char name[16];
};

#endif
