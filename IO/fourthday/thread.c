#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void* thread_id1_function(void* arg)
{

    while(1)
    {
        (*((int*)arg))++;
        // int* p=((int*)arg);
        //(*p)++;
        sleep(1);
    }
}
void* thread_id2_function(void* arg)
{
    while(1)
    {
        (*((int*)arg))--;
        sleep(1);
    }
}
int main(int argc,const char* argv[])
{
    int ret1, ret2;
    int a=66;
    pthread_t thread_id1,thread_id2;

    ret1=pthread_create(&thread_id1,NULL,thread_id1_function,&a);

    if(ret1 != 0)
    {
        fprintf(stderr,"Fail to pthread_create :%s",strerror(ret1));
        return -1;
    }

    ret2=pthread_create(&thread_id2,NULL,thread_id2_function,&a);

    if(ret2 != 0)
    {
        fprintf(stderr,"Fail to pthread_create thread2 :%s",strerror(ret2));
        return -1;
    }
    while(1)
    {
        printf("%d\n",a);
        sleep(1);
    }

}
