#include "handler.h"
#include "request.h"
#include "response.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int verbose = 0;

void *handle_connection(int client_socket)
{
    char buffer[4096];
    int received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

    if (received < 0)
    {
        perror("recv failed");
        return;
    }

    buffer[received] = '\0';

    if (verbose)
    {
        printf("Received request:\n%s\n", buffer);
    }

    HttpRequest request;
    parse_request(buffer, &request);

    HttpResponse response;
    handle_request(&request, &response);

    send(client_socket, response.data, response.length, 0);

    free_response(&response);

    return NULL;
}
