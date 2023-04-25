//
// Created by Nick Palmer on 4/18/23.
//

#pragma once
#include "common.h"

int send_resource_fallback(int valid_socket_fd, int valid_resource_fd);
int send_resource(int valid_socket_fd, int valid_resource_fd, struct stat &stat_buf);
int send_header(int valid_socket_fd, const std::string &header);

