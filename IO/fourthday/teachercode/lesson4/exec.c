#include <head.h>

int main(int argc, const char *argv[])
{
	int ret;

	printf("before execlp\n");

	ret = execlp("ls","ls","-l",NULL);
	if(ret < 0){
		perror("Fail to execlp");
		return -1;
	}
	
	printf("after execlp\n");
	
	return 0;
}
