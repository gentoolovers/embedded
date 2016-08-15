#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
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

	int sockfd;
	int acceptfd;
	struct sockaddr_in serveraddr, clientaddr;
	char buf[N] = {};
	struct sigaction act;

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

	sigaction(SIGALRM, NULL, &act);

	act.sa_handler = handler;
	act.sa_flags &= ~SA_RESTART;

	sigaction(SIGALRM, &act, NULL);

	while(1)
	{
		alarm(5);

		if((acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr,&addrlen)) < 0)
		{
			printf("errno = %d\n", errno);
			if(errno == 4)
			{
				printf("accept timeout.\n");
			}
			else
			{
				err_log("fail to accept");
			}
		}
		else
		{
			printf("%s --> %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

			recv(acceptfd, buf, N, 0);

			printf("server:%s\n", buf);

			if(strncmp(buf, "quit", 4) == 0)
				break;

			strcat(buf, " from server.");

			send(acceptfd, buf, N, 0);

		}
		printf("acceptfd = %d\n", acceptfd);
/*
		alarm(5);
		fgets(buf, N, stdin);
		printf("+++++++\n");
*/

	}

	close(acceptfd);
	close(sockfd);

	return 0;
}
