#include <head.h>

int main(int argc, const char *argv[])
{
	int ret1;
	int ret2;

	ret1 = fork();
	if(ret1 < 0){
		perror("Fail to fork");
		return -1;
	}
	
	if(ret1 == 0){
		sleep(10);
		return 0;
	}

	if(ret1 > 0){
			
		ret2 = fork();
		if(ret2 < 0){
			perror("Fail to fork");
			return -1;
		}

#if 0		
		if(ret2 == 0){
			while(1);
		}

		if(ret2 > 0){
			while(1);
		}
#endif 

		while(1);
	}
	
	return 0;
}
