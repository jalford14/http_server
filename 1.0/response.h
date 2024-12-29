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

const char *body(const char *uri) {
    if (strcmp(uri, "/index.html") == 0) {
        return "<h1>Hello, world!</h1>\n<p>I'm building something</p>";
    } else if (strcmp(uri, "/profile.html") == 0) {
        return "<h1>Another one</h1>";
    } else {
        return "";
    }
}


void build_response(
        char *response,
        const char *method,
        const char *version,
        const char *uri
    ) {

    // TODO: build out HEAD and POST
    if (strcmp(method, "GET") == 0) {
        if (version == NULL) {
            // HTTP/0.9
            snprintf(response, RES_SIZE, "<html>%s</html>", body(uri));
        } else {
            // HTTP/1.0
            if (strcmp(body(uri), "") == 0) {
                snprintf(response, RES_SIZE,
                        "HTTP/1.0 404 Not Found\r\n"
                        "Content-Type: text/html\r\n"
                        "\r\n"
                        "<html>404 Not Found</html>");
            } else {
                snprintf(response, RES_SIZE,
                        "HTTP/1.0 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "\r\n"
                        "<html>%s</html>", body(uri));
            }
        }
    }
}
