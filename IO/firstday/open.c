#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
//int open(const char *pathname, int flags);
int  main(int argc, const char *argv[])
{
    //close(2);
    if(argc <2)
    {
        printf(" Usage:%s <filename>\n",argv[1]);
              return -1;
    }
    

int fd= open("/home/linux/github/embedded/IO/firstday/test.c",
        O_RDWR | O_TRUNC | O_CREAT ,0666);
if(fd<0)
{
    printf("(%d,%s) failed to open %s: %s",__LINE__,__FUNCTION__,argv[1],strerror(errno));
    return -1;
}
printf (" fd  is %d\n",fd);

    return 0;
}
