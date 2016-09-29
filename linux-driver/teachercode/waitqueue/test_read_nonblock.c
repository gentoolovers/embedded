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

	if ((fd = open("/dev/hello", O_RDWR|O_NONBLOCK)) < 0)
	{
		perror("fail to open");
	}
	if((num = read(fd, buf, N))<0)
	{
		printf("num = %d \n",num);
		perror("read:");
	}
	puts(buf);
	close(fd);

	return 0;
}

