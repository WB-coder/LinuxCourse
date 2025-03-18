#ifndef MESSAGE_H
#define MESSAGE_H

void send_message(int id, char *message);
void *receive_messages(void *arg);

#endif