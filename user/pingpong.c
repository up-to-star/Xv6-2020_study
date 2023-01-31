#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int pid;
    int p1[2], p2[2]; // p1: parent  --> child, p2: child --> parent; 0: read fd, 1: write fd
    char buf[1];
    pipe(p1);
    pipe(p2);
    pid = fork();
    if(pid < 0)
    {
        printf("fork error\n");
        exit(1);
    }
    else if(pid == 0) // child process
    {
        close(p1[1]);
        close(p2[0]);
        read(p1[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(p2[1], " ", 1);
        close(p1[0]);
        close(p2[1]);
        exit(0);
    }
    else // parent process
    {
        close(p1[0]);
        close(p2[1]);
        write(p1[1], " ", 1);
        read(p2[0], buf, 1);
        printf("%d: received pong\n", getpid());
        
        close(p1[1]);
        close(p2[0]);
        exit(0);
    }
    return 0;
}