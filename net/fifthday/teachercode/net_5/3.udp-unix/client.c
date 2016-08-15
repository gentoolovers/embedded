#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>

#define  N  128

#define err_log(errlog) \
	do{\
		printf("%s:%s --> %d  ", __FILE__, __func__, __LINE__);\
        perror(errlog);\
		exit(1);\
	}while(0)

int main(int argc, const char *argv[])
{

	int sockfd;
	struct sockaddr_un serveraddr, clientaddr;
	char buf[N] = {};


	if((sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0)
	{
		err_log("fail to socket");
	}

	printf("sockfd = %d\n", sockfd);

	serveraddr.sun_family = AF_LOCAL;
	strcpy(serveraddr.sun_path, "mysocket");

	clientaddr.sun_family = AF_LOCAL;
	strcpy(clientaddr.sun_path, "socket");

	if(bind(sockfd, (struct sockaddr *)&clientaddr, sizeof(clientaddr)) < 0)
	{
		err_log("fail to bind");
	}

	socklen_t  addrlen = sizeof(struct sockaddr);

	while(1)
	{
		printf("Input:");
		fgets(buf, N, stdin);
		buf[strlen(buf)-1] = '\0';
		
		if(sendto(sockfd, buf, N, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
		{
			err_log("fail to sendto");
		}

		if(strncmp(buf, "quit", 4) == 0)
			break;

		if(recvfrom(sockfd, buf, N, 0, (struct sockaddr*)&clientaddr, &addrlen) < 0)
		{
			err_log("fail to recvfrom");
		}

		printf("%s\n", buf);

	}
	system("rm -rf socket");
	
	close(sockfd);
	
	return 0;
}
