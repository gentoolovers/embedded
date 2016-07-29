#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
/* pathname 文件名 ,line 行号, contentstring 插入的字符串
1.拿到行号,当行号等于line的时候插入,假如到达了文件末尾还不等于line则插入到末尾.

 */
typedef char LinkQueueData;
typedef void LinkQueue;
typedef struct _tag_LinkQueue
{

    LinkQueueData* data;
    struct  _tag_LinkQueue* next;
}TLinkQueue;

LinkQueue* LinkQueue_Create()
{

    TLinkQueue* ret=(TLinkQueue*)malloc(sizeof(TLinkQueue));
    if(ret == NULL)
    {
        printf("malloc LinkQueue failed ");
        return NULL;
    }else
    {
        ret->data=NULL;
        ret->next=NULL;
    }
    return ret;
}

int  LinkQueue_Append(LinkQueue* queue,LinkQueueData* data)
{

    TLinkQueue* TLQueue=(TLinkQueue*)queue;
    int i;
    if(TLQueue != NULL)
    {

        return -1;
    }else
    {
        TLinkQueue* node=(TLinkQueue*)malloc(sizeof(TLinkQueue));
        if(node == NULL)
        {
            printf("Insert LinkQueue failed");
            return -1;
        }else
        {
            TLinkQueue* current=TLQueue;
            while(current->next != NULL)
            {
                current=current->next;
            }
            node->data=data;
            node->next=current->next;
            current->next=node;
            TLQueue->data ++;

        }

    }
    return 0;
}

LinkQueueData* LinkQueue_Retrieve(LinkQueue* queue)
{
    TLinkQueue* TLQueue=(TLinkQueue*)queue;
    TLinkQueue* tmp;
    if(TLQueue == NULL)
    {
        return  NULL;

    }else
    {
        tmp =TLQueue->next;
        TLQueue->next=tmp->next;
        TLQueue->data --;
    }
    return tmp->data;

}
void LinkQueue_Destroy(LinkQueue* queue)
{

    TLinkQueue* TLQueue=(TLinkQueue*)queue;
    if(TLQueue != NULL)
    {

        TLinkQueue* current=TLQueue;
        TLQueue=TLQueue->next;
        free(current);
        current == NULL;

    }
}
int LinkQueue_Length(LinkQueue* queue)
{
    TLinkQueue* TLQueue=(TLinkQueue*)queue;
    int ret =-1;
    while(TLQueue !=NULL)
    {
        ret++;
        TLQueue=TLQueue->next;
    }
    return ret;
}


void insert_line_file(const char* pathname,int line,const char* contentstring)
{
    char buf[50];
    char* string;
    FILE* fp,*fp1;
    int count=1,size,offset=0,i;
    fp=fopen(pathname,"r+");
    if(fp == NULL)
    {
        printf("(%d:%s) Fail to open %s:%s",__LINE__,__FUNCTION__,pathname,strerror(errno));
        return;
    }
    while(1)
    {
        string=fgets(buf,sizeof(buf),fp);
        if(string == NULL)
        {
            break;
        }
        if(count ==line)
        {
            break;
        }
        for(i=0;i<sizeof(buf);i++)
        {

            if(buf[i]=='\n');
            {
                count++;
            }


        }

    }

    fp1=fp;
   LinkQueue* queue = LinkQueue_Create();

    while(1)
    {

        string=fgets(buf,sizeof(buf),fp);
        if(string == NULL)
        {
            break;
        }
        LinkQueue_Append(queue,buf);
    }


        size=fwrite(contentstring,strlen(contentstring),1,fp1);

        while(LinkQueue_Length(queue))
        {
           string = LinkQueue_Retrieve(queue);
            fwrite(string,strlen(string),1,fp1);
            if(string == NULL)
            {
                break;
            }

        }
  LinkQueue_Destroy(queue);
    fclose(fp);
}

void delete_line_file(const char* pathname,int line)
{
    char buf[100];
    char* string;
    FILE* fp;
    int count=0,size,i;
    fp=fopen(pathname,"w+");
    if(fp == NULL)
    {
        printf("(%d:%s) Fail to open %s:%s",__LINE__,__FUNCTION__,pathname,strerror(errno));
        return;
    }
    while(1)
    {
        string=fgets(buf,sizeof(buf),fp);
        if(string == NULL)
        {
            break;
        }
        for(i=0;i<sizeof(buf);i++)
        {
            if(buf[i]=='\n');
            {
                count++;
            }
            if(count ==line)
            {
                break;
            }

        }
    }
    bzero(buf,sizeof(buf));

    while(1)
    {
        string=fgets(buf,sizeof(buf),fp);
        if(string == NULL)
        {
            break;
        }
        for(i=0;i<sizeof(buf);i++)
        {
            if(buf[i] != '\n')
            {
                buf[i]=' ';
            }else
            {
                break;
            }
        }
    }

    fclose(fp);
}

int main(int argc,const char* argv[])
{
     insert_line_file(argv[1],2,"test");
     //delete_line_file(argv[1],1);

    return 0;
}
