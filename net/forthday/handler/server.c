#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>
#include <signal.h>
#include <errno.h>

#define err_log(errlog) do{perror(errlog); exit(1);}while(0) 

// ./server  192.168.2.196 10000

#define  N  128
void handler(int sig)
{

}
int main(int argc, const char *argv[])
{
    struct sigaction act;
    int sockfd;
    int acceptfd;
    struct sockaddr_in serveraddr, clientaddr;
    char buf[N] = {};
    fd_set readfds;
    int maxfd;
    int i = 0;
    int ret;
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

    FD_ZERO(&readfds);

    maxfd = sockfd;
    sigaction(SIGALRM,NULL,&act);

    while(1)
    {
        act.sa_handler=handler;
        act.sa_flags &= ~SA_RESTART;
        sigaction(SIGALRM,&act,NULL);


        FD_SET(sockfd, &readfds);
        FD_SET(0, &readfds);
        alarm(5);
        if(ret=select(maxfd+1, &readfds, NULL, NULL, NULL) < 0)
        {
            if(errno == 4)
            {
                printf("select time out\n");
            }else
            {
                err_log("fail to select");
            }


        }
        if(ret ==0)
        {
            printf("time out \n");
        }

        for(i = 0;i < maxfd+1; i++)
        {
            if(FD_ISSET(i, &readfds))
            {

                if(i == 0)
                {
                    alarm(5);
                    fgets(buf, N, stdin);
                    printf("%s", buf);
                    bzero(buf,sizeof(buf));
                }
                
                if(i == sockfd)
                {
                    alarm(5);

                    if((acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr,&addrlen)) < 0)
                    {
                        if(errno == 4)
                        {
                            printf("accept time out\n");
                        }else
                        {
                            err_log("fail to accept");
                        }

                    }
                    printf("acceptfd = %d\n", acceptfd);

                    printf("%s --> %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
                }

            
            }

        }
        printf("++++++++++\n");
    }

    close(acceptfd);
    close(sockfd);

    return 0;
}
