#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>

#include "fs4412_led.h"

int main(int argc, const char *argv[])
{
	int fd;
	int i = 1;

	fd = open("/dev/led", O_RDWR);
	if (fd < 0) {
		perror("open");
		exit(1);
	}

#if 0
	while(1) {
		ioctl(fd, LED_ON, &i);	
		usleep(300000);
		ioctl(fd, LED_OFF, &i);	
		usleep(300000);
		if (i++ == 4)
			i = 1;
	}
#endif

#if 1
	struct led_action action;
	while(1) {
		action.act = LED_ON;
		action.nr = i;
		write(fd, &action, sizeof(action));
		usleep(300000);
		action.act = LED_OFF;
		write(fd, &action, sizeof(action));
		if (i++ == 4)
			i = 1;
	}
#endif

	close(fd);
	
	return 0;
}
