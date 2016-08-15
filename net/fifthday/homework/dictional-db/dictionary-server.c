#include <stdio.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <string.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <time.h>
  #include <sqlite3.h>

#define err_log(errlog)                                          \
    do{                                                          \
        printf("%s:%s --> %d  \n", __FILE__, __func__, __LINE__);\
        perror(errlog);                                          \
        exit(1);                                                 \
    }while(0)


  #define N 2048
  #define SQL 3200
  #define  DATABASE      "mydata.db"

  typedef struct _tag_msg
  {
      int acceptfd;
      sqlite3 *db;

  }MSG;
  int callback(void* arg,int ncolumn,char **f_value,char** f_name)
  {
      int ret=-1;
      MSG* msg=(MSG*)arg;
      char sql[SQL];
      struct sockaddr_in clientaddr;
      socklen_t addrlen=sizeof(struct sockaddr);
      int i=0;
      for(i=0;i<ncolumn;i++)
      {
          sprintf(sql," %s ",f_value[i]);
      }
      if((sendto(msg->acceptfd, sql, N, 0, (struct sockaddr *)&clientaddr, addrlen))< 0)
      {
          err_log("fail to sendto\n");
      }

      ret=0;
      return ret;

  }
  int do_query_db(void* arg,char* buf)
  {

      MSG* msg=(MSG*)arg;
      char sql[SQL];
      char* errmsg;
      sprintf(sql,"select * from msg->db wherer name=%s",buf);
      if((sqlite3_exec(msg->db,sql,callback,msg,&errmsg)) != SQLITE_OK)
      {
          return -1;
      }else
      {
          return 1;
      }

  }
 void user_register(void *arg)
  {
      char buf[N];
      char tmp[N];
      char sql[SQL];
      char* errmsg;
      int ret_do_query_db;
      int ret_recv;
      MSG* msg=(MSG*)arg;
      struct sockaddr_in clientaddr;
      socklen_t addrlen=sizeof(struct sockaddr);

      printf("register\n");
      printf("%d\n",msg->acceptfd);

      //接收用户名
      if((recvfrom(msg->acceptfd,buf,N,0,(struct sockaddr*)&clientaddr,&addrlen))<0)
      {
          err_log("fail to recv\n");
      }
      buf[strlen(buf)-1]='\0';
      //注册用户
      sprintf(sql, "insert into usr  values('%s',NULL);",buf);
      if(sqlite3_exec(msg->db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
      {
          if((sendto(msg->acceptfd, "N", N, 0, (struct sockaddr *)&clientaddr, addrlen)) < 0)
          {
              err_log("fail to sendto\n");
          }
          return;
      }
      else
      {

          if((sendto(msg->acceptfd, "Y", N, 0, (struct sockaddr *)&clientaddr, addrlen)) < 0)
          {
              err_log("fail to sendto\n");
          }
          bzero(tmp,sizeof(tmp));
          bzero(sql,sizeof(sql));

          //接收用户密码
          if((recvfrom(msg->acceptfd,tmp,N,0,(struct sockaddr*)&clientaddr,&addrlen)) <0)
          {
              err_log("fail to recv\n");
          }
         // tmp[strlen(tmp) -1]='\0';

          sprintf(sql,"update usr set passwd='%s' where name='%s';",tmp,buf);

          if((sqlite3_exec(msg->db,sql,NULL,NULL,&errmsg)) != SQLITE_OK)
          {
              if((sendto(msg->acceptfd, "N", N, 0, (struct sockaddr *)&clientaddr, addrlen)) < 0)
              {
                  err_log("fail to sendto\n");
              }
              err_log("sqlite3_exec update passwd fail\n");
          }else
          {
              usleep(50);
              if((sendto(msg->acceptfd, "Y", N, 0, (struct sockaddr *)&clientaddr, addrlen)) < 0)
              {
                  err_log("fail to sendto\n");
              }
              printf("register done.\n");
              return;
          }

      }
  }
  void user_quit(char* tmp)
  {
      printf("%s quit\n",tmp);
  }
int insert_words_to_database(const char *pathname,void* arg)
{
    FILE* fp;
    char buf[N];
    char sql[SQL];
    char* tmp;
    int count;
    int line=0;
    char* errmsg;
    MSG* msg=(MSG*)arg;
    if((fp=fopen(pathname,"r")) == NULL)
    {
        err_log("fail to open\n");
    }
    while(1)
    {
        buf[0]='\0';
        if((fgets(buf,sizeof(buf)-1,fp)) == NULL)
        {
            printf("OK!\n");
            break;
        }
        line= line++;
        buf[strlen(buf) -1 ] = '\0';
        int i = 0;
        while(buf[++i] != ' ');
        buf[i] = '\0';

        while(buf[++i] == ' ');
        tmp = &buf[i];

        sprintf(sql,"insert into words (words,comment) values('%s','%s')",buf,tmp);

        if((sqlite3_exec(msg->db,sql,NULL,NULL,&errmsg)) != SQLITE_OK)
        {
            //err_log("fail  sqlite3_exec\n");
        }

        printf("%d\n",line);
    }
    fclose(fp);
    printf("%d\n",line);
}
  query_words(void* arg,char* name)
  {
      FILE* fp;
      int ret_read;
      char buf[N];
      char sql[SQL];
      char tmp[N];
      char** db_ret;
      char* errmsg;

      int row,column;
      int i,j;
      char fgets_buf[N];
      int ret_recv;
      struct sockaddr_in clientaddr;
      MSG* msg=(MSG*)arg;
      time_t t;
      struct tm *ptm;
      t=time(NULL);
      ptm=localtime(&t);

      socklen_t addrlen=sizeof(struct sockaddr);
      if((sendto(msg->acceptfd, "Y", N, 0, (struct sockaddr *)&clientaddr, addrlen)) < 0)
      {
          err_log("fail to sendto");
      }

      while(1)
      {

          if((ret_recv = recvfrom(msg->acceptfd,buf,N,0,(struct sockaddr*)&clientaddr,&addrlen)) <0)
          {
              err_log("fail to recv\n");
          }
          if(buf[0]== '#')
          {
              printf("return menu\n");
              break;
          }
          printf("start query words\n");



              sprintf(sql,"select * from words where words='%s';",buf);
              if((sqlite3_get_table(msg->db,sql,&db_ret,&row,&column,&errmsg)) != SQLITE_OK)
              {
                  err_log("fail to sqlite3_get_table\n");
              }
              else
              {

                      char time_buf[N] = {};
                      sprintf(time_buf,"%d-%d-%d",ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday);
                      sprintf(sql,"insert into record values('%s','%s','%s')",name,time_buf,buf);
                      if(sqlite3_exec(msg->db,sql,NULL, NULL, &errmsg) != SQLITE_OK)
                      {
                          printf("%s\n", errmsg);
                      }
                      else
                      {
                          printf("insert table success.\n");
                          for(i=1;i<row + 1;i++)
                          {
                              buf[0] = '\0';
                              for(j =1;j<column-1;j++)
                              {

                                  strcat(buf,db_ret[j + i*column]);

                              }
                              strcat(buf, db_ret[j+i*column]);
                          }
                          sprintf(tmp,"Y%s",buf);
                          if((sendto(msg->acceptfd, tmp, N, 0, (struct sockaddr *)&clientaddr, addrlen)) < 0)
                          {
                              err_log("fail to sendto");
                          }
                      }

              }

      }
  }
  void history_query(void* arg,char* tmp)
  {
      char buf[N];
      char sql[SQL];
      char** db_ret;
      char* errmsg;
      int row,column;
      int ret_recv;
      int i,j,index;
      MSG* msg=(MSG*)arg;
      struct sockaddr_in clientaddr;
      socklen_t addrlen=sizeof(struct sockaddr);
      sprintf(sql,"select * from record where name='%s';",tmp);
      if((sqlite3_get_table(msg->db,sql,&db_ret,&row,&column,&errmsg)) != SQLITE_OK)
      {
          err_log("fail to sqlite3_get_table\n");
      }

      if(row < 1)
      {
          buf[0] = '\0';
          sprintf(buf,"empty!");
      }
      else
      {
          for(i=1;i<row + 1;i++)
          {
              buf[0] = '\0';
              for(j =0;j<column-1;j++)
              {

                  strcat(buf,db_ret[j + i*column]);
                  strcat(buf,"|");
              }
              strcat(buf, db_ret[j+i*column]);
              printf("%s\n",buf);
              if((sendto(msg->acceptfd, buf, N, 0, (struct sockaddr *)&clientaddr, addrlen)) < 0)
              {
                  err_log("fail to sendto\n");
              }
          }
      }


  }

  int user_login(void* arg)
  {
      MSG *msg=(MSG*)(arg);
      char buf[N];
      char sql[SQL];
      char tmp[N];
      char* errmsg;
      int ret_query;
      struct sockaddr_in clientaddr;
      socklen_t addrlen=sizeof(struct sockaddr);
      //查询数据库,核对密码
          printf("usr_login\n");

          if((recvfrom(msg->acceptfd,buf,N,0,(struct sockaddr*)&clientaddr,&addrlen))<0)
          {
              err_log("fail to recv\n");
          }
          printf("buf is %s\n",buf);

          sprintf(sql,"select * from usr where name='%s'",buf);
          if((sqlite3_exec(msg->db,sql,NULL,NULL,&errmsg)) != SQLITE_OK)
          {
              printf("*********\n");
              if((sendto(msg->acceptfd, "N", N, 0, (struct sockaddr *)&clientaddr, addrlen)) < 0)
              {
                  err_log("fail to sendto\n");
              }

          }else
          {
              if((sendto(msg->acceptfd, "Y", N, 0, (struct sockaddr *)&clientaddr, addrlen)) < 0)
              {
                  err_log("fail to sendto\n");
              }

              if((recvfrom(msg->acceptfd,tmp,N,0,(struct sockaddr*)&clientaddr,&addrlen))<0)
              {
                  err_log("fail to recv\n");
              }

              sprintf(sql,"select * from usr where name='%s' and passwd = '%s';",buf,tmp);
              if((sqlite3_exec(msg->db,sql,NULL,NULL,&errmsg)) != SQLITE_OK)
              {
                  if((sendto(msg->acceptfd, "n", N, 0, (struct sockaddr *)&clientaddr, addrlen)) < 0)
                  {
                      err_log("fail to sendto\n");
                  }
              }else
              {
                  usleep(50);
                  if((sendto(msg->acceptfd, "Y", N, 0, (struct sockaddr *)&clientaddr, addrlen)) < 0)
                  {
                      err_log("fail to sendto\n");
                  }

                  printf("%s login success\n",tmp);

              }
          }



      // 处理二级菜单
      while(1)
      {
          if((recvfrom(msg->acceptfd, buf, N, 0, (struct sockaddr*)&clientaddr, &addrlen) )< 0)
          {
              err_log("fail to recvfrom");
          }
          switch(buf[0])
          {
          case '1':
              query_words(arg,tmp);
              continue;
          case '2':
              history_query(arg,tmp);
              continue;
          case '3':
              user_quit(tmp);
              continue;
          default:
              continue;
          }
      }
  }
  void* pthread_do(void* arg)
  {
      char buf[N];
      MSG* msg=(MSG*)arg;
      struct sockaddr_in clientaddr;
      socklen_t addrlen=sizeof(struct sockaddr);
      //处理一级菜单
      while(1)
      {
          bzero(buf,sizeof(buf));

          if((recvfrom(msg->acceptfd, buf, N, 0, (struct sockaddr*)&clientaddr, &addrlen)) < 0)
          {
              err_log("fail to recvfrom");
          }
          switch(buf[0])
          {
          case '1':
              user_register(arg);
              break;
          case '2':
              user_login(arg);
              break;
          case '3':
              printf("usr quit\n");
              break;
          default:
              break;
          }
      }
      free(msg);
      msg==NULL;
  }
  int main(int argc,const char* argv[])
  {
      int sockfd;
      int acceptfd;
      char* errmsg;
      struct sockaddr_in serveraddr,clientaddr;
      pthread_t thread_id;
      MSG usr_msg;

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

      //把 sockfd 与 serveraddr 绑定在一起
      if((bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))) < 0 )
      {
          err_log("fail to bind\n");
      }

      //把 sockfd 设置为被动模式,能监听到客户端链接
      if((listen(sockfd,15)) <0 )
      {
          err_log("fail to listen\n");
      }

      //打开数据库
      if((sqlite3_open(DATABASE,&(usr_msg.db)) != SQLITE_OK))
      {
          printf("%s\n",sqlite3_errmsg(usr_msg.db));
          exit(EXIT_FAILURE);
      }
      //创建表
      if((sqlite3_exec(usr_msg.db,"create table usr (name char primary key,passwd char);",NULL, NULL, &errmsg)) != SQLITE_OK)
      {
          printf("%s\n", errmsg);
      }
      else
      {
          printf("Create table success.\n");
      }
      if((sqlite3_exec(usr_msg.db,"create table record (name char,date char,word char);",NULL, NULL, &errmsg)) != SQLITE_OK)
      {
          printf("%s\n", errmsg);
      }
      else
      {
          printf("Create table success.\n");
      }
      if((sqlite3_exec(usr_msg.db,"create table words (id integer primary key autoincrement,words char,comment char);",NULL, NULL, &errmsg)) != SQLITE_OK)
      {
          printf("%s\n", errmsg);
      }
      else
      {
          printf("Create table success.\n");
      }
     // insert_words_to_database("dict.txt",&usr_msg);

      //死循环,监视是否有客户端链接,有则创建一个线程去处理
      socklen_t addrlen=sizeof(struct sockaddr);
      while(1)
      {

          MSG* msg=(MSG*)malloc(sizeof(MSG));
          msg->db=usr_msg.db;

          // 接受客户端链接,并创建专门为该客户端收发消息的 acceptfd
          if((msg->acceptfd=accept(sockfd,(struct sockaddr*)&clientaddr,&addrlen)) < 0)
          {
              err_log("fail to accept\n");
          }

          //创建线程,并执行线程处理函数
          if((pthread_create(&thread_id, NULL,pthread_do, msg)) != 0)
          {
              err_log("fail to pthread_create\n");
          }

          //把线程标记为分离式,线程结束后自动回收资源
          if((pthread_detach(thread_id)) != 0 )
          {
              err_log("fail to pthread_detach\n");
          }

      }

      return 0;
  }
