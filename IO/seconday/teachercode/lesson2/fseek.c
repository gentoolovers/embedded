#include <head.h>

struct student{
	char name[15];
	int age;
	float score;
};

//./a.out  student.txt
int main(int argc, const char *argv[])
{
	FILE *fp;
	int ret;
	struct student stu1 = {"xiaoming",10,90};
	struct student stu2;

	if(argc < 2){
		fprintf(stderr,"Usage : %s <student.txt>\n",argv[0]);
		return -1;
	}

	fp = fopen(argv[1],"w+");
	if(fp == NULL){
		fprintf(stderr,"Fail to fopen %s : %s\n",argv[1],strerror(errno));
		return -1;
	}

	ret = fwrite(&stu1,sizeof(stu1),1,fp);
	if(ret < 1){
		fprintf(stderr,"Fail to fwrite : %s\n",strerror(errno));
		return -1;
	}
	
	rewind(fp);

	ret = fread(&stu2,sizeof(stu2),1,fp);
	if(ret < 0){
		fprintf(stderr,"Fail to fread : %s\n",strerror(errno));
	}
	
	fclose(fp);

	printf("%s  %d  %f\n",stu2.name,stu2.age,stu2.score);

	return 0;
}
