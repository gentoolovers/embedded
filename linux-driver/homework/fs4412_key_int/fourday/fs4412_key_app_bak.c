#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#define COUNT (5)
int main(int argc, const char *argv[])
{
	int ret =0;
	int fd[COUNT]
	int count[COUNT] ;
	int i=0;
	
	for(i=0;i<argc;i++){
		fd[argc] = open(argv[argc],O_RDONLY);
		if(fd<0){
			perror("Open :");
			exit(1);
		}
	}
	while(1){

		/*阻塞读取*/
		ret = read(fd,&count,sizeof(int));
		if(ret ==0){
			printf("count = %d\n",count);
		}
	}

	return 0;
}
