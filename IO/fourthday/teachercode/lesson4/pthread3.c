#include <head.h>

void *thread_function(void *arg)
{
	int *p = (int *)arg;

	printf("%d\n",*p);

	free(arg);
	return;
}

int main(int argc, const char *argv[])
{
	int ret;
	int *p;
	int i = 0;
	int a[] = {1,2,3,4,5,6,7,8,9,10};
	pthread_t thread_id;
	

	for(i = 1;i <= 10;i ++)
	{
		p = (int *)malloc(sizeof(int));
		*p = i;

		//ret = pthread_create(&thread_id,NULL,thread_function,&i);//为什么有bug
		ret = pthread_create(&thread_id,NULL,thread_function,p);
		if(ret != 0){
			fprintf(stderr,"Fail to pthread_create : %s\n",strerror(ret));
			free(p);
			exit(EXIT_FAILURE);
		}
	}
		
	while(1);

	exit(EXIT_SUCCESS);
}
