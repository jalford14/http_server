#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    int status, sockfd;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

    // get ready to connect
    status = getaddrinfo("www.example.net", "3490", &hints, &res);

    // servinfo now points to a linked list of 1 or more struct addrinfos

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    ;
    if ((status = connect(sockfd, res->ai_addr, res->ai_addrlen)) != 0) {
        fprintf(stderr, "connect error: %s\n", gai_strerror(status));
        exit(1);
    }

    char *msg = "Jimmy was here!";
    int len;

    len = strlen(msg);
    send(sockfd, msg, len, 0);
    close(sockfd);
}
