#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
void exit_child(int signum)
{
    int status;
    waitpid(-1,NULL,WNOHANG);
    printf(" child exit \n");
}
int main(int argc,const char* argv[])
{
    pid_t pid;
    pid=fork();
    if(0 > pid)
    {
        fprintf(stderr,"Fail to fork",strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(0 == pid)
    {
        int i=0;
        while(i<5)
        {
            printf("child process exit %d\n",5-i);
            i++;
        }

    }
    if(0 < pid)
    {
        if(signal(SIGCHLD,SIG_ING) == SIG_ERR)
        {
            fprintf(stderr,"signal fail",strerror(errno));
            exit(EXIT_FAILURE);
        }
        while(1);
    }



    return 0;
}
