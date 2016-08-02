#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int exec_input_program(char* program)
{
    int ret;
    int i=0;
    pid_t pid;
    char* p_array[20];

    //p_array[i] = strtok(program," ");
    p_array[i] = strtok(program," ");

    if(p_array[i] == NULL)
    {
        printf("command ,not found \n");
        return -1;
    }
    while(1)
    {
        if(i ==19 || p_array[i] == NULL)
        {
            break;
        }
        p_array[++i]=strtok(NULL," ");
    }
    pid=fork();
    if(pid <0)
    {
        perror("Fail to fork()");
        exit(EXIT_FAILURE);
    }
    if(pid ==0)
    {
        ret=execvp(p_array[0],p_array);
        if(ret <0)
        {
            perror("fail to execvp ");
            exit(EXIT_FAILURE);

        }
    }
    if(pid >0)
    {
        wait(NULL);
    }
    return 0;
}
int main(int argc,const char* argv[])
{


    char buf[1024];
    int i=0;
    while(i)
    {
        printf("input >");
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf) -1] = '\0';
        if(strncmp(buf,"quit",4) == 0)
        {
            break;
        }
        exec_input_program(buf);
    }
    exit(EXIT_SUCCESS);
}
