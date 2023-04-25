// Nicholas Palmer 04/2023

#include <iostream>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "network.h"
#include "response_builder.h"
#include "parser.h"

void respond(char *request, int socket_desc) {
    REQUEST_TYPE request_type = UNKNOWN;
    PROTOCOL_TYPE protocol_type = NONE;
    std::string uri_path; // Path string

    // Parse request line
    parse_request_line(request, request_type, uri_path, protocol_type);
    std::cout << "| " << uri_path << " ";

    if (request_type  == GET) {
        handle_get(socket_desc, uri_path);
    } else {
        std::cerr << "REQUEST TYPE NOT IMPLEMENTED" << std::endl;
    }
}

void handle_get(const int socket_desc, std::string uri_path) {
    int status_code;
    int resource_fd = -1;
    struct stat stat_buf{};
    RESOURCE_STATE resource_state = process_uri(uri_path);

    // Open file
    if (resource_state == OK) {
        resource_fd = open(uri_path.c_str(), O_RDONLY | O_EXCL);

        if (resource_fd == -1) {
            perror("open");
        }
    }

    if (resource_state != OK) {
        status_code = 404;
        std::cout << "FILE READ ERROR: '" << uri_path << "'" << std::endl;
    }
    else {
        status_code = 200;
        fstat(resource_fd, &stat_buf);
    }

    std::cout << "| CODE " << status_code << std::endl;

    // Build response header
    std::string header;
    build_get_response_header(header, stat_buf.st_size, status_code);
    send_header(socket_desc, header);

    // Send resource
    if (resource_fd != -1) {
        send_resource(socket_desc, resource_fd, stat_buf);

        if (close(resource_fd) == -1) {
            perror("close");
        }

    } else {
        // TODO SEND 404 Error;
    }
}

void build_get_response_header(std::string &header, off_t resource_size, int status_code) {
    // Build response header
    header.append("HTTP/1.1 ");

    // Set status message
    switch (status_code) {
        case 200: {
            header.append(std::to_string(status_code) + " " + "OK" + "\r\n");
            break;
        } case 404: {
            header.append(std::to_string(status_code) + " " + "Not Found" + "\r\n");
            break;
        } case 405: {
            header.append(std::to_string(status_code) + " " + "Method Not Allowed" + "\r\n");
            break;
        } default: {
            header.append(std::to_string(500) + " " + "Unknown" + "\r\n");
            break;
        }
    }

    // Build response header
    header.append("Server: SnorlaxServer/0.0.1\r\n");
    // TODO Determine Content Type
    // response.append("Content-Type: text/html\r\n");

    // Content Length is not required for GET
    header.append("Content-Length: ");
    header.append(std::to_string(resource_size));
    header.append("\r\n");
    header.append("\r\n");

    // std::copy(response.begin(), response.end(), std::back_inserter(request));
}