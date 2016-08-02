#include <head.h>

int write_file(int fd)
{
	char buf[1024];

	while(1){
		fgets(buf,sizeof(buf),stdin);
		buf[strlen(buf) - 1] = '\0';
		
		write(fd,buf,strlen(buf));

		//还原offset 
		lseek(fd,-strlen(buf),SEEK_CUR);

		if(strncmp(buf,"quit",4) == 0){
			break;
		}
	}

	return 0;
}

int read_file(int fd)
{
	int n;
	char buf[1024];

	while(1){
		n = read(fd,buf,sizeof(buf) - 1);
		if(n == 0){
			continue;
		}
	
		buf[n] = '\0';//因为我想以字符串形式输出，所以我添加'\0'
		printf("Read %d bytes : %s\n",n,buf);
		
		if(strncmp(buf,"quit",4) == 0){
			break;
		}
	}

	return 0;
}

//./a.out filename
int main(int argc, const char *argv[])
{
	int fd;
	pid_t pid;

	if(argc < 2){
		fprintf(stderr,"Usage : %s <filename>\n",argv[0]);
		return -1;
	}

	fd = open(argv[1],O_RDWR | O_CREAT | O_TRUNC ,0666);
	if(fd < 0) {
		fprintf(stderr,"Fail to open %s : %s\n",argv[1],strerror(errno));
		return -1;
	}

	pid = fork();
	if(pid < 0){
		perror("Fail to fork");
		return -1;
	}

	if(pid > 0){
		write_file(fd);
	}

	if(pid == 0){
		read_file(fd);
	}
	
	return 0;
}
