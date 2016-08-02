#include <stdio.h>
int main(int argc,const char* argv[])
{

    int  pid1,pid2;
    int status,ret;
    pid1=fork();
    if(pid1 <0)
    {
        perror("fail to fork pid1");
    }
    if(pid1 ==0)
    {
        sleep(5);
        return 0;
    }
    if(pid1 >0)
    {
        pid2=fork();
        if(pid2 <0)
        {
            perror("fail to fork pid2e");
            return -1;
        }
        if(pid2 == 0)
        {
            while(1);
        }
        while(1)
        {
            printf("wait ...\n");
           ret= wait(&status);
           if(WIFEXITED(status))
           {
               printf("child terminated normally,return value",WEXITSTATUS(&status));
           }else if(WIFIGNALED(&status))
           {
               printf("child terminated w")
           }

        }
    }

    return 0;
}
