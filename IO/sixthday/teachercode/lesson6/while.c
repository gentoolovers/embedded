#include <head.h>

void handler(int signum)
{
	printf("signum:%d ,hello word!\n",signum);
}

int main(int argc, const char *argv[])
{
	if(signal(SIGINT,handler) == SIG_ERR){
		perror("Fail to signal");
		return -1;
	}

	while(1);
	
	return 0;
}
