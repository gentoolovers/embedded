#include <head.h>

void *write_fifo(void *arg)
{
	char buf[1024];
	int fd = *(int *)arg;
	
	free(arg);

	while(1){
		fgets(buf,sizeof(buf),stdin);
		buf[strlen(buf) - 1] = '\0';

		write(fd,buf,strlen(buf));

		if(strncmp(buf,"quit",4) == 0){
			break;
		}
	}
}

void *read_fifo(void *arg)
{
	int n;
	int fd = *(int *)arg;
	char buf[1024];
	
	free(arg);

	while(1){
		n = read(fd,buf,sizeof(buf) - 1);
		buf[n] = '\0';

		printf("Read %d bytes : %s\n",n,buf);
		if(n == 0 || strncmp(buf,"quit",4) == 0){
			break;
		}
	}
}


pthread_t create_fifo_thread(const char *fifoname,int flag,void* (*start_routine)(void *))
{
	int ret;
	int *pfd;
	pthread_t tid;

	ret = mkfifo(fifoname,0666);
	if(ret < 0 && errno != EEXIST){
		fprintf(stderr,"Fail to mkfifo %s : %s\n",fifoname,strerror(errno));
		exit(EXIT_FAILURE);
	}

	pfd = (int *)malloc(sizeof(int));
	if(pfd == NULL){
		perror("Fail to malloc");
		exit(EXIT_FAILURE);
	}

	*pfd  = open(fifoname,flag);
	if(*pfd  < 0){
		fprintf(stderr,"Fail to open %s : %s\n",fifoname,strerror(errno));
		exit(EXIT_FAILURE);
	}


	ret = pthread_create(&tid,NULL,start_routine,pfd);
	if(ret != 0){
		fprintf(stderr,"Fail to pthread_create %s\n",strerror(ret));
		exit(EXIT_FAILURE);
	}

	return tid;
}

//./a.out  fifo1 fifo2
int main(int argc, const char *argv[])
{
	pthread_t tid[2];
	
	tid[0] = create_fifo_thread(argv[1],O_RDONLY,read_fifo);
	tid[1] = create_fifo_thread(argv[2],O_WRONLY,write_fifo);

	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

	return 0;
}
