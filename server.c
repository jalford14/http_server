#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int status, sockfd, new_fd;
    socklen_t addr_size;
    struct sockaddr_storage their_addr;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if ((status = getaddrinfo(NULL, "3490", &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if ((status = bind(sockfd, res->ai_addr, res->ai_addrlen)) != 0) {
        fprintf(stderr, "bind error: %s\n", gai_strerror(status));
        exit(1);
    }
    if ((status = listen(sockfd, 20)) != 0) {
        fprintf(stderr, "listen error: %s\n", gai_strerror(status));
        exit(1);
    }

    addr_size = sizeof their_addr;
    if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size)) != 0) {
        fprintf(stderr, "accept error: %s\n", gai_strerror(new_fd));
        exit(1);
    }

    char buff[100];
    recv(new_fd, buff, 99, 0);
    printf("Message received: %s\n", buff);

    freeaddrinfo(res); // free the linked-list
}
