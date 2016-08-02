#include <head.h>

int global_data = 10;

int main(int argc, const char *argv[])
{
	pid_t pid;
	int local_data = 20;

	global_data ++;//父

	pid = fork();
	if(pid < 0){
		perror("Fail to fork");
		return -1;
	}

	local_data ++;//父，子

	if(pid == 0){
		local_data ++;//子
	}

	if(pid > 0){
		global_data ++;//父
	}
	
	if(pid == 0){
		printf("child process(%d) local_data  : %d\n",getpid(),local_data);//22
		printf("child process(%d) global_data : %d\n",getpid(),global_data);//11
	}

	if(pid > 0){
		printf("parent process(%d) local_data  : %d\n",getpid(),local_data);//21
		printf("parent process(%d) global_data : %d\n",getpid(),global_data);//12
	}
	
	
	return 0;
}
