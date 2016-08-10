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

#define  N  128

int main(int argc, const char *argv[])
{

	int sockfd;
	int acceptfd;
	struct sockaddr_in serveraddr, clientaddr;
	char buf[N] = {};

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


	if(connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		err_log("fail to connect");
	}

	while(1)
	{
		fgets(buf, N, stdin);
		buf[strlen(buf)-1] = '\0';

		send(sockfd, buf, N, 0);

		if(strncmp(buf, "quit", 4) == 0)
			break;

		recv(sockfd, buf, N, 0);
		printf("%s\n", buf);
	}

	close(sockfd);

	return 0;
}
