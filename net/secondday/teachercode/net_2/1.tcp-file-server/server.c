#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

#define err_log(errlog) do{perror(errlog); exit(1);}while(0) 

// ./server  192.168.2.196 10000

#define  N  128

int process_list(int sockfd)
{
	DIR *dir;
	struct dirent * dp;
	char buf[N] = {};

	dir = opendir(".");
	if(dir == NULL)
	{
		err_log("fail to opendir");
	}

	while((dp = readdir(dir)) != NULL)
	{
		if(dp->d_name[0] == '.')
		{
			continue;
		}
		strcpy(buf, dp->d_name);
		send(sockfd, buf, N, 0);
		usleep(15);
	}

	close(sockfd);
	return 0;
}

int process_upload(int sockfd, char *filename)
{
	char buf[N] ={};
	int fd;
	int nbyte;

	if((fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0664)) < 0)
	{
		err_log("fail to open");
	}

	while((nbyte = recv(sockfd, buf, N, 0)) > 0)
	{
		write(fd, buf, nbyte);
	}
	printf("Receive done.\n");
	close(sockfd);

	return 0;
}
int process_download(int sockfd, char *filename)
{

	char buf[N] ={};
	int fd;
	int nbyte;

	if((fd = open(filename, O_RDONLY)) < 0)
	{
		sprintf(buf, "N fail to open %s", filename);
		// N
		send(sockfd, buf, N, 0);
	}
	else 
	{
		// Y
		strcpy(buf, "Y");
		send(sockfd, buf, N, 0);
	}

	while((nbyte = read(fd, buf, N)) > 0)
	{
		usleep(15);
		send(sockfd, buf, nbyte, 0);
	}
	printf("Transmit done.\n");
	close(sockfd);
	return 0;
}
int process_quit()
{
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

	while(1)
	{
		if((acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr,&addrlen)) < 0)
		{
			err_log("fail to accept");
		}
		printf("acceptfd = %d\n", acceptfd);
		
		recv(acceptfd, buf, N, 0);

		switch(buf[0])
		{
		case 'L':
			process_list(acceptfd);
			break;
		case 'P':
			process_upload(acceptfd, buf+2);
			break;
		case 'G':  // G filename
			process_download(acceptfd,buf+2);
			break;
		case 'Q':
			process_quit();
			break;
		default:
			printf("Error cmd.\n");
		}

	}

	close(acceptfd);
	close(sockfd);

	return 0;
}
