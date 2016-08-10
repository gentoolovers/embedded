#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define err_log(errlog) do{perror(errlog); exit(1);}while(0) 

// ./server  192.168.2.196 10000

#define  N  128

int process_help()
{
	return 0;
}

int process_list(struct sockaddr_in serveraddr)
{
	int sockfd;
	char buf[N] = {};

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		err_log("fail to socket");
	}

	printf("sockfd = %d\n", sockfd);

	if(connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		err_log("fail to connect");
	}

	strcpy(buf, "L");

	if(send(sockfd, buf, N, 0) < 0)
	{
		err_log("fail to send");
	}

	while(recv(sockfd, buf, N, 0) > 0)
	{
		printf("%s\n", buf);
	}

	close(sockfd);

	return 0;
}

int process_download(struct sockaddr_in serveraddr, char *filename)
{
	int sockfd;
	char buf[N] = {};
	int fd;
	int nbyte;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		err_log("fail to socket");
	}

	printf("sockfd = %d\n", sockfd);

	if(connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		err_log("fail to connect");
	}

	// G filename
	sprintf(buf, "G %s", filename);

	if(send(sockfd, buf, N, 0) < 0)
	{
		err_log("fail to send");
	}

	if(recv(sockfd, buf, N, 0) < 0)
	{
		err_log("fail to recv");
	}

	// Y, N
	if(strncmp(buf, "Y", 1) == 0)
	{
		printf("recving ...\n");
	}
	else
	{
		printf("File not exist.\n");
		close(sockfd);
		return -1;
	}

	if((fd = open(filename, O_WRONLY|O_TRUNC|O_CREAT, 0664)) < 0)
	{
		err_log("fail to open");
	}

	while((nbyte = recv(sockfd, buf, N, 0)) > 0)
	{
		write(fd, buf, nbyte);
	}
	printf("download done.\n");

	close(sockfd);
	close(fd);

	return 0;
}
int process_upload(struct sockaddr_in serveraddr, char *filename)
{
	int sockfd;
	char buf[N] = {};
	int fd;
	int nbyte;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		err_log("fail to socket");
	}

	printf("sockfd = %d\n", sockfd);

	if(connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		err_log("fail to connect");
	}

	// P filename
	sprintf(buf, "P %s", filename);

	if(send(sockfd, buf, N, 0) < 0)
	{
		err_log("fail to send");
	}

	if((fd = open(filename, O_RDONLY)) < 0)
	{
		err_log("fail to open");
	}

	while((nbyte = read(fd, buf, N)) > 0)
	{
		usleep(15);
		send(sockfd, buf, nbyte, 0);
	}
	printf("upload done.\n");

	close(sockfd);
	close(fd);

	return 0;
}

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

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	while(1)
	{
		printf("<client>");
		fgets(buf, N, stdin);
		buf[strlen(buf)-1] = '\0'; // get filename , put filename

		if(strncmp(buf, "help", 4) == 0)
		{
			process_help();
		}
		else if(strncmp(buf, "list", 4) == 0)
		{
			process_list(serveraddr);
		}
		else if(strncmp(buf, "get", 3) == 0)
		{
			process_download(serveraddr, buf+4);
		}
		else if(strncmp(buf, "put", 3) == 0)
		{
			process_upload(serveraddr, buf+4);
		}
		else
		{
			printf("Input error.\n");
		}

	}

	
	return 0;
}
