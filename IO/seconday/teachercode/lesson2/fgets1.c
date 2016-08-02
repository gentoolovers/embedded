#include <head.h>

int main(int argc, const char *argv[])
{
	int ret;
	char buf[5];

	while(1){
		printf("Input a string : ");
		//abc回车
		//buf:a b c \n \0
		//    0 1 2  3  4
		//
		//回车 
		//buf:\n
		fgets(buf,sizeof(buf),stdin);
		if(buf[strlen(buf) - 1] == '\n'){
			buf[strlen(buf) - 1] = '\0';
		}
		
		//printf("buf: %s\n",buf);
		ret = fputs(buf,stdout);
		printf("ret = %d\n",ret);

		if(strlen(buf) == 0){
			break;
		}
	}
	
	return 0;
}
