#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define size 4096
typedef struct _tag_sharemem
{
    char read_flag;
    char write_flag;
    char data[0];
}sharemem;

void read_sharmem(void* addr)
{
    sharemem *sha_mem=(sharemem*)addr;

    while(1)
    {

        if(0 == (sha_mem)->read_flag)
        {
            printf("%s\n",(sha_mem)->data);

            if(0 == strncmp((sha_mem)->data,"quit",4))
            {
                break;
            }
            (sha_mem)->read_flag=1;

        }


    }
}
int main(int argc,const char* argv[])
{

    int shmid;
    void* addr_shmat;
    sharemem* sha_mem;
    key_t key=ftok(argv[1],'k');
    if(0 > key)
    {
        fprintf(stderr,"Fail to ftok %s",strerror(errno));
        exit(EXIT_FAILURE);
    }

    shmid = shmget(key,size,IPC_CREAT|0666);
    if(0 > shmid)
    {
        fprintf(stderr,"Fail to ftok %s",strerror(errno));
        exit(EXIT_FAILURE);
    }
    addr_shmat = shmat(shmid, NULL, 0);

    sha_mem=(sharemem*)addr_shmat;
    sha_mem->read_flag=1;

    read_sharmem(sha_mem);
    return 0;

}
