#inlcude <stdio.h>
#inlcude <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

typedef struct _tag_pipeid
{
    int fd1[2];
    int fd2[2];
}pipeid;
int main(int argc,const char* argv[])
{
    pipeid pipid;

    pid_t pid_fork1,pid_fork2;
    int ret_pipe1,ret_pipe2;
    ret_pipe1=pipe(pipid.fd1);
    if(0 > ret_pipe1)
    {
        fprintf(stderr,"Fail to pipe pipe1",strerror(ret_pipe1));
        exit(EXIT_FAILURE);
    }

    ret_pipe2=pipe(pipid.fd2);
    if(0 >ret_pipe2)
    {
        fprintf(stderr,"Fail to pipe pipe2",strerror(ret_pipe2));
        exit(EXIT_FAILURE);0
    }
    pid_fork1 = fork();
    if(0 < pid_fork1)
    {
        fprintf(stderr,"Fail to fork fork1",strerror(errno));
        exit(EXIT_FAILURE);
    }else
    {
        pid_fork2=fork();
        if(0 < pid_fork2)
        {
            fprintf(stderr,"Fail to fork fork1",strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    if(0 == pid_fork1)
    {
        pthread_t tid[2];
        int ret;

        ret=pthread_create(&tid[0],NULL,fork1_tread1_read,&pipid.fd1);
        if(ret != 0)
        {
            fprintf(stderr, "Fail to pthread_create fork1_thread1:%s \n",strerror(ret));
            exit(EXIT_FAILURE);
        }
        ret=pthread_create(&tid[0],NULL,fork1_tread2_write,&pipid.fd1);
        if(ret != 0)
        {
            fprintf(stderr, "Fail to pthread_create fork2_thread2:%s \n",strerror(ret));
            exit(EXIT_FAILURE);
        }

    }

    if(0 ==pid_fork2 )
    {
        pthread_t tid[2];
        int ret;

        ret=pthread_create(&tid[0],NULL,fork2_tread1_read,&pipid.fd2);
        if(ret != 0)
        {
            fprintf(stderr, "Fail to pthread_create fork2_thread1:%s \n",strerror(ret));
            exit(EXIT_FAILURE);
        }
        ret=pthread_create(&tid[0],NULL,fork2_tread2_write,&pipid.fd2);
        if(ret != 0)
        {
            fprintf(stderr, "Fail to pthread_create fork2_thread2:%s \n",strerror(ret));
            exit(EXIT_FAILURE);
        }

    }


    return 0;
}
