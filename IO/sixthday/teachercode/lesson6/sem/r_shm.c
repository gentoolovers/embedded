#include <head.h>

#define SHM_SIZE 4096


int shm_init(const char *filename,char **shm_addr)
{
	key_t key;
	int shm_id;

	key = ftok(filename,'k');
	if(key < 0){
		perror("Fail to key");
		exit(EXIT_FAILURE);
	}

	shm_id = shmget(key,SHM_SIZE,IPC_CREAT | 0666);
	if(shm_id < 0){
		fprintf(stderr,"Fail to shmget : %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	*shm_addr = (char *)shmat(shm_id,NULL,0);
	if(*shm_addr == (char *)-1){
		perror("Fail to shmat");
		exit(EXIT_FAILURE);
	}
	
	return shm_id;
}

//./a.out filename
int main(int argc, const char *argv[])
{
	int shm_id;
	int sem_id;
	char *pmsg;
	int create_flag = 0;

	if(argc < 2){
		fprintf(stderr,"Usage : %s <filename>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	
	shm_id = shm_init(argv[1],(char **)&pmsg);
	sem_id = init_sems(argv[1],2,&create_flag);

	while(1){
		P(sem_id,0);
		printf("%s\n",pmsg);
		V(sem_id,1);

		if(strncmp(pmsg,"quit",4) == 0){
			break;
		}
	}
	
	if(create_flag){
		usleep(500);
		delete_sems(sem_id);
	}

	
	return 0;
}
