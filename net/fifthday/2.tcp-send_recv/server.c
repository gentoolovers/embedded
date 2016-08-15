#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>

#define err_log(errlog) do{perror(errlog); exit(1);}while(0) 

// ./server  192.168.2.196 10000

#define  N  128

int main(int argc, const char *argv[])
{

    int sockfd;
    int acceptfd;
    int maxfd;
    fd_set dfs,tmpdfs;
    struct sockaddr_un serveraddr, clientaddr;
    char buf[N] = {};



    if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        err_log("fail to socket");
    }

    printf("sockfd = %d\n", sockfd);

    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path,"socketfile");

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
    maxfd=sockfd;
    FD_ZERO(&dfs);
    FD_SET(0,&dfs);
    FD_SET(sockfd,&dfs);
    while(1)
    {
        tmpdfs=dfs;
        if((select(maxfd+1,&tmpdfs,NULL,NULL,NULL)) <0)
        {
            err_log("fail to select\n");
        }
        for(i=0;i<=maxfd;i++)
        {
            if(FD_ISSET(i,&tmpdfs))
            {
                if(i == 0)
                {
                    fgets(buf, N, stdin);
                    buf[strlen(buf)-1] = '\0';

                }
            }
        }
        fgets(buf, N, stdin);
        buf[strlen(buf)-1] = '\0';

        recv(acceptfd, buf, N, 0);

        printf("server:%s\n", buf);

        if(strncmp(buf, "quit", 4) == 0)
            break;

        strcat(buf, " from server.");

        send(acceptfd, buf, N, 0);
    }

    close(acceptfd);
    close(sockfd);

    return 0;
}
