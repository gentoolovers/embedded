#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define MSG_TYPE 100

typedef struct _tag_IPC
{
    long msg_type;
    char mtxt[4096];
}msg_t;


int main(int argc,const char* argv)
{

    msg_t msg_A;

    key_t ret_ftok;
    int proj_id,msg_id;
    pid_t process_pid;

    msg_A.msg_type = MSG_TYPE;
    ret_ftok =ftok("ipc_A.c",proj_id);
    if(0 > ret_ftok)
    {
        fprintf(stderr,"Fail to ftok %s",strerror(errno));
        exit(EXIT_FAILURE);
    }
    msg_id=msgget(ret_ftok,IPC_CREAT|0666);
    if(-1 == msg_id)
    {
        fprintf(stderr,"Fail to msgget",strerror(errno));
        exit(EXIT_FAILURE);
    }

    process_pid =fork();
    if(0 > process_pid)
    {
        fprintf(stderr,"Fail to fork %s",strerror(errno));
        exit(EXIT_FAILURE);
    }
    if(0 == process_pid)
    {
        while(1)
        {
            printf("input>");
            fgets(msg_A.mtxt,sizeof(msg_A.mtxt),stdin);

            msg_A.mtxt[strlen(msg_A.mtxt) -1]='\0';

            msgsnd(msg_id,&msg_A,sizeof(msg_A)-sizeof(long),0);

            if(0 == strncmp(msg_A.mtxt,"quit",4))
            {
                break;
            }
        }
         msgctl(msg_id, IPC_RMID, NULL);
    }

    while(1);
    return 0;
}
