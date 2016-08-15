#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define err_log(errlog) do{perror(errlog); exit(1);}while(0)

// ./server  192.168.2.196 10000

#define  N  4096

int main(int argc, const char *argv[])
{

    int sockfd;
    int acceptfd;
    int ret_recv;
    struct ip_mreq mreq;
    struct sockaddr_in serveraddr, clientaddr;
    char buf[N] = {};

    if(argc < 3)
    {
        fprintf(stderr, "Usage:%s serverip port.", argv[0]);
        return -1;
    }

    if((sockfd = socket(AF_INET,  SOCK_DGRAM, 0)) < 0)
    {
        err_log("fail to socket");
    }

    printf("sockfd = %d\n", sockfd);

    bzero(&mreq,sizeof(mreq));
    bzero(&serveraddr,sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));
    mreq.imr_multiaddr.s_addr=inet_addr(argv[1]);
    mreq.imr_interface.s_addr=htons(INADDR_ANY);

    if((setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq))) <0)
    {
        err_log("fail to setsockopt\n");
    }


    socklen_t addrlen=sizeof(struct sockaddr);

    if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        err_log("fail to bind");
    }
    while(1)
    {
/*        printf("<client>");
        fgets(buf, N, stdin);

        buf[strlen(buf)-1] = '\0';
        sendto(sockfd, buf, strlen(buf),0,(struct sockaddr* )&serveraddr,addrlen);

        if(strncmp(buf, "quit", 4) == 0)

            break;
*/
        ret_recv = recvfrom(sockfd, buf, sizeof(buf)-1, 0,(struct sockaddr*)&clientaddr, &addrlen);
        if(ret_recv <0)
        {
            err_log("fail to recv\n");
        }
        printf("%s:%d:%s\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port),buf);
    }

    close(sockfd);

    return 0;
}
