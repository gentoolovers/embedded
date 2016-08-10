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
		printf("%s:%s --> %d  \n", __FILE__, __func__, __LINE__);\
        perror(errlog);\
		exit(1);\
	}while(0)

typedef struct {
	char type;
	char name[32];
	char text[N];
}MSG;

typedef struct node {
	struct sockaddr_in clientaddr;
	struct node *next;
}linknode, *linklist;

int process_login(int sockfd, MSG msg,linklist h,struct sockaddr_in clientaddr)
{
	linklist temp;

	sprintf(msg.text, "%s login.", msg.name);
	while(h->next != NULL)
	{
		sendto(sockfd, &msg, sizeof(msg),0, (struct sockaddr *)&h->next->clientaddr, sizeof(struct sockaddr));

		h = h->next;
	}

	temp = (linklist)malloc(sizeof(linknode));
	temp->clientaddr = clientaddr;

	temp->next = h->next;
	h->next = temp;

	return 0;
}

int process_broadcast(int sockfd, MSG msg,linklist h,struct sockaddr_in clientaddr)
{
	linklist temp;
	char buf[N] = {};
	linklist p = h->next;

	sprintf(buf, "%s said %s.", msg.name, msg.text);
	strcpy(msg.text, buf);

	while(p != NULL)
	{
		if(memcmp(&clientaddr, &p->clientaddr, sizeof(clientaddr)) == 0)
		{
			p = p->next;
			continue;
		}
		sendto(sockfd, &msg, sizeof(msg),0, (struct sockaddr *)&p->clientaddr, sizeof(struct sockaddr));
		
		p = p->next;
	}

	return 0;
}

int process_quit(int sockfd, MSG msg,linklist h,struct sockaddr_in clientaddr)
{
	linklist temp;
	linklist p = h;

	sprintf(msg.text, "%s quit.", msg.name);

	while(p->next != NULL)
	{
		if(memcmp(&clientaddr, &p->next->clientaddr, sizeof(clientaddr)) == 0)
		{
			temp = p->next;
			p->next = temp->next;
			free(temp);
			temp = NULL;
		}
		else
		{
			sendto(sockfd, &msg, sizeof(msg),0, (struct sockaddr *)&p->next->clientaddr, sizeof(struct sockaddr));	
			p = p->next;
		}
	}

	return 0;
}

linklist create_linklist(void)
{
	linklist h = (linklist)malloc(sizeof(linknode));
	h->next = NULL;

	return h;
}


int main(int argc, const char *argv[])
{

	int sockfd;
	struct sockaddr_in serveraddr, clientaddr;
	pid_t pid;
	MSG msg;
	linklist h;

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

	if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		err_log("fail to bind");
	}

	socklen_t  addrlen = sizeof(struct sockaddr);


	if((pid = fork()) < 0)
	{
		err_log("fail to fork");
	}
	else if(pid == 0)
	{
		h = create_linklist();

		while(1)
		{
			if(recvfrom(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&clientaddr, &addrlen) < 0)
			{
				err_log("fail to recvfrom");
			}
			switch(msg.type)
			{
			case 'L':
				process_login(sockfd, msg, h, clientaddr);
				break;
			case 'B':
				process_broadcast(sockfd,msg, h, clientaddr);
				break;
			case 'Q':
				process_quit(sockfd, msg, h, clientaddr);
				break;
			default:
				printf("Error msg.\n");
			}
		}

	}
	else
	{
		while(1)
		{
			msg.type = 'B';
			strcpy(msg.name, "server");
			fgets(msg.text, N, stdin);
			msg.text[strlen(msg.text) -1] = '\0';

			if(sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&serveraddr, addrlen) < 0)
			{
				err_log("fail to sendto");
			}

		}

	}

	close(sockfd);
	
	return 0;
}
