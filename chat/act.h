#ifndef ACT_H
#define ACT_H

void connect(char* name);
void disconnect(char* name);
void send_message(char* buf, char* name);
void add_sym(char* buf, int ch);
void del_sym(char* buf);

#endif
