#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/select.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define MAX_MSG_LEN 100

typedef struct {
    int socket;
    struct sockaddr_in address;
} Connection;

extern Connection connections[MAX_CLIENTS];
extern int connection_count;
extern int server_socket;
extern int listening_port;
extern pthread_mutex_t lock;

#endif