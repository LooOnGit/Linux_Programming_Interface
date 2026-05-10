#include <stdio.h>    // printf, fgets, perror
#include <stdlib.h>   // exit, EXIT_FAILURE
#include <string.h>   // memset, strlen
#include <unistd.h>   // close, read, write
#include <sys/socket.h> // socket, bind, sendto, recvfrom, sockaddr
#include <errno.h>    // errno

#include <netinet/in.h>  // struct sockaddr_in, INADDR_ANY, htons, htonl
#include <arpa/inet.h>   // inet_pton, inet_ntop, ntohs, ntohl

#define PORT 8080

int main(){
    int clientFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(clientFd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    char buff[1024];
    int len;

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    while(1)
    {
        printf("Enter message: ");
        fgets(buff, sizeof(buff), stdin);

        if(strlen(buff) > 0)
        {
            len = sendto(clientFd, buff, strlen(buff), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            if(len < 0)
            {
                perror("sendto");
                exit(EXIT_FAILURE);
            }            
        }

        len = recvfrom(clientFd, buff, sizeof(buff), 0, (struct sockaddr*)&serverAddr, &clientAddrLen);
        if(len < 0)
        {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        buff[len] = '\0';
        printf("Server received: %s\n", buff);
    }
    close(clientFd);
    return 0;
}