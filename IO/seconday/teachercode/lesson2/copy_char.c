#include <head.h>

void do_copy(const char *src_file,const char *dest_file)
{
    int ch;
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
        ch = fgetc(fp_src);
        if(ch == -1){
            break;
        }
        
        fputc(ch,fp_dest);
    }

    fclose(fp_src);
    fclose(fp_dest);

    return;
}

//./a.out src_file dest_file
int main(int argc, const char *argv[])
{
    if(argc < 3){
        fprintf(stderr,"Usage : %s <src file> <dest file>\n",argv[0]);
        return -1;
    }

    do_copy(argv[1],argv[2]);

    return 0;
}
