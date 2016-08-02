#include <head.h>

int main(int argc, const char *argv[])
{
	int ret;

	ret = fork();
	if(ret < 0){
		perror("Fail to fork");//Fail to fork : 失败原因
		return -1;
	}

	//此时已经有两个进程

	if(ret == 0){
		printf("Hello 123\n");
	}

	if(ret > 0){
		printf("Hello 456\n");
	}

	printf("Hello word!\n");

	return 0;
}
