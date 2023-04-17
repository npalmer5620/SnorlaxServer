// Nicholas Palmer 04/2023

#pragma once
#include <string>

enum REQUEST_TYPE {INVALID = -1, UNKNOWN = 0, GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH};
enum PROTOCOL_TYPE {NONE, HTTP_1_1};

int parse_request_line(char *recv_buffer, REQUEST_TYPE &request_type, std::string &path, PROTOCOL_TYPE &protocol_type);

