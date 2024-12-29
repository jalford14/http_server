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

#define RES_SIZE 1024 // max number of bytes we can send to client

const char *get_body(const char *uri) {
    if (strcmp(uri, "/index.html") == 0) {
        return "<h1>Hello, world!</h1>\n<p>I'm building something</p>";
    } else if (strcmp(uri, "/profile.html") == 0) {
        return "<h1>Another one</h1>";
    } else {
        return "";
    }
}

const char *not_found_html() {
    return "<html>404 Not Found</html>";
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
            snprintf(response, RES_SIZE, "<html>%s</html>", get_body(uri));
        } else {
            // HTTP/1.0
            const char *body = get_body(uri);
            if (strcmp(get_body(uri), "") == 0) {
                snprintf(response, RES_SIZE,
                        "HTTP/1.0 404 Not Found\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %lu\r\n"
                        "%s\r\n", strlen(body), body);
            } else {
                const char *body = get_body(uri);
                snprintf(response, RES_SIZE,
                        "HTTP/1.0 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %lu\r\n"
                        "\r\n"
                        "<html>%s</html>", strlen(body) + 13, body);
            }
        }
    }
}
