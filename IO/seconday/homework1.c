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


void delete_line_file(const char* pathname,int line)
{
    FILE* fp;
    FILE* tmp_fp;
    char buf[100];
    int count=1,i,swrite=0;
    char* sgets;


    fp=fopen(pathname,"r");
    tmp_fp=fopen(".tmpdline","w+");
    if(fp == NULL)
    {
        printf("(%d:%s) Fail to open %s:%s",__LINE__,__FUNCTION__,pathname,strerror(errno));
        return;
    }
    if(tmp_fp == NULL)
    {
        printf("(%d:%s) Fail to open %s:%s",__LINE__,__FUNCTION__,".tmpdline",strerror(errno));
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
        swrite=fwrite(buf,strlen(buf),1,tmp_fp);
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

while(1)
{
    sgets=fgets(buf,sizeof(buf),fp);
    if(sgets == NULL)
    {
        break;
    }
    for(i=0;i<sizeof(buf);i++)
    {
        if(buf[i] == '\n')
        {
            break;
        }
    }
    if(buf[i] == '\n')
    {
        break;
    }
}
while(1)
{
    sgets=fgets(buf,sizeof(buf),fp);
    if(sgets == NULL)
    {
        break;
    }
    swrite=fwrite(buf,strlen(buf),1,tmp_fp);
}
fclose(fp);
fp=fopen(pathname,"w+");
if(fp == NULL)
{
    printf("(%d:%s) Fail to open %s:%s",__LINE__,__FUNCTION__,pathname,strerror(errno));
    return;
}
fseek(tmp_fp, 0, SEEK_SET);
while(1)
{
    sgets=fgets(buf,sizeof(buf),tmp_fp);
    if(sgets == NULL)
    {
        break;
    }
    swrite=fwrite(buf,strlen(buf),1,fp);
}

while(1)
{
    sgets=fgets(buf,sizeof(buf),fp);
    if(sgets == NULL)
    {
        break;
    }
    swrite=fwrite(buf,strlen(buf),1,tmp_fp);
}
fclose(fp);
fclose(tmp_fp);
remove(".tmpdline");

}
int main(int argc,const char* argv[])
{
    insert_line_file(argv[1],0,"20");
    //delete_line_file(argv[1],1);

    return 0;
}
