#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "setup.h"
#include "handler.h"

extern int verbose;

int main(int argc, char *argv[])
{
    int port = 80;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc)
        {
            port = atoi(argv[++i]);
            if (port < 1 || port > 65535)
            {
                fprintf(stderr, "Invalid port number: %s\n", argv[i]);
                return 1;
            }
        }
        else if (strcmp(argv[i], "-v") == 0)
        {
            verbose = 1;
        }
        else
        {
            fprintf(stderr, "Usage: %s [-p port] [-v]\n", argv[0]);
            return 1;
        }
    }

    printf("Starting server on port %d\n", port);
    start_server(port);
    return 0;
}
