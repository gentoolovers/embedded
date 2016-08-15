#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>



#define N 128

int main(int argc,const char* argv[])
{

    int sockfd;
    int acceptfd;
    int ret_recv;
    int on=1;
    struct sockaddr_in serveraddr,clientaddr;
    char buf[N];
    int bufer_size;
    if(argc <3)
    {
        fprintf(stderr,"Usage:%s <serverip> <port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) <0)
    {
        perror("fail to socket\n");
        exit(EXIT_FAILURE);
    }
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr(argv[1]);
    serveraddr.sin_port=htons(atoi(argv[2]));

    socklen_t addrlen=sizeof(struct sockaddr);

    socklen_t optlen=sizeof(int);
    /* if((getsockopt(acceptfd, SOL_SOCKET, SO_SNDBUF,&bufer_size, &optlen)) <0)
       {
       perror("fail to getsockopt\n");
       exit(EXIT_FAILURE);
       }
    */

    struct timeval tv;
    tv.tv_sec=2;
    tv.tv_usec=0;
    if((setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,&on,optlen)) < 0)
    {
        perror("fial to setsockopt\n");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
/*
        ret_recv = recvfrom(sockfd, buf, strlen(buf), 0,(struct sockaddr*)&clientaddr, &addrlen);
        if(ret_recv <0)
        {
            perror("fail to recv\n");
            exit(EXIT_FAILURE);
        }
        printf("server:%s\n",buf);
        if(strncmp(buf,"quit",4) == 0)
        {
            break;
        }
        strcat(buf,"hello ");

*/
        printf("server:");
        fgets(buf, N, stdin);

        buf[strlen(buf)-1] = '\0';

        if(sendto(sockfd, buf, strlen(buf),0, (struct sockaddr*)&serveraddr,addrlen) < 0 )
        {
            perror("fail to send\n");
            exit(EXIT_FAILURE);
        }


    }
    return 0;
}
