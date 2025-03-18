#include "../inc/common.h"
#include "../inc/message.h"

void send_message(int connection_id, char *message) {
    // Validate connection ID range
    if (connection_id < 1 || connection_id > connection_count) {
        fprintf(stderr, "Error: Invalid connection ID %d\n", connection_id);
        return;
    }

    const int index = connection_id - 1;
    const int client_socket = connections[index].socket;
    const struct sockaddr_in *client_addr = &connections[index].address;

    // Validate message length against requirements
    const size_t message_length = strlen(message);
    if (message_length > MAX_MSG_LEN) {
        fprintf(stderr, "Error: Message exceeds maximum length of %d characters\n", MAX_MSG_LEN);
        return;
    }

    // Attempt to send message
    const ssize_t bytes_sent = send(client_socket, message, message_length, 0);
    
    if (bytes_sent < 0) {
        perror("Send failed");
    } else if (bytes_sent != (ssize_t)message_length) {
        fprintf(stderr, "Warning: Partial message sent (%zd/%zu bytes)\n", 
                bytes_sent, message_length);
    } else {
        printf("Success: Message sent to %s:%d\n",
               inet_ntoa(client_addr->sin_addr),
               ntohs(client_addr->sin_port));
    }
}

void *receive_messages(void *arg) {
    (void)arg; // Mark unused parameter
    fd_set read_fds;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int max_sd, activity, new_socket;
    char buffer[BUFFER_SIZE];

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        max_sd = server_socket;

        pthread_mutex_lock(&lock);
        for (int i = 0; i < connection_count; i++) {
            if (connections[i].socket > 0) {
                FD_SET(connections[i].socket, &read_fds);
                if (connections[i].socket > max_sd) 
                    max_sd = connections[i].socket;
            }
        }
        pthread_mutex_unlock(&lock);

        activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);

        if (FD_ISSET(server_socket, &read_fds)) {
            new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
            if (new_socket >= 0) {
                // Get actual peer port from connection
                struct sockaddr_in peer_addr;
                socklen_t peer_len = sizeof(peer_addr);
                getpeername(new_socket, (struct sockaddr *)&peer_addr, &peer_len);

                pthread_mutex_lock(&lock);
                connections[connection_count].socket = new_socket;
                connections[connection_count].address = peer_addr; // Store peer's actual address
                connection_count++;
                pthread_mutex_unlock(&lock);
                
                printf("New connection from %s:%d\n", 
                       inet_ntoa(peer_addr.sin_addr), 
                       ntohs(peer_addr.sin_port));
            }
        }

        pthread_mutex_lock(&lock);
        for (int i = 0; i < connection_count; i++) {
            if (FD_ISSET(connections[i].socket, &read_fds)) {
                memset(buffer, 0, BUFFER_SIZE);
                int bytes_received = recv(connections[i].socket, buffer, BUFFER_SIZE, 0);
                if (bytes_received > 0) {
                    buffer[bytes_received] = '\0';
                    printf("\nMessage from %s:%d -> %s\n",
                           inet_ntoa(connections[i].address.sin_addr),
                           ntohs(connections[i].address.sin_port), // Now shows correct port
                           buffer);
                }
            }
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}