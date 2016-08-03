#include <head.h>
#include <pthread.h>

typedef struct{
	void *pdata;
	int  n;
}parm_t;

sem_t reverse_sem;
sem_t output_sem;

void *reverse_array(void *arg)
{
	int i,j;
	int tmp;
	int n  = ((parm_t *)arg)->n;
	int *p = (int *)(((parm_t *)arg)->pdata); 
	
	while(1)
	{
		if(sem_wait(&reverse_sem) < 0){
			perror("Fail to sem_wait");
			exit(EXIT_FAILURE);
		}
		
		printf("reverse_array thread runing\n");
		i = 0;
		j = n - 1;

		while(i < j){
			tmp  = p[i];
			p[i] = p[j];
			p[j] = tmp;

			i ++;
			j --;
		}
		
		if(sem_post(&output_sem) < 0){
			perror("Fail to output_sem");
			exit(EXIT_FAILURE);
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
		if(sem_wait(&output_sem) < 0){
			perror("Fail to sem_wait");
			exit(EXIT_FAILURE);
		}
		
		printf("output_array thread runing\n");

		for(i = 0;i < n;i ++){
			printf("%d ",p[i]);
		}
		printf("\n");


		if(sem_post(&reverse_sem) < 0){
			perror("Fail to sem_wait");
			exit(EXIT_FAILURE);
		}
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

	if(sem_init(&reverse_sem,0,1) < 0){
		fprintf(stderr,"Fail to sem_init reverse_sem:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	if(sem_init(&output_sem,0,0) < 0){
		fprintf(stderr,"Fail to output_sem:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

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
