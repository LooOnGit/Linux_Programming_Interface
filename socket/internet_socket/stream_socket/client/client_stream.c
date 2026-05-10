#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <error.h>

#define BUF_SIZE 1024
#define PORT 8080

int main(int argc, char *argv[])
{
    int clientFd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientFd < 0)
    {
        perror("Init socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if(connect(clientFd, (struct sockaddr*)&addr, sizeof(addr)))
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    char buf[BUF_SIZE];

    while(1)
    {
        printf("Enter message: ");
        fgets(buf, BUF_SIZE, stdin);

        write(clientFd, buf, strlen(buf));

        if(strcmp(buf, "quit\n") == 0)
        {
            break;
        }

        char recvBuf[BUF_SIZE];
        
        ssize_t sizeRead = read(clientFd, recvBuf, BUF_SIZE - 1);
        if(sizeRead < 0)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        recvBuf[sizeRead] = '\0';  // null-terminate

        printf("Server: %s\n", recvBuf);
    }

    close(clientFd);
    return 0;
}