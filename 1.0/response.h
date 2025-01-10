/*
** response.h -- forms the responses sent back to the client
**
** This file is responsible for creating a response to send to the
** client. It is both compatible with HTTP/0.9 and HTTP/1.0.
*/

// Determines what version of HTTP is being used and then builds the
// response accordingly.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RES_SIZE 1024 // max number of bytes we can send to client
#define URL "localhost:3490"

char *get_body(const char *uri) {
    if (strcmp(uri, "/index.html") == 0) {
        char *response = strdup("<html><h1>Hello, world!</h1>\n<p>I'm building something</p></html>");
        return response;
    } else if (strcmp(uri, "/posts.html") == 0) {
        FILE *fptr = fopen("db.txt", "r");
        if (!fptr) {
            return strdup("<html><h1>Error reading posts</h1></html>");
        }

        char *block = malloc(2000);
        char *posts = malloc(2000);
        char line[200];
        
        if (!block || !posts) {
            fclose(fptr);
            free(block);
            free(posts);
            return strdup("<html><h1>Memory allocation failed</h1></html>");
        }

        block[0] = '\0';
        
        while (fgets(line, sizeof(line), fptr)) {
            char post[300];  // Bigger than line to accommodate HTML tags
            snprintf(post, sizeof(post), "<p>%s</p>\n", line);
            if (strlen(block) + strlen(post) < 2000) {
                strcat(block, post);
            }
        }
        fclose(fptr);

        snprintf(posts, 2000, "<html><h1>Posts</h1>%s</html>", block);
        free(block);
        return posts;
    } else {
        return strdup("<html><h1>Page not found</h1></html>");
    }
}

const char *build_status_line(const char *status) {
    if (strcmp(status, "200") == 0) {
        return "HTTP/1.0 200 OK\r\n";
    } else if (strcmp(status, "201") == 0) {
        return "HTTP/1.0 201 Created\r\n";
    } else if (strcmp(status, "404") == 0) {
        return "HTTP/1.0 404 Not Found\r\n";
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
        char *buf = (char *)malloc(200);
        fptr = fopen("db.txt", "a");
        sscanf(uri, "/posts.html?body=%[^\n]", buf);
        printf("body: %s", buf);
        fprintf(fptr, "%s\n", buf);
        fclose(fptr);
        free(buf);

        strcpy(response, build_header("201", get_body(uri), uri));
    }
}
