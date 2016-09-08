#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fs4412_led.h"
#include <sys/ioctl.h>

int main(int argc,const char* argv[])
{
    int fd;
    int i=2;
    char buf[1024] = "hello world";
    fd=open("/dev/led",O_RDWR);
    if(fd <0)
    {
        perror("open");
        exit(1);
    }
    while(1)
    {
        ioctl(fd, LED_ON, &i);
        usleep(300000);
        ioctl(fd, LED_OFF, &i);
        usleep(300000);
        if (i == 5)
        {
            i = 2;
        }else
        {
            i++;
        }


    }

}
