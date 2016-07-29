#include <stdio.h>
#include <errno.h>
#include <string.h>
struct student
{
    char name[20];
    int age;
    float score;
};
void do_copy(const char* src_file , const char* dest_file);
int main(int argc,const char* argv[])
{
    struct student stu1={"xiaoming",10,50};
    struct student stu2;
    FILE* fp;
    int i;
    if(argc <2)
    {
        fprintf(stderr,"Usage %s <src_fiel> <dest_file>",argv[0]);
        return -1;
    }
    fp=fopen(argv[1],"w+");
    if(fp == NULL)
    {
        return -1;
    }
    fwrite(&stu1,sizeof(stu1),1,fp);
    rewind(fp);
    fread(&stu2,sizeof(stu2),1,fp);
    fclose(fp);
    return 0;
}
