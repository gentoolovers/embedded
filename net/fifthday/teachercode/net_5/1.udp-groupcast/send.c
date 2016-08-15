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

#define err_log(errlog) \
	do{\
		printf("%s:%s --> %d  ", __FILE__, __func__, __LINE__);\
        perror(errlog);\
		exit(1);\
	}while(0)

int main(int argc, const char *argv[])
{

	int sockfd;
	struct sockaddr_in groupcastaddr, clientaddr;
	char buf[N] = {};

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

	groupcastaddr.sin_family = AF_INET;
	groupcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
	groupcastaddr.sin_port = htons(atoi(argv[2]));


	socklen_t  addrlen = sizeof(struct sockaddr);

	while(1)
	{
		printf("Input:");
		fgets(buf, N, stdin);
		buf[strlen(buf)-1] = '\0';
		
		if(sendto(sockfd, buf, N, 0, (struct sockaddr *)&groupcastaddr, sizeof(groupcastaddr)) < 0)
		{
			err_log("fail to sendto");
		}

		if(strncmp(buf, "quit", 4) == 0)
			break;

	}
	
	close(sockfd);
	
	return 0;
}
