#include <head.h>

int main(int argc, const char *argv[])
{
	int ret;
	char *env_p[] = {"PATH=ABC",NULL};

	ret = execle("./path","path",NULL,env_p);
	if(ret < 0){
		perror("Fail to execlp");
		return -1;
	}
	
	printf("after execlp\n");
	
	return 0;
}
