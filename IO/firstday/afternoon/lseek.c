#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

//./a.out  filename
int main(int argc, const char *argv[])
{
    int n;
    int fd;
    char *string = "hello";
    char buf[1024];
    //close(2);

    //必须有
    if(argc < 2){
        printf("Usage : %s <filename>\n",argv[0]);
        return -1;
    }
    
    fd = open(argv[1],O_RDWR | O_CREAT|O_TRUNC,0666);//文件实际权限:0666 & ~umask
    //必须有
    if(fd < 0){
        //printf("Fail to open %s\n",argv[1]);
        printf("(%d,%s),Fail to open %s : %s\n",__LINE__,__FUNCTION__,argv[1],strerror(errno));
        return -1;
    }

    printf("fd : %d\n",fd);
    
    n = write(fd,string,strlen(string));
    printf("Write %d bytes!\n",n);
    lseEk(fd,0,SEEK_SET);
    n = read(fd,buf,sizeof(buf));
    buf[n]='\0';
    printf("Read %d bytes : %s\n",n,buf);
    

    close(fd);

    return 0;
}
