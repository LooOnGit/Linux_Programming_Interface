#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define SV_SOCK_PATH "/tmp/unix.dgram.sock"
#define BUF_SIZE 4096

int main() {
    int clientFd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(clientFd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    char clientPath[128];
    snprintf(clientPath, sizeof(clientPath), "/tmp/unix.dgram.%d", getpid());
    
    unlink(clientPath);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, clientPath, sizeof(addr.sun_path)-1);

    if(bind(clientFd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);   
    }

    char buffer[BUF_SIZE];
    memset(buffer, 0, BUF_SIZE);

    struct sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(struct sockaddr_un));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, SV_SOCK_PATH, sizeof(serverAddr.sun_path)-1);
    
    while(1)
    {
        printf("Enter messages: ");
        fgets(buffer, BUF_SIZE, stdin);
        sendto(clientFd, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_un));
        printf("Send message\n");
        memset(buffer, 0, BUF_SIZE);
        socklen_t addrlen = sizeof(struct sockaddr_un);
        int ret = recvfrom(clientFd, buffer, BUF_SIZE-1, 0, NULL, NULL);
        if(ret < 0)
        {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
        printf("Receive: %s\n", buffer);
    }
    close(clientFd);
    exit(EXIT_SUCCESS);
}