#ifndef RESPONSE_H
#define RESPONSE_H

#include "request.h"

typedef struct
{
    char *data;
    int length;
} HttpResponse;

void handle_request(const HttpRequest *req, HttpResponse *res);
void free_response(HttpResponse *res);

#endif
