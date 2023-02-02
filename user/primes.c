#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void child(int *pl)
{
    int pr[2];
    int n;
    close(pl[1]);

    int resd_size = read(pl[0], &n, sizeof(int));
    if(resd_size == 0)
    {
        exit(0);
    }
    pipe(pr);
    
    if(fork() == 0)
    {
        child(pr);
    }
    else
    {
        printf("prime %d\n", n);
        close(pr[0]);
        int t = n;
        while(read(pl[0], &n, sizeof(int)) != 0)
        {
            if(n % t != 0) write(pr[1], &n, sizeof(int));
        }
        close(pl[0]);
        close(pr[1]);
        wait(0);
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if(fork() == 0)
    {
        child(p);
    }
    else
    {
        close(p[0]);
        for(int i=2;i<=35;i++)
            write(p[1], &i, sizeof(int));
        close(p[1]);
        wait(0);
        exit(0);
    }
    return 0;
}