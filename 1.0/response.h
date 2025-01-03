/*
** response.h -- forms the responses sent back to the client
**
** This file is responsible for creating a response to send to the
** client. It is both compatible with HTTP/0.9 and HTTP/1.0.
*/

// Determines what version of HTTP is being used and then builds the
// response accordingly.
#include <stdio.h>
#include <string.h>
#include <time.h>

#define RES_SIZE 1024 // max number of bytes we can send to client
#define URL "localhost:3490"

const char *get_body(const char *uri) {
    if (strcmp(uri, "/index.html") == 0) {
        return "<html><h1>Hello, world!</h1>\n<p>I'm building something</p></html>";
    } else if (strcmp(uri, "/posts.html") == 0) {
        return "<html><h1>Posts</h1></html>";
    } else {
        return "<html><h1>Page not found</h1></html>";
    }
}

const char *build_status_line(const char *status) {
    if (strcmp(status, "200") == 0) {
        return "HTTP/1.0 200 OK\r\n";
    } else if (strcmp(status, "404") == 0) {
        return "HTTP/1.0 200 OK\r\n";
    } else {
        return "HTTP/1.0 500 Internal Server Error\r\n";
    }
}

const char *build_header(const char *status, const char *body, const char *uri) {
    time_t now = time(0);
    struct tm *gmt = gmtime(&now);
    char date_str[128];
    strftime(date_str, sizeof(date_str), 
         "%a, %d %b %Y %H:%M:%S GMT", gmt);
    char status_line[1024];
    char header[1024];

    strcpy(status_line, build_status_line(status));
    snprintf(header, 1024,
            "Content-Type: text/html\r\n"
            "Content-Length: %lu\r\n"
            "Location: %s%s\r\n"
            "Date: %s\r\n"
            "\r\n", strlen(body), URL, uri, date_str);

    return strcat(status_line, header);
}

void build_response(
        char *response,
        const char *method,
        const char *version,
        const char *uri
    ) {

    // TODO: build out HEAD and POST
    if (strcmp(method, "GET") == 0) {
        if (strcmp(version, "") == 0) {
            // HTTP/0.9
            snprintf(response, RES_SIZE, "%s", get_body(uri));
        } else {
            // HTTP/1.0
            if (strcmp(get_body(uri), "") == 0) {
                strcpy(response, build_header("404", get_body(uri), uri));
            } else {
                strcpy(response, build_header("200", get_body(uri), uri));
            }
            strcat(response, get_body(uri));
        }
    } else if (strcmp(method, "HEAD") == 0) {
        if (strcmp(get_body(uri), "") == 0) {
            snprintf(response, RES_SIZE,
                    "HTTP/1.0 404 Not Found\r\n"
                    "Content-Type: text/html\r\n"
                    "Location: %s%s\r\n", URL, uri);
        } else {
            if (strcmp(get_body(uri), "") == 0) {
                strcpy(response, build_header("404", get_body(uri), uri));
            } else {
                strcpy(response, build_header("200", get_body(uri), uri));
            }
        }
    } else if (strcmp(method, "POST") == 0) {
        FILE *fptr;
        char buf[200];
        fptr = fopen("db.txt", "w");
        sscanf(uri, "%*[^?]?body=%[^\n]", buf);
        printf("body: %s", buf);
        fprintf(fptr, "%s\n", buf);
        fclose(fptr);

        strcpy(response, build_header("201", get_body(uri), uri));
    }
}
