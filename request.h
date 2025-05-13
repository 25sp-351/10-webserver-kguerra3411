#ifndef REQUEST_H
#define REQUEST_H

typedef struct
{
    char method[8];
    char path[1024];
    char version[16];
} HttpRequest;

void parse_request(const char *raw_request, HttpRequest *request);

#endif
