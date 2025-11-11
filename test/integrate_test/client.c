#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "io-multiplexing/client/client.h"
#include "io-multiplexing/server/config.h"

typedef struct {
    ServerConfig server_conf;
    char *request;
    int thread_id;
} ThreadArgs;

void* client_thread(void *arg) {
    ThreadArgs *args = (ThreadArgs*)arg;
    
    Client *client = client_create();
    client_connect(client, &args->server_conf);
    
    size_t req_size = strlen(args->request);
    size_t res_size = 0;
    
    char *response = client_send_request(client, args->request, req_size, &res_size);
    printf("Response: %s. Size: %d\n", response, (int)res_size);
    
    
    free(client);
    free(args);
    return NULL;
}

int main() {
    ServerConfig server_conf = {.port = 8091};
    
    // Random vector string of 20 random strings:
    char *random_strings[] = {
        "Hello, World!",
        "Protobuf is great!",
        "Multithreading in C++",
        "Network programming",
        "Socket communication",
        "Concurrency",
        "Thread safety",
        "C++ best practices",
        "Error handling",
        "Performance optimization",
        "Data serialization",
        "Protocol buffers",
        "Asynchronous programming",
        "Event-driven architecture",
        "Client-server model",
        "Network protocols",
        "TCP/IP stack",
        "Socket programming in C++",
        "C++ networking libraries",
        "Cross-platform development"
    };
    
    const int numThreads = 20;
    pthread_t threads[numThreads];
    
    for (int i = 0; i < numThreads; i++) {
        ThreadArgs *args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
        args->server_conf = server_conf;
        args->request = random_strings[i];
        args->thread_id = i;
        
        if (pthread_create(&threads[i], NULL, client_thread, args) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            free(args);
            return 1;
        }
    }
    
    // Join all threads
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}
