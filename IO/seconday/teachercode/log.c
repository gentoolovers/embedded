#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int count_line(int fd)
{
	int n;
	int i;
	int line = 0;
	char buf[100];

	while(1){
		n = read(fd,buf,sizeof(buf));
		if(n == 0){
			break;
		}

		if(n == -1){
			fprintf(stderr,"Fail to read\n");
			return -1;
		}
		
		for(i = 0;i < n;i ++){
			if(buf[i] == '\n'){
				line ++;
			}
		}
	}
	
	return line;
}

int write_log(int fd,int line)
{
	int n;
	time_t tim;
	struct tm *ptm;
	char buf[1024];

    
	tim = time(NULL);
	ptm = localtime(&tim);
	
	sprintf(buf,"%d,%d-%d-%d  %d:%d:%d\n",line,ptm->tm_year + 1900,ptm->tm_mon + 1,ptm->tm_mday,\
			ptm->tm_hour,ptm->tm_min,ptm->tm_sec);

	
	n = write(fd,buf,strlen(buf));
	if(n < strlen(buf)){
		fprintf(stderr,"Fail to write : %s\n",strerror(errno));
		return -1;
	}

	return n;
}

void do_log(const char *pathname)
{
	int fd;
	int ret;
	int line = 0;

	fd = open(pathname,O_RDWR | O_CREAT | O_APPEND,0666);
	if(fd < 0){
		fprintf(stderr,"Fail to open %s : %s\n",pathname,strerror(errno));
		return;
	}
	
	line = count_line(fd);
	if(line < 0){
		close(fd);
		return;
	}
	
	while(1){
		ret = write_log(fd,++line);
		if(ret < 0){
			close(fd);
			break;
		}
		sleep(1);
	}
}


//./a.out  log.txt
int main(int argc, const char *argv[])
{
	if(argc < 2){
		fprintf(stderr,"Usage : %s <log.txt>\n",argv[0]);
		return -1;
	}
	
	do_log(argv[1]);
	
	return 0;
}
