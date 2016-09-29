#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

int main(int num, char *arg[])
{
	char *devname = "/dev/fskey0";
	if(2 == num){
		devname = arg[1];
	}

	int fd = open(devname, O_RDWR);
	if(0 > fd){
		perror("open");
		return -1;
	}

	while(1){
		sleep(1);
		printf("------------\n");
	}
}
