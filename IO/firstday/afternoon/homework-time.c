#include <stdio.h>

int main(int argc,const char *argv[])
{

    int n,fd,i=0,k=0;
    int buf[100];
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
        close(fd);
    }
    return 0;

}
