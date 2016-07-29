#include <stdio.h>

int main(int argc,const char* argv[])
{
    char buf[10];
    int i=0;
    while(1)
    {
        char* string =fgets(buf,sizeof(buf),stdin);
        for(i=0;i<sizeof(buf);i++)
        {
            if(buf[i] == '\n')
            {
                buf[i]='\0';
                break;
            }
        }
        printf("%s\n",string);
    }

}
