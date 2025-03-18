#include "../inc/common.h"
#include "../inc/cli.h"
#include "../inc/connection.h"
#include "../inc/message.h"
#include "../inc/utils.h"

void print_help() {
    printf("\nCommands:\n"
           "help                 - Show command list\n"
           "myip                 - Display your IP address\n"
           "myport               - Show listening port\n"
           "connect <ip> <port>  - Connect to another peer\n"
           "list                 - Show active connections\n"
           "terminate <id>       - Terminate connection by ID\n"
           "send <id> <message>  - Send message to a connection\n"
           "exit                 - Close all connections and exit\n\n");
}

void start_cli() {
    char command[BUFFER_SIZE];
    
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    while (1) {
        char *input = readline("> ");
        if (!input) break;
        
        strncpy(command, input, BUFFER_SIZE);
        free(input);
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            printf("Closing all connections and exiting...\n");
            break;
        }
        else if (strcmp(command, "help") == 0) print_help();
        else if (strcmp(command, "myip") == 0) get_myip();
        else if (strcmp(command, "myport") == 0) printf("Listening port: %d\n", listening_port);
        else if (strcmp(command, "list") == 0) list_connections();
        else if (strstr(command, "terminate") == command) {
            int id;
            if (sscanf(command, "terminate %d", &id) == 1) terminate_connection(id);
        }
        else if (strstr(command, "connect") == command) {
            char ip[16];
            int port;
            if (sscanf(command, "connect %15s %d", ip, &port) == 2) connect_to_peer(ip, port);
        }
        else if (strstr(command, "send") == command) {
            int id;
            char message[MAX_MSG_LEN];
            if (sscanf(command, "send %d %99[^\n]", &id, message) == 2) send_message(id, message);
        }
        else printf("Invalid command. Type 'help' for command list.\n");
    }

    exit(0);
}