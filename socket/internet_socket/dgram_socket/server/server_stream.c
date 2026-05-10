#include <stdio.h>    // printf, fgets, perror
#include <stdlib.h>   // exit, EXIT_FAILURE
#include <string.h>   // memset, strlen
#include <unistd.h>   // close, read, write
#include <sys/socket.h> // socket, bind, sendto, recvfrom, sockaddr
#include <errno.h>    // errno

#include <netinet/in.h>  // struct sockaddr_in, INADDR_ANY, htons, htonl
#include <arpa/inet.h>   // inet_pton, inet_ntop, ntohs, ntohl

#define PORT 8080

int main(int argc, char *argv[]){
    int serverFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(serverFd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    if(bind(serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    socklen_t clientAddrLen = sizeof(clientAddr);

    char buff[1024];

    while(1)
    {
        int len = recvfrom(serverFd, buff, sizeof(buff), 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if(len < 0)
        {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        buff[len] = '\0';
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
        printf("Client %s:%d received: %s\n", clientIP, ntohs(clientAddr.sin_port), buff);

        char okBuff[] = "OK";
        if(sendto(serverFd, okBuff, strlen(okBuff), 0, (struct sockaddr*)&clientAddr, clientAddrLen) < 0)
        {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
    }
    close(serverFd);
    return 0;
}