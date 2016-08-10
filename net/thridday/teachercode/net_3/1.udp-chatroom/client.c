#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define  N  128

#define err_log(errlog) \
	do{\
		printf("%s:%s --> %d  ", __FILE__, __func__, __LINE__);\
        perror(errlog);\
		exit(1);\
	}while(0)

typedef struct {
	char type;
	char name[32];
	char text[N];
}MSG;

int main(int argc, const char *argv[])
{

	int sockfd;
	struct sockaddr_in serveraddr, clientaddr;
	MSG msg;
	pid_t pid;
	
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


	printf("Input:");
	fgets(msg.name, 32, stdin);
	msg.name[strlen(msg.name)-1] = '\0';
	msg.type = 'L';

	if(sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		err_log("fail to sendto");
	}

	if((pid = fork()) < 0)
	{
		err_log("fail to fork");
	}
	else if(pid == 0)
	{
		while(1)
		{
			fgets(msg.text, N, stdin);
			msg.text[strlen(msg.text)-1] = '\0';

			if(strncmp(msg.text, "quit", 4) == 0)
			{
				msg.type = 'Q';
				if(sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
				{
					err_log("fail to sendto");
				}

				kill(getppid(), SIGKILL);
				close(sockfd);
				exit(0);
			}

			msg.type = 'B';
			if(sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
			{
				err_log("fail to sendto");
			}

		}
	}
	else 
	{
		while(1)
		{
			if(recvfrom(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&clientaddr, &addrlen) < 0)
			{
				err_log("fail to recvfrom");
			}
			printf("%s\n", msg.text);
		}

	}

	
	return 0;
}
