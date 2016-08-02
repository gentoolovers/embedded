#include <head.h>

void *thread_function1(void *arg)
{
	int *p = (int *)arg;
	
	while(1)
	{
		(*p) ++;
		usleep(2000);
	}
}

void *thread_function2(void *arg)
{
	int *p = (int *)arg;

	while(1)
	{
		(*p) --;
		usleep(1000);
	}
}

int main(int argc, const char *argv[])
{
	int ret;
	int data = 100;
	pthread_t tid[2];


	ret = pthread_create(&tid[0],NULL,thread_function1,&data);
	if(ret != 0){
		fprintf(stderr,"Fail to pthread_create : %s\n",strerror(errno));
		return -1;
	}
	
	ret = pthread_create(&tid[1],NULL,thread_function2,&data);
	if(ret != 0){
		fprintf(stderr,"Fail to pthread_create : %s\n",strerror(errno));
		return -1;
	}

	while(1)
	{
		printf("data : %d\n",data);
		sleep(1);
	}
	
	return 0;
}
