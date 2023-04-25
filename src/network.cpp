// Nicholas Palmer 04/2023

#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>
#include <iostream>

#include "../include/network.h"

int send_resource_fallback(const int valid_socket_fd, const int valid_resource_fd) {
    ssize_t bytes_read;
    ssize_t sent;
    unsigned char buffer[4096];

    read(valid_resource_fd, buffer, sizeof(buffer));
    std::cout << "Sending resource using fallback..." << std::endl;

    // Send resource
    while ((bytes_read = read(valid_resource_fd, buffer, sizeof(buffer))) != 0) {
        if (bytes_read == -1) {
            perror("read");
            return -1;
        }

        sent = send(valid_socket_fd, buffer, bytes_read, 0);

        if (sent == -1) {
            perror("send");
            return -1;
        }

        if (bytes_read < sizeof(buffer)) {
            break;
        }
    }
    return 0;
}


int send_resource(const int valid_socket_fd, const int valid_resource_fd, struct stat &stat_buf) {
    off_t offset = 0;
    // Send resource
    for (size_t bytes_to_send = stat_buf.st_size; bytes_to_send > 0;) {
        int bytes_sent = sendfile(valid_resource_fd, valid_socket_fd, offset, &stat_buf.st_size, nullptr, 0);

        if (bytes_sent <= 0) {
            // Error occurred
            if (bytes_sent == -1) {
                perror("sendfile");

                // Send fallback if errno is 45
                if (errno == 45) {
                    send_resource_fallback(valid_socket_fd, valid_resource_fd);
                }

                return -1;
            }
            break;
        }

        // Update offset
        bytes_to_send -= bytes_sent;
    }
    return 0;
}

int send_header(const int valid_socket_fd, const std::string &header) {
    // Send header
    if (send(valid_socket_fd, header.c_str(), header.size(), 0) == -1) {
        perror("send");
        return -1;
    }
    return 0;
}