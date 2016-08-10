#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#define err_log(errlog) do{perror(errlog); exit(1);}while(0) 

// ./server  192.168.2.196 10000

#define  N  1024
#define CMD 3

void help(int sockfd)
{
    int i;
    char* cmd[CMD];
    printf("*************************************\n");
    for(i=0;i<CMD;i++)
    {
        recv(sockfd,cmd[i],strlen(cmd[i]),0);
        printf("%s\n",cmd[i]);
        sleep(0.1);
    }
    printf("*************************************\n");
}

void list(int sockfd)
{
    char buf[N];




}

void push(char* str[],int sockfd)
{
    int i;
    char read_buf[N];
    int  src_fd,ret_read,dest_fd;
    struct dirent *ret_readdir;

    printf("local push\n");


  

            if((src_fd = open(str[1],O_RDONLY)) <0)
            {
                err_log("fail to open ");
            }


            while(1)
            {
                if((ret_read = read(src_fd,read_buf,sizeof(read_buf))) < 0)
                {
                    err_log("fail to read ");
                }
                if(ret_read == 0)
                {
                    break;
                }
                send(sockfd,read_buf,ret_read,0);

            }



}

void get(char* str[],int sockfd)
{
    int i;
    char buf[N];
    struct dirent *ret_readdir;
    int  src_fd,ret_read,dest_fd;



        if(str[1] != NULL)
        {

            if((dest_fd = open(str[1],O_WRONLY|O_CREAT|O_TRUNC,0666)) <0)
            {
                err_log("fail to open");
            }
            printf("local get\n");

            while(1)
            {
                if((recv(sockfd,buf,N,0)) < 0)
                {
                    err_log("fail to recv");
                }
                if(recv == 0)
                {
                    break;
                }
                write(dest_fd,buf,N);

            }

        }
        close(dest_fd);
}

int main(int argc, const char *argv[])
{

    int sockfd,dest_fd,src_fd;
    int acceptfd;
    int i=0,j=1;
    char* str[2];
    struct sockaddr_in serveraddr, clientaddr;
    struct dirent *ret_readdir;
    DIR* dir;
    char read_buf[N];
    char buf[N] = {};
    char* cmd[CMD];
    int ret_read;

    if(argc < 3)
    {
        fprintf(stderr, "Usage:%s serverip port.", argv[0]);
        return -1;
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        err_log("fail to socket");
    }

    printf("sockfd = %d\n", sockfd);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));

    if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        err_log("fail to bind");
    }

    if(listen(sockfd, 15) < 0)
    {
        err_log("fail to listen");
    }


    socklen_t addrlen = sizeof(struct sockaddr);

    if((acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr,&addrlen)) < 0)
    {
        err_log("fail to accept");
    }
    printf("acceptfd = %d\n", acceptfd);


    printf("%s --> %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));


    while(1)
    {
        printf("<input> ");

        fgets(buf, N, stdin);
        buf[strlen(buf)-1] = '\0';

        send(sockfd, buf, N, 0);

        if(strncmp(buf, "quit", 4) == 0)
            break;

        str[0] = strtok(buf, " ");
        str[1]=strtok(NULL," ");


        if(strncmp(buf,"list",4) ==0)
        {
            list(sockfd);

        }

        if(strncmp(buf,"help",4) == 0)
        {
            help(sockfd);
        }

        if(strncmp(buf,"get",3) ==0)
        {

            get(str,sockfd);

        }
        if(strncmp(buf,"push",4) ==0)
        {
            push(str,sockfd);
        }



        close(sockfd);
    }
    return 0;
}
