#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

void parent_do(int* fd)
{
    while(1)
    {
        char buf[1024];
        printf("please ipnut a string\n");
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf) -1] = '\0';

        write(fd[1],buf,strlen(buf));

        if(strncmp(buf,"quit",4))
        {
            break;
        }

    }

}
void child_do(int* fd)
{
    while(1)
    {
        char buf[1024];
        int ret_read;
        ret_read=read(fd[0],buf,sizeof(buf)-1);
        buf[ret_read - 1]='\0';
        if(ret_read == 0)
        {
            break;
        }
        printf("%s\n",buf);

    }

}

int main(int argc,const char* argv[])
{
    int fd[2];
    int ret_pipe;
    pid_t pid_fork;

    ret_pipe=pipe(fd);

    if(0 > ret_pipe)
    {
        perror("Fail to pipe");
        exit(EXIT_FAILURE);
    }

    pid_fork=fork();

    if(0 > pid_fork)
    {
        perror("Fail to fork");
        exit(EXIT_FAILURE);
    }
    if(0 < pid_fork)
    {

        parent_do(fd);
    }
    if(0 == pid_fork)
    {
        child_do(fd);
    }

    return 0;

}
