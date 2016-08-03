#include <head.h>

int main(int argc, const char *argv[])
{
	int ret;
	int fd[2];
	int n;
	char buf[1024];
	char *string = "hello word";

	ret = pipe(fd);
	if(ret < 0){
		perror("Fail to pipe\n");
		return -1;
	}
	
	close(fd[0]);

	write(fd[1],string,strlen(string));

	printf("write success!\n");

	n = read(fd[0],buf,sizeof(buf) - 1);
	buf[n] = '\0';

	printf("Read %d bytes,buf : %s\n",n,buf);
	
	//close(fd[1]);
	
	n = read(fd[0],buf,sizeof(buf) - 1);
	buf[n] = '\0';

	printf("Read %d bytes,buf : %s\n",n,buf);
	
	return 0;
}
