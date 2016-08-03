#include <head.h>

typedef struct{
	void *pdata;
	int  n;
}parm_t;

void *reverse_array(void *arg)
{
	int i,j;
	int tmp;
	int n  = ((parm_t *)arg)->n;
	int *p = (int *)(((parm_t *)arg)->pdata); 
	
	while(1)
	{
		i = 0;
		j = n - 1;

		while(i < j){
			tmp  = p[i];
			p[i] = p[j];
			p[j] = tmp;

			i ++;
			j --;
		}
	}
}

void *output_array(void *arg)
{
	int i;
	int n  = ((parm_t *)arg)->n;
	int *p = (int *)(((parm_t *)arg)->pdata); 
	
	while(1)
	{
		for(i = 0;i < n;i ++){
			printf("%d ",p[i]);
		}
		printf("\n");
	}
}

int main()
{
	int ret;
	parm_t parm;
	pthread_t tid[2];
	int a[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	
	parm.pdata = a;
	parm.n     = sizeof(a)/sizeof(a[0]);

	ret = pthread_create(&tid[0],NULL,reverse_array,&parm);
	if(ret != 0){
		fprintf(stderr,"Fail to pthread_create : %s\n",strerror(ret));
		exit(EXIT_FAILURE);
	}

	ret = pthread_create(&tid[1],NULL,output_array,&parm);
	if(ret != 0){
		fprintf(stderr,"Fail to pthread_create : %s\n",strerror(ret));
		exit(EXIT_FAILURE);
	}
	
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

	exit(EXIT_SUCCESS);
}
