#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>

#include "hello.h"

int main(int argc, const char *argv[])
{
	int fd;
	char buf[1024] = "hello world";

	fd = open("/dev/hello", O_RDWR);
	if (fd < 0) {
		perror("open");
		exit(1);
	}

	ioctl(fd, HELLO_CMD1);
	ioctl(fd, HELLO_CMD2, &fd);

	close(fd);
	
	return 0;
}
