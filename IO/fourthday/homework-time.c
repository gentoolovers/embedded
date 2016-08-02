#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


int main(int argc,const char *argv[])
{

    int n,fd,fd_log,i=0,k=0;
    char buf[100];
    pid_t pid;
    if(argc < 2)
    {

        printf("Usage :%s <filename>",argv[0]);
        return -1;
    }

    fd=open(argv[1],O_RDWR|O_CREAT|O_APPEND,0666);
    if(fd < 0 )
    {

        printf("(%d,%s) Fail to open %s:%s",__LINE__,__FUNCTION__,argv[1],strerror(errno));
        return -1;
    }



    while(1)
    {

        n=read(fd,buf,sizeof(buf));
        if(0 == n)
        {

            break;
        }
        if(n < 0)
        {

            printf("(%d,%s) Fail to read %s:%s",__LINE__,__FUNCTION__,argv[1],strerror(errno));
            return -1;
        }
        for(i=0;i<n;i++)
        {

            if(buf[i] == '\n');
            k++;
        }
    }

    pid=fork();
    if(pid <0 )
    {
        printf("Fail to fork()");
        exit(EXIT_FAILURE);
    }
    if(pid > 0)
    {
        exit(EXIT_FAILURE);
    }
    if(pid == 0 )
    {
        if(setsid() < 0)
        {
            perror("Fail to setsid");
            exit(EXIT_FAILURE);
        }
        if(chdir("./") <0)
        {
            perror("fail to chang dir /");
            exit(EXIT_FAILURE);
        }
        if(umask(0) <0)
        {
            perror("fail to setsid");
            exit(EXIT_SUCCESS);
        }


    }

    fd_log=open("log.txt",O_RDWR|O_CREAT|O_APPEND,0666);
    if(fd_log < 0 )
    {

        printf("(%d,%s) Fail to open %s:%s",__LINE__,__FUNCTION__,"log.txt",strerror(errno));
        return -1;
    }

    close(0);
    close(1);
    close(2);

    dup2(fd_log,1);
    dup2(fd_log,2);


    while(1)
    {

        k=k+1;
        time_t t;
        struct tm *ptm;
        t=time(NULL);
        ptm=localtime(&t);

        n=sprintf(buf,"%d,%d-%d-%d %d:%d:%d\n",k,ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);

        printf("%d,%d-%d-%d %d:%d:%d\n",k,ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
        write(fd,buf,n);
        sleep(1);

    }
    close(fd);
    return 0;

}
