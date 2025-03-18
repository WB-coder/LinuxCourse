#include "../inc/common.h"

// Define global variables
Connection connections[MAX_CLIENTS];
int connection_count = 0;
int server_socket = -1;
int listening_port = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;