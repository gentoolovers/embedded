#include <head.h>

void *thread_function(void *arg)
{
	while(1){
		printf("thread : Hello wrod\n");
		sleep(1);
	}
}

int main(int argc, const char *argv[])
{
	int ret;
	pthread_t thread_id;

	ret = pthread_create(&thread_id,NULL,thread_function,NULL);
	if(ret != 0){
		fprintf(stderr,"Fail to pthread_create : %s\n",strerror(ret));
		exit(EXIT_FAILURE);
	}
	
	while(1){
		printf("main \n");
		sleep(1);
	}
	exit(EXIT_SUCCESS);
}
