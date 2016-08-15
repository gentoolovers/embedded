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

#define err_log(errlog) do{perror(errlog); exit(1);}while(0) 

// ./server  192.168.2.196 10000

#define  N  128
typedef struct _tag_threadnode
{

    int* a;
    fd_set fds;
}threadnode;

void do_pthread(void *arg)
{
    char buf[N];
    threadnode* thread_struct=(threadnode*)arg;


        if(recv(*(thread_struct->a), buf, N, 0) < 0)
        {
            err_log("fail to recv");
        }
        printf("%s\n", buf);

        if(strncmp(buf, "quit", 4) == 0)
        {
            //...
            FD_CLR(*(thread_struct->a), &(thread_struct->fds));
            close(*(thread_struct->a));

        }

        strcat(buf, " from server.");

        if(send(*(thread_struct->a), buf,N, 0) < 0)
        {
            err_log("fail to send");
        }
free(thread_struct->a);
thread_struct->a ==NULL;

}
int main(int argc, const char *argv[])
{

    int sockfd;
    int acceptfd;
    int ret_thread;
    pthread_t* thread_id;
    struct sockaddr_in serveraddr, clientaddr;
    threadnode thread_struct;
    char buf[N] = {};
    fd_set readfds, tempfds;
    int maxfd;
    int i = 0;

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
    FD_SET(sockfd, &readfds);

    maxfd = sockfd;

    while(1)
    {

        tempfds = readfds;

        if(select(maxfd+1, &tempfds, NULL, NULL, NULL) < 0)
        {
            err_log("fail to select");
        }

        for(i = 0;i < maxfd+1; i++)
        {
            if(FD_ISSET(i, &tempfds))
            {               
                if(i == sockfd)
                {
                    if((acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr,&addrlen)) < 0)
                    {
                        err_log("fail to accept");
                    }
                    printf("acceptfd = %d\n", acceptfd);
                    printf("%s --> %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

                    FD_SET(acceptfd, &readfds);

                    maxfd = maxfd > acceptfd ? maxfd:acceptfd;

                }
                else
                {
                    thread_struct.a=(int *)malloc(sizeof(int));
					*(thread_struct.a)=i;
                    thread_struct.fds=readfds;
                   ret_thread= pthread_create(&thread_id, NULL,do_pthread,&thread_struct);
                   if(ret_thread <0)
                   {
                       err_log("fail to pthread_create\n");
                   }
                   pthread_detach(&thread_id);

                }   
            }

        }
    }

    close(acceptfd);
    close(sockfd);

    return 0;
}
