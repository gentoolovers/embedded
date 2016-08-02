#include <head.h>

int main(int argc, const char *argv[])
{
	int ret1;
	int ret2;
	int pid;
	int status;

	ret1 = fork();
	if(ret1 < 0){
		perror("Fail to fork");
		return -1;
	}
	
	if(ret1 == 0){
		sleep(5);
		return 0x1234;
	}

	if(ret1 > 0){
			
		ret2 = fork();
		if(ret2 < 0){
			perror("Fail to fork");
			return -1;
		}

		if(ret2 == 0){
			while(1);
		}

		if(ret2 > 0){
			while(1)
			{
				printf("wait ....\n");
				pid = waitpid(-1,&status,WNOHANG);
				if(pid == -1){
					break;
				}

				printf("pid : %d\n",pid);
				
				//是否是正常结束
				if(WIFEXITED(status)){
					printf("The child process terminated normally,return value : %#x\n",WEXITSTATUS(status));
				}else if(WIFSIGNALED(status)){
					printf("The child process terminated by signal : %d\n",WTERMSIG(status));
				}	
			}
		}
	}
	
	return 0;
}
