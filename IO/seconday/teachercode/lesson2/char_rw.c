#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, const char *argv[])
{
	FILE *fp;
	int ch;

	fp = fopen(argv[1],"r+");
	if(fp == NULL){
		fprintf(stderr,"Fail to fopen %s : %s\n",argv[1],strerror(errno));
		return -1;
	}
	

	while(1){
		ch = fgetc(fp);
		if(ch == -1){
			break;
		}
		printf("%c\n",ch);
	}

	//写入一个'\0'到文件中
	fputc('\0',fp);

	return 0;
}
