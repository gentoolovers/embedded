#include <head.h>

int main(int argc, const char *argv[])
{
	FILE *fp;
	int n;
	int data[] = {10,20,30,40,50,60,70};

	fp = fopen(argv[1],"w+");
	
	//n = fwrite(data,sizeof(int),sizeof(data)/sizeof(int),fp);
	//printf("n = %d\n",n);
	
	n = fwrite(data,sizeof(data),1,fp);
	
	printf("n = %d\n",n);
	


	return 0;
}
