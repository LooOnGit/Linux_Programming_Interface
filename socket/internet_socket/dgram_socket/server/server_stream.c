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
    char buffer[BUF_SIZE];

    int serverFd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(serverFd < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    unlink(SV_SOCK_PATH);
    
    struct sockaddr_un addr;
    struct sockaddr_un clientAddr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path)-1);
    if(bind(serverFd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("DGRAM server waiting...\n");

    while(1)
    {
        socklen_t addrlen = sizeof(struct sockaddr_un);
        memset(buffer, 0, BUF_SIZE);
        int ret = recvfrom(serverFd, buffer, BUF_SIZE-1, 0, (struct sockaddr *)&clientAddr, &addrlen);
        if(ret < 0)
        {
            perror("recv");
            exit(EXIT_FAILURE);
        }
        printf("Receive: %s\n", buffer);

        char okBuf[] = "OK\n";
        ret = sendto(serverFd, okBuf, strlen(okBuf), 0, (struct sockaddr *)&clientAddr, sizeof(struct sockaddr_un));
        if(ret < 0)
        {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
        printf("Send: %s\n", okBuf);
    }

    close(serverFd);

    exit(EXIT_SUCCESS);

}