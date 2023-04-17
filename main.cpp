// Nicholas Palmer 04/2023

#include <iostream>
#include <filesystem>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "response_builder.h"
#include "common.h"

#define PORT 80
#define BUFFER_SIZE 65536

int main(int argc, char *argv[]) {
    int socket_fd, new_socket_fd; // socket file descriptors
    struct sockaddr_in server_addr{}, client_addr{}; // Create address struct
    char addr_str[INET_ADDRSTRLEN]; // ipv4 address string

    // Parse command line arguments
    if (argc != 2) {
        config::base_path = fs::current_path();
    } else {
        config::base_path = argv[1];
    }

    // Check if base path exists
    if (exists(config::base_path)) {
        std::cout << "Base path: " << config::base_path << std::endl;
    } else {
        perror("Could not find base path");
        return -1;
    }

    // Create socket file descriptor
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("Failed to create socket fd...!");
        exit(1);
    }

    // int setsockopt(int sockfd, int level, int optname,  const void *optval, socklen_t optlen);

    // Allow socket to be reused
    const int enable = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEPORT) failed");

    // Set socket options
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // MAYBE
    // server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero)); // Erase 8 bytes

    // Bind socket
    inet_ntop(AF_INET, &(server_addr.sin_addr), addr_str, INET_ADDRSTRLEN);
    printf("HOST: bound to -> %s:%i\n", addr_str, PORT);

    if (bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Put server into listen mode
    if (listen(socket_fd, SOMAXCONN) != 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept connections
    while (true) {
        socklen_t client_addr_len = sizeof(client_addr);

        // printf("Waiting for accept...\n");
        // Extract first request to connect in the queue, and create new connected socket
        if ((new_socket_fd = accept(socket_fd, (struct sockaddr *) &server_addr, (socklen_t *) &client_addr_len)) ==
            -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // inet_ntop(server_addr.sin_family, get_in_addr((struct sockaddr*) &client_addr), addr_str, sizeof(addr_str));
        printf("HOST: CONNECTION FROM %s | ", addr_str);

        char recv_buffer[BUFFER_SIZE] = {0}; // 8 Kb
        size_t response_size = 0;
        ssize_t recv_size = recv(new_socket_fd, recv_buffer + response_size, BUFFER_SIZE - response_size, 0);

        if (recv_size > 0) {
            response_size += recv_size;
        } else if (recv_size == 0) {

            continue;
        } else if (recv_size == -1) {
            perror("recv");
            exit(EXIT_FAILURE);
        } else {
            printf("An Unknown Error has occurred\n");
            exit(EXIT_FAILURE);
        }

        respond(recv_buffer, new_socket_fd);

        // close the new socket
        if (close(new_socket_fd) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
    }

    // shutdown the main socket
    if (shutdown(socket_fd, SHUT_RDWR) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    return 0;
}
