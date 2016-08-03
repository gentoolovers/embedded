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

void send_msg(int msg_id,void *pmsg,int msg_len)
{
	int ret;
      
	ret = msgsnd(msg_id,pmsg,msg_len,0);
	if(ret < 0){
		perror("Fail to msgsnd");
		exit(EXIT_FAILURE);
	}

	return;
}

void msg_delete(int msg_id)
{
	int ret;
     
	ret = msgctl(msg_id,IPC_RMID,NULL);
	if(ret < 0){
		perror("Fail to msgctl");
		exit(EXIT_FAILURE);
	}

	return;
}

//./a.out filename
int main(int argc, const char *argv[])
{
	int msg_id;
	msg_t msg;	

	if(argc < 2){
		fprintf(stderr,"Usage : %s <filename>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	
	msg_id = msg_init(argv[1]);
	
	while(1){
		printf("Input > ");
		fgets(msg.mtxt,sizeof(msg.mtxt),stdin);
		msg.mtxt[strlen(msg.mtxt) - 1] = '\0';
		msg.msg_type = MSG_TYPE;

		send_msg(msg_id,&msg,sizeof(msg_t) - sizeof(long));
		
		if(strncmp(msg.mtxt,"quit",4) == 0){
			break;
		}
	}
	
	msg_delete(msg_id);

	return 0;
}
