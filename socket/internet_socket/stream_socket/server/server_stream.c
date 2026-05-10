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


int main(int argc, char *argv[]) {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverFd < 0)
    {
        perror("Init socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if(setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("Set socket opt");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
    
    if(bind(serverFd, (struct sockaddr*)&addr, sizeof(addr)))
    {
        perror("Bind");
        exit(EXIT_FAILURE);
    }

    if(listen(serverFd, 10))
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server waiting for client ...\n");

    struct sockaddr_in clientAddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    int clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &addrlen);
    if(clientFd < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];

    while(1)
    {
        memset(buf,0,BUF_SIZE);

        size_t sizeRead = read(clientFd, buf, BUF_SIZE);
        if(sizeRead < 0)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        printf("%s\n", buf);
        

        char okBuf[] = "OK";
        if(write(clientFd, okBuf, strlen(okBuf)) < 0 ){
            perror("write");
            exit(EXIT_FAILURE);
        }
        
    }
    close(clientFd);
    close(serverFd);
    return 0;
}
