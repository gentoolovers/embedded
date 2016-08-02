#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void insert_line_file(const char* pathname,int line,const char* contentstring)
{
    FILE* fp;
    FILE* tmp_fp;
    char buf[100];
    int swrite;
    int count=1,i,offset,size=0;
    char* enter="\n",*sgets;


    fp=fopen(pathname,"r+");
    tmp_fp=fopen(".tmpinline","w+");
    if(fp == NULL)
    {
        printf("(%d:%s) Fail to open %s:%s",__LINE__,__FUNCTION__,pathname,strerror(errno));
        return;
    }
    if(tmp_fp == NULL)
    {
        printf("(%d:%s) Fail to open %s:%s",__LINE__,__FUNCTION__,".tmpinline",strerror(errno));
        return;
    }
    while(1)
    {
        if(count >=line)
        {
            break;
        }
        sgets=fgets(buf,sizeof(buf),fp);
        if(sgets == NULL)
        {
            break;
        }

        for(i=0;i<strlen(buf);i++)
        {

            if(buf[i]=='\n');
            {
                count++;
                if(count ==line)
                {
                    break;
                }
            }


        }
    }
    offset= ftell(fp);
    while(1)
    {
        sgets=fgets(buf,sizeof(buf),fp);
        if(sgets ==NULL)
        {
            break;
        }
        swrite=fwrite(buf,strlen(buf),1,tmp_fp);
        if(swrite <=0)
        {
            break;
        }
    }
    fseek(tmp_fp, 0, SEEK_SET);
    fseek(fp,offset,SEEK_SET);
    size=fwrite(contentstring,strlen(contentstring),1,fp);

    if(size <=0)
    {
        printf("(%d:%s) Fail to fwrite %s:%s",__LINE__,__FUNCTION__,pathname,strerror(errno));
		return ;

    }

    size=fwrite(enter,strlen(enter),1,fp);

    while(1)
    {
        sgets=fgets(buf,sizeof(buf),tmp_fp);
        if(sgets ==NULL)
        {
            break;
        }
        swrite=fwrite(buf,strlen(buf),1,fp);
        if(swrite <=0)
        {
            break;
        }
    }

    fclose(tmp_fp);
    remove(".tmpinline");

    fclose(fp);

}
