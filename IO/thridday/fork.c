#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, const char* argv[])
{
    int pid,wpid,status;
    char buf[100];

    pid=fork();
    if(pid <0)
    {
        perror("fail to fork ");
        return -1;
    }
    while(1)
    {

        if(pid ==0)
        {
            sleep(5);
            break;

        }
        if(pid >0)
        {
            while(1)
            {
                wpid=waitpid(-1,&status,WNOHANG);
                if(wpid ==0)
                {
                    sleep(1);
                }else
                {
                    waitpid(pid,&status,WNOHANG);
                    break;
                }
            }
        }

    }



    return 0;

}
