#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#include "kernel/fs.h"

#define MSGSIZE 16

int main(int argc, char *argv[])
{
    char buf[MSGSIZE];
    read(0, buf, MSGSIZE);
    // printf("%s\n", buf);
    int n = read(0, buf, MSGSIZE);
    int buf_idx = 0;
    while(n>0)
    {
        buf_idx += n;
        n = read(0, &buf[buf_idx], MSGSIZE);
    }

    char *xargv[MAXARG];
    int xargc = 0;
    for(int i=1;i<argc;i++)
    {
        xargv[xargc] = argv[i];
        xargc++;
    }
    
    char *p = buf;
    for(int i=0;i<MAXARG;i++)
    {
        if(buf[i] == '\n')
        {
            int pid = fork();
            if(pid > 0)
            {
                p = &buf[i+1];
                wait(0);
            }
            else
            {
                buf[i] = 0;
                xargv[xargc] = p;
                xargc++;
                xargv[xargc] = 0;
                exec(xargv[0], xargv);
                exit(0);
            }
        }
    }
    exit(0);
}