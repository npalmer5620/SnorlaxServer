// Nicholas Palmer 04/2023

#pragma once
#include <vector>

enum RESOURCE_STATE {READ_ERROR = -2, NOT_FOUND = -1, OK = 0};

RESOURCE_STATE get_resource(const std::string &path, std::vector<char> &buffer);
void respond(char *request, int socket_desc);
void build_get_response_header(std::vector<char> &request, int status_code);



