#include "setup.h"
#include "handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void *thread_func(void *arg)
{
    int client_socket = *(int *)arg;
    free(arg);
    handle_connection(client_socket);
    close(client_socket);
    return NULL;
}

void start_server(int port)
{
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    while (1)
    {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0)
        {
            perror("accept failed");
            continue;
        }

        int *pclient = malloc(sizeof(int));
        *pclient = client_socket;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, thread_func, pclient);
        pthread_detach(thread_id);
    }
}
