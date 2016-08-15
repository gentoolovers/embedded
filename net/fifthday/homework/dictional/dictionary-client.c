#include <stdio.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <string.h>

#define err_log(errlog)                                          \
    do{                                                          \
        printf("%s:%s --> %d  \n", __FILE__, __func__, __LINE__);\
        perror(errlog);                                          \
        exit(1);                                                 \
    }while(0)


    #define N 2048

    int  user_register_or_login(char* buf,int sockfd,struct sockaddr_in serveraddr)
    {

        char msg[N];
        socklen_t addrlen=sizeof(struct sockaddr);

            // 1表示注册,2表示登录
            if(buf[0]=='1')
            {
                if((sendto(sockfd, "1", N, 0, (struct sockaddr *)&serveraddr, addrlen)) < 0)
                {
                    err_log("fail to sendto");
                }
            }else if(buf[0] =='2')
            {
                if((sendto(sockfd, "2", N, 0, (struct sockaddr *)&serveraddr, addrlen)) < 0)
                {
                    err_log("fail to sendto");
                }
            }else
            {
                printf("buf[0] error\n");
            }

            printf("username\n");
            fgets(msg, N, stdin);
            buf[strlen(msg)-1] = '\0';

            if((sendto(sockfd, msg, N, 0, (struct sockaddr *)&serveraddr, addrlen)) < 0)
            {
                err_log("fail to sendto");
            }
            if((recvfrom(sockfd, msg, N, 0, (struct sockaddr*)&serveraddr, &addrlen)) < 0)
            {
                err_log("fail to recvfrom");
            }


            //假如msg[0] 为Y 或者y,则可以注册或者登录
            if(msg[0] =='Y'|| msg[0]=='y')
            {
                printf("you passwd \n");
                bzero(msg,sizeof(msg));
                fgets(msg, N, stdin);
                msg[strlen(msg)-1] = '\0';
                if((sendto(sockfd, msg, N, 0, (struct sockaddr *)&serveraddr, addrlen)) < 0)
                {
                    err_log("fail to sendto");
                }

                msg[0]='\0';
                if((recvfrom(sockfd, msg, N, 0, (struct sockaddr*)&serveraddr, &addrlen)) < 0)
                {
                    err_log("fail to recvfrom");
                }
                printf("%c\n",msg[0]);
                if((msg[0] == 'Y')&& (buf[0] =='1'))
                {
                    printf("register success\n");
                    return 1;
                }else if((msg[0]=='Y')&& (buf[0] =='2'))
                {
                    printf("login success\n");
                    return 2;
                }else
                {
                      printf("error\n");
                }

            }else if((msg[0]== 'N') && (buf[0] =='1'))
            {
                printf("fail to register,username already exist\n");

            }else if((msg[0] == 'N') && (buf[0] =='2'))
            {
                printf("invalid username\n");

            }else if((msg[0]== 'n') && (buf[0] =='2'))
            {
                printf("invalid passwd\n");

            }else
            {
                printf("user_register_or_login error\n");
                exit(EXIT_FAILURE);
            }

    }

    user_quit(int sockfd,struct sockaddr_in  serveraddr)
    {

        socklen_t addrlen=sizeof(struct sockaddr);

        if((sendto(sockfd, "3", N, 0, (struct sockaddr *)&serveraddr, addrlen)) < 0)
        {
            err_log("fail to sendto");
        }
        printf("user quit\n");
        exit(EXIT_SUCCESS);
    }
  void query_words(int sockfd,struct sockaddr_in serveraddr)
  {
      char msg[N];
      socklen_t addrlen=sizeof(struct sockaddr);
      if((sendto(sockfd,"1", N, 0, (struct sockaddr *)&serveraddr, addrlen)) < 0)
      {
          err_log("fail to sendto");
      }

      if((recvfrom(sockfd, msg, N, 0, (struct sockaddr*)&serveraddr, &addrlen)) < 0)
      {
          err_log("fail to recvfrom");
      }
      if(msg[0] == 'Y')
      {
          while(1)
          {
              printf("# is return to menu\n");
              printf("please input the words that you want to query\n");
              fgets(msg, N, stdin);
              msg[strlen(msg)-1] = '\0';
              if((sendto(sockfd, msg, N, 0, (struct sockaddr *)&serveraddr, addrlen)) < 0)
              {
                  err_log("fail to sendto");
              }
              if(msg[0] == '#')
              {
                  break;
              }

              if((recvfrom(sockfd, msg, N, 0, (struct sockaddr*)&serveraddr, &addrlen)) < 0)
              {
                  err_log("fail to recvfrom");
              }

              if(msg[0] == 'Y')
              {
                  printf("%s\n",msg+1);
              }else if(msg[0] == 'N')
              {
                  printf("sorry ,it's not a word\n");
              }else
              {
                  printf("query error\n");
                  exit(EXIT_FAILURE);
              }
          }
      }

  }
  void history_of_query(int sockfd,struct sockaddr_in serveraddr)
  {
      char msg[N];
      int ret_recv;
      socklen_t addrlen=sizeof(struct sockaddr);
      if((sendto(sockfd, "2", N, 0, (struct sockaddr *)&serveraddr, addrlen)) < 0)
      {
          err_log("fail to sendto");
      }

          if((ret_recv=recvfrom(sockfd, msg, N, 0, (struct sockaddr*)&serveraddr, &addrlen)) < 0)
          {
              err_log("fail to recvfrom");
          }
          printf("%s\n",msg);


  }
    second_menu(int sockfd,struct sockaddr_in serveraddr)
    {
        char buf[N];
        socklen_t addrlen=sizeof(struct sockaddr);

        //二级菜单
        while(1)
        {
            printf("************************************************\n");
            printf("*      1 :query words 2 :history 3 :quit       *\n");
            printf("************************************************\n");
            fgets(buf, N, stdin);
            buf[strlen(buf)-1] = '\0';

            switch(buf[0])
            {
            case '1':
                query_words(sockfd,serveraddr);
                continue;
            case '2':
                history_of_query(sockfd,serveraddr);
                break;
            case '3':
                user_quit(sockfd,serveraddr);
                break;
            default:
                printf("input error\n");
            }

        }

    }

    int main(int argc,const char* argv[])
    {
        int sockfd;
        int ret_login;
        char buf[N];
        struct sockaddr_in serveraddr;
        // 判断参数是否正确
        if(argc <3)
        {
            fprintf(stderr,"Usage:%s <serverip> <port>\n",argv[0]);
            exit(EXIT_FAILURE);
        }

        // 创建套接字,得到套接字描述符
        if((sockfd=socket(AF_INET,SOCK_STREAM,0)) <0)
        {
            err_log("fail to socket\n");
        }

        // 填充网络信息结构体
        serveraddr.sin_family=AF_INET;
        serveraddr.sin_addr.s_addr=inet_addr(argv[1]);
        serveraddr.sin_port=htons(atoi(argv[2]));

        //连接服务器
        if((connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0)
        {
            err_log("fail to connect");
        }

        //一级菜单
        while(1)
        {
            printf("************************************************\n");
            printf("*        1 :register 2 :login 3 :quit          *\n");
            printf("************************************************\n");

            fgets(buf, N, stdin);
            buf[strlen(buf)-1] = '\0';
            switch(buf[0])
            {
            case '1':
                user_register_or_login(buf,sockfd,serveraddr);
                continue;

            case '2':
                ret_login= user_register_or_login(buf,sockfd,serveraddr);
                printf("%d\n",ret_login);
                if((ret_login == 2)&&(buf[0] == '2'))
               {
                   second_menu(sockfd,serveraddr);
               }
               continue;

            case '3':
                user_quit(sockfd,serveraddr);
                break;

            default:
                printf("input error\n");
            }

        }
    }
