#include <head.h>

void write_pipe(int fd)
{
	char buf[1024];
	
	while(1){
		fgets(buf,sizeof(buf),stdin);
		buf[strlen(buf) - 1] = '\0';

		write(fd,buf,strlen(buf));

		if(strncmp(buf,"quit",4) == 0){
			break;
		}
	}
}

void read_pipe(int fd)
{
	int n;
	char buf[1024];
	
	while(1){
		n = read(fd,buf,sizeof(buf) - 1);
		buf[n] = '\0';

		printf("Read %d bytes : %s\n",n,buf);
		if(n == 0 || strncmp(buf,"quit",4) == 0){
			break;
		}
	}
}

int main(int argc, const char *argv[])
{
	int fd[2];
	pid_t pid;

	if(pipe(fd) < 0){
		fprintf(stderr,"Fail to pipe : %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if(pid < 0){
		perror("Fail to fork");
		exit(EXIT_FAILURE);
	}

	if(pid == 0 ){
		close(fd[1]);
		read_pipe(fd[0]);
	}

	if(pid > 0){
		close(fd[0]);
		write_pipe(fd[1]);
	}
	
	return 0;
}
