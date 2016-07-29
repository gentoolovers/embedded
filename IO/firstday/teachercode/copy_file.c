#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int copy_file(const char *src_file,const char *dest_file)
{
	int n = 0;
	char buf[1024];
	int fd_src,fd_dest;

	fd_src = open(src_file,O_RDONLY);
	if(fd_src < 0){
		printf("Fail to open %s : %s\n",src_file,strerror(errno));
		return -1;
	}

	fd_dest = open(dest_file,O_WRONLY | O_CREAT | O_TRUNC,0666);
	if(fd_dest < 0){
		printf("Fail to open %s : %s\n",dest_file,strerror(errno));
		return -1;
	}

	while(1){
		n = read(fd_src,buf,sizeof(buf));
		if(n == 0){
			break;
		}

		if(n == -1){
			printf("Fail to read  %s : %s\n",src_file,strerror(errno));
			return -1;
		}

		n = write(fd_dest,buf,n);
		if(n == -1){
			printf("Fail to write %s : %s\n",dest_file,strerror(errno));
			return -1;
		}
	}

	close(fd_src);
	close(fd_dest);

	return 0;
}

//./a.out src_file dest_file
int main(int argc, const char *argv[])
{
	int ret;

	if(argc < 3){
		printf("Usage : %s <src file> <dest file>\n",argv[0]);
		return -1;
	}

	ret = copy_file(argv[1],argv[2]);
	if(ret < 0){
		printf("Fail to copy file : %s to %s\n",argv[1],argv[2]);
		return -1;
	}
	
	return 0;
}
