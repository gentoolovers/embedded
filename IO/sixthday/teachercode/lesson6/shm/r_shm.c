#include <head.h>

#define SHM_SIZE 4096

typedef struct{
	char read_flag;
	char write_flag;
	char data[1];
}msg_t;

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
	msg_t *pmsg;

	if(argc < 2){
		fprintf(stderr,"Usage : %s <filename>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	
	shm_id = shm_init(argv[1],(char **)&pmsg);
	pmsg->read_flag  = 0;
	pmsg->write_flag = 1;

	while(1){
		if(!pmsg->read_flag){
			continue;
		}else{
			printf("%s\n",pmsg->data);
			pmsg->read_flag = 0;
			pmsg->write_flag = 1;
		}
	}
	
	return 0;
}
