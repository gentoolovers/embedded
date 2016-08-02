#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
int do_copy(const char* src_file,const char* dest_file)
{
    FILE* src_fp,*src_fp1;
    FILE* dest_fp,*dest_fp1;
    int ret_read,ret_write;
    char buf[4096];
    int src_file_size,src_hal_size;
    pid_t pid;

    if((src_file == NULL) || (dest_file == NULL))
    {
        return -1;
    }

    src_fp=fopen(src_file,"r");
    if(src_fp == NULL)
    {
        fprintf(stderr,"Fail to open %s:%s",src_file,strerror(errno));
        return -1;
    }
    dest_fp=fopen(dest_file,"w+");
    if(dest_fp == NULL)
    {
        fprintf(stderr,"Fail to open %s:%s",dest_file,strerror(errno));
        return -1;
    }

    fseek(src_fp,0,SEEK_END);
    src_file_size=ftell(src_fp);
    src_hal_size=src_file_size/2;

    rewind(src_fp);

    pid=fork();

    if(0 > pid )
    {
        perror("Fail to fork child process");
        return -1;
    }
    if(0 < pid)
    {
        while(1)
        {
            fclose(src_fp);
            fclose(dest_fp);

            src_fp=fopen(src_file,"r");
            if(src_fp == NULL)
            {
                fprintf(stderr,"Fail to open %s:%s",src_file,strerror(errno));
                return -1;
            }
            dest_fp=fopen(dest_file,"r+");
            if(dest_fp == NULL)
            {
                fprintf(stderr,"Fail to open %s:%s",dest_file,strerror(errno));
                return -1;
            }


            if((ftell(src_fp) + sizeof(buf)) <= (src_hal_size))
            {

                ret_read=fread(buf,1,sizeof(buf), src_fp);
                if(ret_read == 0)
                {
                    break;
                }
                ret_write = fwrite(buf,1,ret_read,dest_fp);
                if(ret_write == 0)
                {
                    break;
                }
            }else
            {
                ret_read=fread(buf,1,src_hal_size -ftell(src_fp), src_fp);
                if(ret_read == 0)
                {
                    break;
                }
                ret_write = fwrite(buf,1,ret_read,dest_fp);
                if(ret_write == 0)
                {
                    break;
                }

                break;
            }

        }
        fclose(src_fp);
        fclose(dest_fp);
    }
    if(0 == pid)
    {
        fclose(src_fp);
        fclose(dest_fp);

        src_fp1=fopen(src_file,"r");
        if(src_fp1 == NULL)
        {
            fprintf(stderr,"Fail to open %s:%s",src_file,strerror(errno));
            return -1;
        }
        dest_fp1=fopen(dest_file,"r+");
        if(dest_fp1 == NULL)
        {
            fprintf(stderr,"Fail to open %s:%s",dest_file,strerror(errno));
            return -1;
        }

        fseek(src_fp1,src_hal_size,SEEK_SET);
        fseek(dest_fp1,src_hal_size,SEEK_SET);

         while(1)
        {
            if((ftell(src_fp1) + sizeof(buf) <= src_file_size))
            {
                ret_read=fread(buf,1,sizeof(buf),src_fp1);
                if(ret_read == 0)
                {
                    break;
                }
                ret_write = fwrite(buf,1,ret_read,dest_fp1);
                if(ret_write == 0)
                {
                    break;
                }
            }else
            {
                ret_read=fread(buf,1,src_file_size - ftell(src_fp1),src_fp1);
                    if(ret_read == 0)
                    {
                        break;
                    }
                    ret_write = fwrite(buf,1,ret_read,dest_fp1);
                    if(ret_write == 0)
                    {
                        break;
                    }
                    break;
            }


        }

        fclose(src_fp1);
        fclose(dest_fp1);
        }

    return 0;
}




int main(int argc,const char* argv[])
{
    if(argc <3 )
    {
        fprintf(stderr,"Usage : %s <src file> <dest fiel>f",argv[0]);
        return 0;
    }
    do_copy(argv[1],argv[2]);

    return 0;
}
