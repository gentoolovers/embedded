#include <head.h>

void handler(int signum)
{

}

int main(int argc, const char *argv[])
{
	if(signal(SIGINT,handler) == SIG_ERR){
		perror("Fail to signal");
		return -1;
	}
	printf("Wait a signal!\n");
	pause();
	printf("Ok,go\n");
	
	return 0;
}
