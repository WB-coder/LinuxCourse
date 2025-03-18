#include "../inc/common.h"
#include "../inc/utils.h"

int is_valid_ip(char *ip) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip, &sa.sin_addr) == 1;
}

int is_self_connection(char *ip, int port) {
    struct sockaddr_in self_addr;
    socklen_t len = sizeof(self_addr);
    getsockname(server_socket, (struct sockaddr*)&self_addr, &len);
    return (ntohl(self_addr.sin_addr.s_addr) == inet_addr(ip)) && (port == listening_port);
}

void get_myip() {
    system("hostname -I | awk '{print $1}'");
}