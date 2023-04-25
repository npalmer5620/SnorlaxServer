// Nicholas Palmer 04/2023

#pragma once
#include <vector>

void respond(char *request, int socket_desc);
void handle_get(int socket_desc, std::string uri_path);
void build_get_response_header(std::string &header, off_t resource_size, int status_code);



