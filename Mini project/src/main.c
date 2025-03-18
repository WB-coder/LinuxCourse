#include "../inc/common.h"
#include "../inc/connection.h"
#include "../inc/cli.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    init_server(atoi(argv[1]));
    start_cli();
    cleanup_connections();
    return 0;
}