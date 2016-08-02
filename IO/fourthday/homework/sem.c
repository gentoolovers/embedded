#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define size 20
typedef struct _tag_args
{
    sem_t print_sem;
    sem_t reverse_sem;
    int arry[size];

}Arry;

void* thread1_function(void* arg)
{
    int i=0;
    Arry* arry1=(Arry*)arg;
    while(1)
    {
        sem_wait(&arry1->print_sem);
        printf("printf_thread\n");
            for(i=0;i<size;i++)
            {
                printf("%d ",arry1->arry[i]);
            }
            puts(" ");
            printf("-------------------------------------------\n");


            sem_post(&arry1->reverse_sem);
        sleep(1);
    }

}

void* thread2_function(void* arg)
{

    int i=0,tmp;
    Arry* arry2=(Arry*)arg;
    while(1)
    {
        sem_wait(& arry2->reverse_sem);
        printf("reverse_thread\n");
        for(i=0;i<size/2;i++)
        {
            tmp=arry2->arry[i];
            arry2->arry[i] = arry2->arry[size -1-i];
            arry2->arry[size -1-i ]=tmp;

        }

        sem_post(&arry2->print_sem);
        sleep(1);
    }

}

int main(int argc,const char* argv[])
{
    int i;


    int ret_thread1,ret_thread2;
    pthread_t thread1_id,thread2_id;



    Arry arry1;
    sem_init(&(arry1.reverse_sem),0,1);
    sem_init(&(arry1.print_sem),0,0);

    for(i=0;i<size;i++)
    {
        arry1.arry[i]=i;
    }


    ret_thread1 = pthread_create(&thread1_id,NULL,thread1_function,&arry1);
    if(ret_thread1 <0)
    {
        fprintf(stderr,"Fail to pthread_create :%s",strerror(ret_thread1));
        exit(EXIT_FAILURE);
    }

    ret_thread2 = pthread_create(&thread2_id,NULL,thread2_function,&arry1);
    if(ret_thread2 <0)
    {
        fprintf(stderr,"Fail to pthread_create :%s",strerror(ret_thread2));
        exit(EXIT_FAILURE);
    }

    pthread_join(thread1_id,NULL);
    pthread_join(thread2_id,NULL);


    return 0;
}
