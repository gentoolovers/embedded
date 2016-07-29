#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
//int open(const char *pathname, int flags);
int  main(int argc, const char *argv[])
{
    int ret;
    //close(2);
    if(argc <3)
    {
        printf(" Usage:%s <srcfile> <destfiel>",argv[1],argv[2]);
              return -1;
    }
    ret=copy_file(argv[1],argv[2]);
    if(ret<0)
    {
        printf("Fail to copy : %s to %s ",argv[1],argv[2]);
        return -1;
    }
    return 0;
}
int copy_file(const char* src_file ,const char* dest_file)
{
    char buf[1024];
    int n=0,fd_src,fd_dest;

    fd_src=open(src_file,O_RDONLY);
    if(fd_src<0)
    {
        printf("(%d,%s) failed to open %s: %s\n",__LINE__,__FUNCTION__,src_file,strerror(errno));
        return -1;
    }

    fd_dest=open(dest_file,O_WRONLY | O_CREAT | O_TRUNC,0666);
    if(fd_dest<0)
    {
        printf("(%d,%s) failed to open %s: %s",__LINE__,__FUNCTION__,dest_file,strerror(errno));
        return -1;
    }

    //printf (" srcfd  is %d\n desfd is %d\n",fdsrc,fddest);
    while(1)
    {
        n=read(fd_src,buf,sizeof(buf));
        if( 0 == n)
        {
            break;
        }
        if(n == -1)
        {

            printf("Fail to read %s :%s \n",src_file,strerror(errno));
            return -1;
        }
        n=write(fd_dest,buf,n);
        if(n == -1)
        {
            printf("fail to write %s :%s\n",dest_file,strerror(errno));
            return -1;
        }
    }
    close(fd_src);
    close(fd_dest);

    return 0;
}
