#include <head.h>

int copy_file_init(const char *src_file,const char *dest_file)
{
	int fd_src;
	int fd_dest;
	int src_file_len;
	
	fd_src = open(src_file,O_RDONLY);
	if(fd_src < 0){
		fprintf(stderr,"Fail to open %s : %s\n",src_file,strerror(errno));
		exit(EXIT_FAILURE);
	}

	src_file_len = lseek(fd_src,0,SEEK_END);

	fd_dest = open(dest_file,O_WRONLY | O_TRUNC | O_CREAT,0666);
	if(fd_dest < 0){
		fprintf(stderr,"Fail to open %s : %s\n",dest_file,strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	close(fd_src);
	close(fd_dest);

	return src_file_len;
}

int copy_file(const char *src_file,const char *dest_file,int postion,int len)
{
	int n;
	int fd_src;
	int fd_dest;
	int count = 0;
	char buf[1024];

	fd_src = open(src_file,O_RDONLY);
	if(fd_src < 0){
		fprintf(stderr,"Fail to open %s : %s\n",src_file,strerror(errno));
		exit(EXIT_FAILURE);
	}

	fd_dest = open(dest_file,O_WRONLY);
	if(fd_dest < 0){
		fprintf(stderr,"Fail to open %s : %s\n",dest_file,strerror(errno));
		exit(EXIT_FAILURE);
	}

	lseek(fd_src,postion,SEEK_SET);
	lseek(fd_dest,postion,SEEK_SET);
	
	while(1){
		n = read(fd_src,buf,sizeof(buf));
		if(n <= 0){
			break;
		}
		
		n = write(fd_dest,buf,n);
		if(n <= 0){
			break;
		}

		count += n;

		if(count >= len){
			break;
		}
	}
	
	close(fd_src);
	close(fd_dest);

	return count;
}

void process_copy_file(const char *src_file,const char *dest_file,int src_file_len)
{
	int n;
	pid_t pid;

	pid = fork();
	if(pid < 0){
		perror("Fail to fork");
		exit(EXIT_FAILURE);
	}

	if(pid == 0){
		n = copy_file(src_file,dest_file,src_file_len/2,src_file_len / 2);
		printf("Child process copy %d bytes!\n",n);
	}

	if(pid > 0){
		n = copy_file(src_file,dest_file,0,src_file_len - src_file_len / 2);
		printf("Parent process copy %d bytes!\n",n);
	}
	
	return ;
}

//./a.out src_file dest_file
int main(int argc, const char *argv[])
{
	int src_file_len;

	if(argc < 3){
		fprintf(stderr,"Usage : %s <src file> <dest file>!\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	src_file_len = copy_file_init(argv[1],argv[2]);
	process_copy_file(argv[1],argv[2],src_file_len);
		
	exit(EXIT_SUCCESS);
}
