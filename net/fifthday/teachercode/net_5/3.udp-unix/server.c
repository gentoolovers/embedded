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
		printf("%s:%s --> %d  \n", __FILE__, __func__, __LINE__);\
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

	if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		err_log("fail to bind");
	}

	socklen_t  addrlen = sizeof(struct sockaddr);

	while(1)
	{
		
		if(recvfrom(sockfd, buf, N, 0, (struct sockaddr*)&clientaddr, &addrlen) < 0)
		{
			err_log("fail to recvfrom");
		}

		printf("%s\n", buf);

		if(strncmp(buf, "quit", 4) == 0)
			break;

		strcat(buf, " from server.");

		if(sendto(sockfd, buf, N, 0, (struct sockaddr *)&clientaddr, addrlen) < 0)
		{
			err_log("fail to sendto");
		}

	}

	sleep(1);

	system("rm -f mysocket");
	
	close(sockfd);
	
	return 0;
}
