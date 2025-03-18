#include "../inc/common.h"
#include "../inc/connection.h"
#include "../inc/utils.h"

void init_server(int port) {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = INADDR_ANY
    };

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);
    listening_port = port;
    pthread_mutex_init(&lock, NULL);
    printf("Server started on port %d\n", port);
}

void list_connections()
{
    pthread_mutex_lock(&lock);
    printf("\nActive Connections:\n");
    for (int i = 0; i < connection_count; i++)
    {
        printf("%d: %s %d\n", i + 1, inet_ntoa(connections[i].address.sin_addr), ntohs(connections[i].address.sin_port));
    }
    pthread_mutex_unlock(&lock);
}

void connect_to_peer(char *ip, int port)
{
    if (connection_count >= MAX_CLIENTS)
    {
        printf("Connection limit reached!\n");
        return;
    }

    // Create client socket and connect to server
    int client_socket = socket(AF_INET, SOCK_STREAM, 0); // TCP (SOCK_STREAM), IPv4 (AF_INET)
    if (client_socket < 0)
    {
        perror("Socket creation failed");
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(client_socket);
        return;
    }

    pthread_mutex_lock(&lock);
    connections[connection_count].socket = client_socket;
    connections[connection_count].address = server_addr;
    connection_count++;
    pthread_mutex_unlock(&lock);

    printf("Connected to %s:%d\n", ip, port);
}

void terminate_connection(int id)
{
    if (id < 1 || id > connection_count)
    {
        printf("Invalid connection ID\n");
        return;
    }

    int index = id - 1;
    close(connections[index].socket);
    printf("Connection %d terminated.\n", id);

    pthread_mutex_lock(&lock);
    for (int i = index; i < connection_count - 1; i++)
    {
        connections[i] = connections[i + 1];
    }
    connection_count--;
    pthread_mutex_unlock(&lock);
}

void cleanup_connections() {
    // Close server socket
    if (server_socket != -1) {
        shutdown(server_socket, SHUT_RDWR);
        close(server_socket);
        server_socket = -1;
    }

    pthread_mutex_lock(&lock);
    
    // Close all client connections
    for (int i = 0; i < connection_count; i++) {
        if (connections[i].socket != -1) {
            shutdown(connections[i].socket, SHUT_RDWR);
            close(connections[i].socket);
            connections[i].socket = -1;
        }
    }
    connection_count = 0;
    
    pthread_mutex_unlock(&lock);
    
    // Destroy mutex
    pthread_mutex_destroy(&lock);
    
    printf("All connections cleaned up\n");
}
