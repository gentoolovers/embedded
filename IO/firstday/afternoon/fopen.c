#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

//./a.out  filename
int main(int argc, const char *argv[])
{

    int n ;
    FILE* fp;
    char* string="hello";
    char buf[50];
    if(argc < 2)
    {
        printf("Usage :%s <filename>\n",argv[0]);
        return -1;
    }
    fp=fopen(argv[1],"w+");
    if(fp<0)
    {
        printf("(%d ,%s) Fail to open %s : %s\n",__LINE__,__FUNCTION__,argv[1],strerror(errno));
        return -1;
    }
    //fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream);

    sprintf(fp,"%d:%s\n",strlen(string),string);
    printf("%d,%s\n",strlen(string),string);
    fprintf(fp,"%d,%s\n",strlen(string),string);
    close (fp);

    return 0;
}
