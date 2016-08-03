#include <head.h>

void handler(int signum)
{
	printf("Game over!\n");
	exit(EXIT_SUCCESS);
}

int main(int argc, const char *argv[])
{
	if(signal(SIGALRM,handler) == SIG_ERR){
		perror("Fail to signal");
		exit(EXIT_FAILURE);
	}

	alarm(3);

	while(1){
		printf("hello word\n");
		sleep(1);
	}
	
	return 0;
}
