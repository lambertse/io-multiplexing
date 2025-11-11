#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io-multiplexing/server/server.h"
#include "io-multiplexing/define.h"

// Callback function implementation
char* process_request(const char *request, size_t req_size, size_t *res_size) {
    printf("Request: %s\n", request);
    
    // Calculate required buffer size
    size_t prefix_len = strlen("Hello, I'm responsing for \"");
    size_t suffix_len = strlen("\"");
    *res_size = prefix_len + req_size + suffix_len + 1; // +1 for null terminator
    
    char *response = (char *)malloc(*res_size);
    if (!response) {
        *res_size = 0;
        return NULL;
    }
    
    // Build response string
    strcpy(response, "Hello, I'm responsing for \"");
    strncat(response, request, req_size);
    strcat(response, "\"");
    
    *res_size = strlen(response);
    
    return response;
}

int main() {
    ServerConfig server_conf;
    server_conf.port = 8091;
    
    Server *server = server_create(server_conf);
    if (!server) {
        printf("Failed to create server\n");
        return 0;
    }
    
    // Initialize server with callback function
    if (!server_init(server, process_request)) {
        printf("Init failed\n");
        server_destroy(server);
        return 0;
    }
    
    server_start(server);
    
    server_destroy(server);
    
    return 1;
}
