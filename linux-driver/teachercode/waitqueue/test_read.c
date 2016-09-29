#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define N 90

int main()
{
	int i, fd;
	char buf[N] = {0};
	int num = 0;

	if ((fd = open("/dev/hello", O_RDWR)) < 0)
	{
		perror("fail to open");
		return -1;
	}
	puts("open is ok");
	if((num = read(fd, buf,12))<0)
	{
		printf("num = %d\n",buf);
		perror("read:");
	}
	puts("11111111111111111111");
	printf("num = %d \n",num);
	puts(buf);
	
	close(fd);

	return 0;
}

