#include "request.h"
#include <string.h>
#include <stdio.h>

void parse_request(const char *raw_request, HttpRequest *request)
{
    sscanf(raw_request, "%s %s %s", request->method, request->path, request->version);
}
