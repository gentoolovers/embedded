#include <head.h>

void wait_child_process(int signum)
{
	waitpid(-1,NULL,WNOHANG);//此处不可以写wait(NULL),原因?
}


int main(int argc, const char *argv[])
{
	pid_t pid;
	
#if 0
	//方法1
	if(signal(SIGCHLD,wait_child_process) == SIG_ERR){
		perror("Fail to signal");
		exit(EXIT_FAILURE);
	}
#endif 

	//方法2 
	//显示忽略SIGCHLD信号，告诉操作系统，当子进程结束的时候，不要保留僵尸态，系统直接释放资源
	if(signal(SIGCHLD,SIG_IGN) == SIG_ERR){
		perror("Fail to signal");
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if(pid < 0){
		perror("Fail to fork()");
		exit(EXIT_FAILURE);
	}

	if(pid == 0){
		sleep(5);
		exit(EXIT_SUCCESS);
	}

	if(pid > 0){
		while(1){
			printf("Hello word\n");
			sleep(1);
		}
	}
	
	return 0;
}
