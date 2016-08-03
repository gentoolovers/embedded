#include <stdio.h>
#include <pthread.h>

#define size 20
typedef struct
{
    void *pdata;
    int n;
    pthread_mutex_t lock;
    pthread_cond_t cond;
}parm_t;

void *revers_array(void *arg)
{
    int i,j;
    int tmp;
    int n=(parm_t*)arg->n;
    int *p = (int*)(parm_t*)arg->pdata;
    while(1)
    {

    }
}
int main(int argc,const char* argv[])
{
    int i,ret;
    parm_t parm;
    pthread_t tid[2];
    for(i=0;i<size;i++)
    {
        parm.pdata[i]=i;
    }
    parm.n=size;

    ret=pthread_create(&tid[0],NULL,reverse_array,&parm_t);
    if(ret != 0)
    {
        fprintf(stderr,"Fail to pthread_create:%s\n",strerror(ret));
        exit(EXIT_FAILURE);
    }
    ret=pthread_create(&tid[1],NULL,output_array,&parm);
    if(ret != 0)
    {
        fprintf(stderr,"Fail to pthread_create \n",strerror(ret));
        exit(EXIT_FAILURE);
    }

    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);

    return 0;
}
