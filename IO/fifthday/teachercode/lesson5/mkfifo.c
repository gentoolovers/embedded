#include <head.h>

//./a.out  fifo
int main(int argc, const char *argv[])
{
	int ret;

	ret = mkfifo(argv[1],0666);
	if(ret < 0 && errno != EEXIST){
		fprintf(stderr,"Fail to mkfifo %s : %s\n",argv[1],strerror(errno));
		exit(EXIT_FAILURE);
	}
	

	return 0;
}
