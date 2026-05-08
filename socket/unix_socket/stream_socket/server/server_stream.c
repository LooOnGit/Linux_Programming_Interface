#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define SV_SOCK_PATH "/tmp/unix.stream.sock"
#define BUF_SIZE 4096

int main() {
    //create a UNIX domain stream socket
    int serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
    char buffer[BUF_SIZE];
    if (serverFd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    unlink(SV_SOCK_PATH);
    
    // Set up the socket address structure
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path)-1);

    bind(serverFd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));

    if(listen(serverFd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server waiting for client ...\n");
    /*wait client to connect*/
    int clientFd = accept(serverFd, NULL, NULL);

    if(clientFd < 0)
    {
        perror("accept");
        return 1;
    }

    printf("Client connected!\n");

    while(1)
    {
        /*read data from client and echo it back*/
        memset(buffer, 0, BUF_SIZE);
        ssize_t bytesRead = read(clientFd, buffer, BUF_SIZE);
        if(bytesRead <= 0)
        {
            printf("Client disconnect\n");
            break;
        }
        printf("receive: %s\n", buffer);

        write(clientFd, buffer, strlen(buffer));
    }

    close(clientFd);
    close(serverFd);
    unlink(SV_SOCK_PATH);
    exit(EXIT_SUCCESS);
}