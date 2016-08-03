#include <head.h>

#define MSG_TYPE 100

typedef struct{
	long msg_type;
	char mtxt[1024];
}msg_t;

int msg_init(const char *filename)
{
	key_t key;
	int msg_id;

	key = ftok(filename,'k');
	if(key < 0){
		perror("Fail to key");
		exit(EXIT_FAILURE);
	}

	msg_id = msgget(key,IPC_CREAT | 0666);
	if(msg_id < 0){
		fprintf(stderr,"Fail to msgget : %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	return msg_id;
}

//./a.out filename
int main(int argc, const char *argv[])
{
	int ret;
	int msg_id;
	msg_t msg;	

	if(argc < 2){
		fprintf(stderr,"Usage : %s <filename>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	
	msg_id = msg_init(argv[1]);


	while(1){
    	ret =  msgrcv(msg_id,&msg,sizeof(msg_t) - sizeof(long),MSG_TYPE,0);
		if(ret < 0){
			perror("Fail to msgrcv");
			exit(EXIT_FAILURE);
		}

		printf("----------------------------------------------\n");
		printf("msg type : %ld\n",msg.msg_type);
		printf("msg mtxt : %s\n",msg.mtxt);
		printf("----------------------------------------------\n");

		if(strncmp(msg.mtxt,"quit",4) == 0){
			break;
		}
	}

	
	return 0;
}
