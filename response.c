#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void build_response(HttpResponse *res, const char *content, const char *content_type, int status_code)
{
    const char *status_text = (status_code == 200) ? "OK" : "Bad Request";

    char headers[1024];
    snprintf(headers, sizeof(headers),
             "HTTP/1.1 %d %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "Connection: close\r\n"
             "\r\n",
             status_code, status_text, content_type, strlen(content));

    res->length = strlen(headers) + strlen(content);
    res->data = malloc(res->length + 1);
    sprintf(res->data, "%s%s", headers, content);
}

void serve_static(const char *filepath, HttpResponse *res)
{
    FILE *fp = fopen(filepath, "rb");
    if (!fp)
    {
        build_response(res, "404 Not Found", "text/plain", 404);
        return;
    }

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);

    char *body = malloc(filesize);
    fread(body, 1, filesize, fp);
    fclose(fp);

    const char *content_type = "application/octet-stream";
    if (strstr(filepath, ".html"))
        content_type = "text/html";
    else if (strstr(filepath, ".png"))
        content_type = "image/png";
    else if (strstr(filepath, ".jpg"))
        content_type = "image/jpeg";

    const char *status_text = "OK";
    char headers[1024];
    snprintf(headers, sizeof(headers),
             "HTTP/1.1 200 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "Connection: close\r\n"
             "\r\n",
             status_text, content_type, filesize);

    res->length = strlen(headers) + filesize;
    res->data = malloc(res->length + 1);
    memcpy(res->data, headers, strlen(headers));
    memcpy(res->data + strlen(headers), body, filesize);

    free(body);
}

void calc_operation(const char *path, HttpResponse *res)
{
    int num1, num2;
    char op[10];
    if (sscanf(path, "/calc/%[^/]/%d/%d", op, &num1, &num2) != 3)
    {
        build_response(res, "Bad Request", "text/plain", 400);
        return;
    }

    int result = 0;
    if (strcmp(op, "add") == 0)
    {
        result = num1 + num2;
    }
    else if (strcmp(op, "mul") == 0)
    {
        result = num1 * num2;
    }
    else if (strcmp(op, "div") == 0)
    {
        if (num2 == 0)
        {
            build_response(res, "Division by zero!", "text/plain", 400);
            return;
        }
        result = num1 / num2;
    }
    else
    {
        build_response(res, "Invalid operation", "text/plain", 400);
        return;
    }

    char body[256];
    snprintf(body, sizeof(body), "Result: %d\n", result);
    build_response(res, body, "text/plain", 200);
}

void handle_request(const HttpRequest *req, HttpResponse *res)
{
    if (strcmp(req->method, "GET") != 0)
    {
        build_response(res, "Only GET supported", "text/plain", 400);
        return;
    }

    if (strncmp(req->path, "/static/", 8) == 0)
    {
        char filepath[1024] = "static";
        strcat(filepath, req->path + 7);
        serve_static(filepath, res);
    }
    else if (strncmp(req->path, "/calc/", 6) == 0)
    {
        calc_operation(req->path, res);
    }
    else
    {
        build_response(res, "404 Not Found", "text/plain", 404);
    }
}

void free_response(HttpResponse *res)
{
    free(res->data);
}
