#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define  N  128

#define err_log(errlog)                                         \
    do{                                                         \
        printf("%s:%s --> %d  ", __FILE__, __func__, __LINE__); \
        perror(errlog);                                         \
        exit(1);                                                \
    }while(0)

int main(int argc, const char *argv[])
{

    int sockfd;
    struct sockaddr_in serveraddr, clientaddr;
    char buf[N] = {};
    char msg[N];

    if(argc < 3)
    {
        fprintf(stderr, "usage:%s serverip port.\n", argv[0]);
        return -1;
    }

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        err_log("fail to socket");
    }

    printf("sockfd = %d\n", sockfd);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));


    socklen_t  addrlen = sizeof(struct sockaddr);

    printf("Input username:");
    fgets(buf, N, stdin);
    buf[strlen(buf)-1] = '\0';
    sprintf(msg,"L%s",buf);

    if(sendto(sockfd, msg, N, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        err_log("fail to sendto");
    }

    while(1)
    {
        printf("Input:");
        fgets(buf, N, stdin);
        buf[strlen(buf)-1] = '\0';
        sprintf(msg,"C%s",buf);

        if(strncmp(buf, "quit", 4) == 0)
        {
            sprintf(msg,"Q%s",buf);

            if(sendto(sockfd, msg, N, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
            {
                err_log("fail to sendto");
            }
            break;
        }


        if(sendto(sockfd, msg, N, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        {
            err_log("fail to sendto");
        }



    }

    close(sockfd);

    return 0;
}
