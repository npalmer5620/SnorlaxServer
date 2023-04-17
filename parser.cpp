// Nicholas Palmer 04/2023

#include "parser.h"

int parse_request_line(char *request, REQUEST_TYPE &request_type, std::string &path, PROTOCOL_TYPE &protocol_type) {
    char *next;

    // Get request type
    if ((next = strsep(&request," ")) == nullptr)
        return -1;

    if (strncmp(next, "GET", 3) == 0) {
        request_type = GET;
    } else if (strncmp(next, "PUT", 3) == 0) {
        request_type = PUT;
    } else if (strncmp(next, "POST", 4) == 0) {
        request_type = POST;
    } else if (strncmp(next, "HEAD", 4) == 0) {
        request_type = HEAD;
    } else if (strncmp(next, "TRACE", 5) == 0) {
        request_type = TRACE;
    } else if (strncmp(next, "PATCH", 5) == 0) {
        request_type = PATCH;
    } else if (strncmp(next, "DELETE", 6) == 0) {
        request_type = DELETE;
    } else if (strncmp(next, "OPTIONS", 7) == 0) {
        request_type = OPTIONS;
    } else if (strncmp(next, "CONNECT", 7) == 0) {
        request_type = CONNECT;
    } else {
        request_type = INVALID;
    }

    // Get path
    if ((next = strsep(&request," ")) == nullptr)
        return -1;

    // Set Path
    path = next;

    // Get protocol
    if ((next = strsep(&request,"\r\n")) == nullptr)
        return -1;

    if (strncmp(next, "HTTP/1.1", 3) == 0) {
        protocol_type = HTTP_1_1;
    } else {
        protocol_type = NONE;
    }

    return 0;
}



