#ifndef CONNECTION_H
#define CONNECTION_H

void init_server(int port);
void list_connections();
void connect_to_peer(char *ip, int port);
void terminate_connection(int id);
void cleanup_connections();

#endif