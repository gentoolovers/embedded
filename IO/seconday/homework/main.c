#include <stdio.h>
int main(int argc,const char* argv[])
{
    if(argc < 2)
    {
        printf("Usage %s <filename>",argv[0]);
        return -1;
    }
    //insert_line_file(argv[1],2,"xyz");
    delete_line_file(argv[1],2);
}
