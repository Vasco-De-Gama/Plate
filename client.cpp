#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    char buffer[256];
    ssize_t nread;
    adr.sin_family = AF_INET;
    adr.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);
    connect(fd, (struct sockaddr*) &adr, sizeof adr);

    while (1) {
        nread = read(fd, buffer, BUFFER_SIZE);
        printf ("%s\n", buffer);
        memset (buffer, 0, sizeof(buffer));
    }

    return 0;

}
