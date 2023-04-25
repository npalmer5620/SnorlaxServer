// Nicholas Palmer 04/2023

#pragma once
#include "common.h"

int send_resource_fallback(int valid_socket_fd, int valid_resource_fd);
int send_resource(int valid_socket_fd, int valid_resource_fd, struct stat &stat_buf);
int send_header(int valid_socket_fd, const std::string &header);

