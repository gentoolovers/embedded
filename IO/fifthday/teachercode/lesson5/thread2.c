#include <head.h>
#include <pthread.h>

typedef struct{
	void *pdata;
	int  n;
	//pthread_mutex_t lock;
}parm_t;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void thread_clean(void *arg)
{
	printf("hello\n");
	pthread_mutex_unlock((pthread_mutex_t *)arg);
}

void *reverse_array(void *arg)
{
	int i,j;
	int tmp;
	int n  = ((parm_t *)arg)->n;
	int *p = (int *)(((parm_t *)arg)->pdata); 
	
	while(1)
	{
		//获得锁
		
		pthread_cleanup_push(thread_clean,(void *)&lock);
		pthread_mutex_lock(&lock);
		printf("reverse_array get lock ok!\n");
		i = 0;
		j = n - 1;

		while(i < j){
			tmp  = p[i];
			p[i] = p[j];
			p[j] = tmp;

			i ++;
			j --;
		}
		
		pthread_exit(NULL);

		//释放锁
		pthread_mutex_unlock(&lock);

		pthread_cleanup_pop(0);
		printf("reverse_array put lock ok!\n");
	}

}

void *output_array(void *arg)
{
	int i;
	int n  = ((parm_t *)arg)->n;
	int *p = (int *)(((parm_t *)arg)->pdata); 
	
	while(1)
	{
		//获得锁 
		pthread_mutex_lock(&lock);
		printf("output_array get lock ok !\n");

		for(i = 0;i < n;i ++){
			printf("%d ",p[i]);
		}
		printf("\n");
		//释放锁 
	
		pthread_mutex_unlock(&lock);
		printf("output_array put lock ok!\n");
	}
}

int main()
{
	int ret;
	parm_t parm;
	pthread_t tid[2];
	int a[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	
	parm.pdata = a;
	parm.n     = sizeof(a)/sizeof(a[0]);

	ret = pthread_create(&tid[0],NULL,reverse_array,&parm);
	if(ret != 0){
		fprintf(stderr,"Fail to pthread_create : %s\n",strerror(ret));
		exit(EXIT_FAILURE);
	}

	ret = pthread_create(&tid[1],NULL,output_array,&parm);
	if(ret != 0){
		fprintf(stderr,"Fail to pthread_create : %s\n",strerror(ret));
		exit(EXIT_FAILURE);
	}
	
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

	exit(EXIT_SUCCESS);
}