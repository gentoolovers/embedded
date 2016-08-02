#include <head.h>

void *thread_function(void *arg)
{
	static int data = 100;

	pthread_exit(&data);
	
}

int main(int argc, const char *argv[])
{
	int ret;
	int count = 0;
	int *pdata;
	pthread_t thread_id;
	
	
	ret = pthread_create(&thread_id,NULL,thread_function,NULL);
	if(ret != 0){
		fprintf(stderr,"Fail to pthread_create : %s\n",strerror(ret));
		exit(EXIT_FAILURE);
	}
	
	pthread_join(thread_id,&pdata);

	printf("*pdata = %d\n",*pdata);

	exit(EXIT_SUCCESS);
}
