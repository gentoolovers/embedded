#include <head.h>

int main(int argc, const char *argv[])
{
	char *ret;
	FILE *fp;
	char buf[3];

	fp = fopen(argv[1],"r");
	

	while(1){
		ret = fgets(buf,sizeof(buf),fp);
		if(ret == NULL){
			break;
		}

		printf("buf : %s\n",buf);
	}

	
	return 0;
}
