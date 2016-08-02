#include <head.h>

#define DEAMON_MODE 1

void deamon_mode()
{
	int fd;
	pid_t pid;
	
	fd = open("/home/linux/highclass/lesson4/log.txt",O_WRONLY | O_CREAT | O_APPEND,0666);
	if(fd < 0){
		fprintf(stderr,"Fail to open log.txt : %s\n",strerror(errno));
		return ;
	}

	pid = fork();
	if(pid < 0){
		perror("Fail to fork");
		exit(EXIT_FAILURE);
	}

	if(pid > 0){
		exit(EXIT_SUCCESS);
	}
	
	if(setsid() < 0){
		perror("Fail to setsid()");
		exit(EXIT_FAILURE);
	}
	
	if(chdir("/") < 0){
		perror("Fail to chdir /");
		exit(EXIT_FAILURE);
	}

	if(umask(0) < 0){
		perror("Fail to umask 0\n");
		exit(EXIT_FAILURE);
	}

	close(0);
	close(1);
	close(2);
	
	/*
	 *1  stdout
	 *2  stderr
	 *将标准输出，标准出错重定向到的log.txt 
	 */
	 dup2(fd,1);
	 dup2(fd,2);

	return;
}

//./a.out  mode:0->非守护进程模式  1:->守护进程模式
int main(int argc, const char *argv[])
{
	int mode;
	int fd;

	if(argc < 2){
		fprintf(stderr,"Usage : %s < mode >\n",argv[0]);
		return -1;
	}

	mode = atoi(argv[1]);
	if(mode == DEAMON_MODE){
		deamon_mode();
	}
	
	fd = open("test.txt",O_CREAT | O_WRONLY | O_TRUNC,0666);
	if(fd < 0){
		fprintf(stderr,"Fail to open test.txt : %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("open success !\n");

	return 0;
}
