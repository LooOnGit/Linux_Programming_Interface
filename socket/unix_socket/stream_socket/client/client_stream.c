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
    int clientFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (clientFd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    char buffer[BUF_SIZE];

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path)-1);

    if(connect(clientFd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    while(1)
    {
        printf("Enter message: ");
        memset(buffer, 0, BUF_SIZE);
        if(fgets(buffer, BUF_SIZE, stdin) == NULL)
        {
            printf("break\n");
            break;
        }


        write(clientFd, buffer, strlen(buffer));

        memset(buffer, 0, BUF_SIZE);
        int ret = read(clientFd, buffer, BUF_SIZE);
        if(ret <= 0)
        {
            printf("Server discaonnected\n");
            break;
        }
        printf("received: %s\n", buffer);
    }
    close(clientFd);
    exit(EXIT_SUCCESS);
}