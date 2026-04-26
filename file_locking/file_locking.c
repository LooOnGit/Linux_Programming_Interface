#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/file.h>

int main()
{
    int fd = open("output.txt", O_CREAT|O_RDWR|O_APPEND, 0644);
    if(fd < 0)
    {
        perror("Failed to open file");
        return 0;
    }

    // int lockfd = flock(fd, LOCK_EX);
    // if(lockfd < 0){
    //     perror("Failed to lock file");
    //     close(fd);
    //     return 0;
    // }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 5;
    lock.l_pid = getpid();

    if(fcntl(fd, F_SETLKW, &lock) < 0)
    {
        perror("Failed to lock file");
        close(fd);
        return 0;
    }
    printf("File is locked\n");
    sleep(5);

    char buffer[100];
    for(int i = 0; i < 1000; i++)
    {
        sprintf(buffer, "%d\n", i);
        ssize_t size = write(fd, buffer, strlen(buffer));
        if(size < 0){
            perror("Failed to write to file");
            break;
        }
        usleep(1000);
    }
    
    lock.l_type = F_UNLCK;
    if(fcntl(fd, F_SETLK, &lock) < 0)
    {
        perror("Failed to unlock file");
        close(fd);
        return 0;
    }
    printf("File is unlocked\n");
    
    // flock(fd, LOCK_UN);

    close(fd);

    return 0;
}