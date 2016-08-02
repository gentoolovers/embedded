#include <head.h>

int main(int argc, const char *argv[])
{
	int i = 0;
	char buf[1024];
	char *p_arry[10];

	//abc会车
	//buf:abc\n\0
	//ls  -l
	fgets(buf,sizeof(buf),stdin);
	buf[strlen(buf) - 1] = '\0';
	

	p_arry[i] = strtok(buf," ");


	while( p_arry[++i] = strtok(NULL," ") )
		;

	for(i = 0;p_arry[i] != NULL;i ++){
		printf("%s\n",p_arry[i]);
	}
	
	printf("buf : %s\n",buf);

	return 0;
}
