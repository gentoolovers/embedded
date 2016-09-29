#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#define COUNT (5)
int main(int argc, const char *argv[])
{
	fd_set readfds ; 
	
	int ret =0;
	int fd[COUNT];
	int count[COUNT] ;
	int i=0;
	
	FD_ZERO(&readfds);

	for(i=0;i<argc;i++){
		fd[i] = open(argv[i],O_RDONLY);
		if(fd[i]<0){
			perror("Open :");
			exit(1);
		}
	}
		
	

	while(1){

		/*由于每一次select返回里都会自动清空readfds ，故需要重设readfds*/
		for(i=0;i<argc;i++){
			FD_SET(fd[i],&readfds);
		}		
 		printf("the max fd = %d\n",fd[argc-1]+1);

		if( select(fd[argc-1]+1,&readfds,NULL,NULL,NULL) <0){
			perror("Select :");
			exit(1);
		}
		printf("I have listned a readfds  !\n");
		for(i=0;i<argc;i++){
			if(FD_ISSET(fd[i],&readfds)){
				/*阻塞读取*/
				printf("i = %d\n",i);
				ret = read(fd[i],&count[i],sizeof(int));
				if(ret ==0){
					printf("count[%d] = %d\n",i,count[i]);
				}
			}
		}
	}

	return 0;
}
