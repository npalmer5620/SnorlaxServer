// Nicholas Palmer 04/2023

#include <iostream>
#include <fstream>

#include <sys/socket.h>

#include "response_builder.h"
#include "parser.h"
#include "common.h"

RESOURCE_STATE get_resource(const std::string &uri_path, std::vector<char> &buffer) {
    // Prepend base path
    std::string system_path = config::base_path.string().append(uri_path);

    // Append index.html if path ends with a slash
    if (system_path.back() == '/') {
        system_path.append("index.html");
    }

    // Check if file exists
    if (!fs::exists(system_path)) {
        std::cout << "RESOURCE NOT FOUND: '" << system_path << "'" << std::endl;
        return NOT_FOUND;
    }

    // Read file into buffer
    std::ifstream file(system_path, std::ios::binary);

    // Check if file opened
    if (!file.is_open()) {
        std::cout << "RESOURCE LOAD ERROR: '" << system_path << "'" << std::endl;
        return READ_ERROR;
    }

    // Preserve all whitespace chars
    file.unsetf(std::ios::skipws);

    // Read file into buffer
    std::istream_iterator<char> start(file), end;
    buffer.assign(start, end);

    file.close();

    return OK;
}

void respond(char *request, int socket_desc) {
    int status_code;
    REQUEST_TYPE request_type = UNKNOWN;
    PROTOCOL_TYPE protocol_type = NONE;
    RESOURCE_STATE res_state = NOT_FOUND;
    std::string uri_path; // Path string

    std::vector<char> resource; // Resource buffer
    std::vector<char> response; // Response buffer

    // Parse request line
    parse_request_line(request, request_type, uri_path, protocol_type);
    std::cout << "| " << uri_path << " ";

    // Load requested resource
    res_state = get_resource(uri_path, resource);

    // Set status code
    if (res_state == OK) {
        status_code = 200;
    } else {
        status_code = 404;
    }

    std::cout << "| CODE " << status_code << std::endl;

    // Build response header
    build_get_response_header(response, status_code);

    // Append resource to response
    response.insert(std::end(response), std::begin(resource), std::end(resource));

    // Clear resource
    std::vector<char>().swap(resource);

    // Send response
    if (send(socket_desc, response.data(), response.size(), 0) == -1) {
        perror("send");
        exit(EXIT_FAILURE);
    }

    // Clear response
    std::vector<char>().swap(response);
}


void build_get_response_header(std::vector<char> &request, int status_code) {
    std::string response;
    std::string status_message;

    // Set status message
    switch (status_code) {
        case 200: {
            status_message = "OK";
            break;
        } case 404: {
            status_message = "Not Found";
            break;
        } case 405: {
            status_message = "Method Not Allowed";
            break;
        } default: {
            status_code = 500;
            status_message = "Unknown";
            break;
        }
    }

    // Build response header
    response.append("HTTP/1.1 " + std::to_string(status_code) + " " + status_message + "\n");
    response.append("Server: BladeServer/0.0.1\n");
    response.append("Content-Type: text/html\n");
    response.append("\n");

    std::copy(response.begin(), response.end(), std::back_inserter(request));
}