#include <stdio.h>
#include <errno.h>
#include <string.h>
void do_copy(const char* src_file , const char* dest_file);
int main(int argc,const char* argv[])
{
    int i;
    if(argc <3)
    {
        fprintf(stderr,"Usage %s <src_fiel> <dest_file>",argv[0]);
        return -1;
    }
    do_copy(argv[1],argv[2]);
    return 0;
}
void do_copy(const char* src_file , const char* dest_file)
{
    char* gs;
    int fps;
    char buf[30];
    FILE* src_fp ,*dest_fp;
    src_fp=fopen(src_file,"r");
    if(src_fp == NULL)
    {
        fprintf(stderr,"open %s fail : %s",src_file,strerror(errno));
        return ;
    }
    dest_fp=fopen(dest_file,"a+");
    if(dest_fp == NULL)
    {
        fprintf(stderr,"open %s fail :%s",dest_file,strerror(errno));
        return ;
    }

    while(1)
    {
        //char *fgets(char *s, int size, FILE *stream);

        gs=fgets(buf,sizeof(buf),src_fp);
        if(gs == NULL)
        {
            break;
        }
        fps=fputs(buf,dest_fp);
        if(fps <0)
        {
            break;
        }
    }
fclose(src_fp);
fclose(dest_fp);
}