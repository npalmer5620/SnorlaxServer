// Nicholas Palmer 04/2023

#pragma once
#include <string>

enum RESOURCE_STATE {READ_ERROR = -2, NOT_FOUND = -1, OK = 0};
enum REQUEST_TYPE {INVALID = -1, UNKNOWN = 0, GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH};
enum PROTOCOL_TYPE {NONE, HTTP_1_1};

RESOURCE_STATE process_uri(std::string &uri_path);
int parse_request_line(char *recv_buffer, REQUEST_TYPE &request_type, std::string &path, PROTOCOL_TYPE &protocol_type);

