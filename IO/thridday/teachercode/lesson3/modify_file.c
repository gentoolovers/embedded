#include <head.h>

void do_copy(const char *src_file,const char *dest_file)
{
	char *ret;
	char buf[1024];
	FILE *fp_src;
	FILE *fp_dest;

	fp_src = fopen(src_file,"r");
	if(fp_src == NULL){
		fprintf(stderr,"Fail to fopen %s : %s\n",src_file,strerror(errno));
		return;
	}

	fp_dest = fopen(dest_file,"w");
	if(fp_dest == NULL){
		fprintf(stderr,"Fail to fopen %s : %s\n",dest_file,strerror(errno));
		return;
	}

	while(1){
		ret = fgets(buf,sizeof(buf),fp_src);
		if(ret == NULL){
			break;
		}
		fputs(buf,fp_dest);
	}

	fclose(fp_src);
	fclose(fp_dest);

	return;
}

int insert_line_file(const char *pathname,int line,const  char *content_string)
{
	char *ret;
	FILE *src_fp;
	FILE *tmp_fp;
	char buf[4096];
	int count_line = 0;
	const char *tmpname = "tmp.file";

	if(pathname == NULL || line <= 0 || content_string == NULL){
		fprintf(stderr,"Invalid params!\n");
		return -1;
	}

	src_fp = fopen(pathname,"r");
	if(src_fp == NULL){
		fprintf(stderr,"Fail to fopen %s : %s\n",pathname,strerror(errno));
		return -1;
	}

	tmp_fp = fopen(tmpname,"w");
	if(tmp_fp == NULL){
		fprintf(stderr,"Fail to fopen %s : %s\n",tmpname,strerror(errno));
		return -1;
	}

	while(1){
		ret = fgets(buf,sizeof(buf),src_fp);
		if(ret == NULL){
			break;
		}
		
		if(buf[strlen(buf) - 1] == '\n'){
			count_line ++;
		}
		
		fputs(buf,tmp_fp);
		
		if(count_line == line - 1){
			fputs(content_string,tmp_fp);
		}

	}
	
	fclose(src_fp);
	fclose(tmp_fp);
	
	do_copy(tmpname,pathname);
	
	return 0;
}

/*
 *
 * 删除第10行:
 * 先将前9行读到临时文件，第10行数据不存入临时文件，接着将后面的数据读取临时文件
 * ，最后将临时文件拷贝到源文件
 */
