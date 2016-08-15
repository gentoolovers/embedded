#include <stdio.h>
       #include <stdlib.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
       #include <sys/socket.h>
       #include <arpa/inet.h>
       #include <netinet/in.h>
       #include <string.h>
       #include "DLinkList.h"

       typedef struct _tag_user_info
       {
           DLinkListNode header;
           char* name;
           struct sockaddr_in clientaddr;
       }user_info;

       #define  N  4096

       #define err_log(errlog)                                             \
           do{                                                             \
               printf("%s:%s --> %d  \n", __FILE__, __func__, __LINE__);   \
               perror(errlog);                                             \
               exit(1);                                                    \
           }while(0)

       void user_tosend_msg(DLinkList* list,char* buf,struct sockaddr_in addr,int sockfd)
       {
           char msg[N];
           DLinkListNode* node=(DLinkListNode*)list;
           socklen_t  addrlen = sizeof(struct sockaddr);
           node=node->next;
           while(node != NULL)
           {

               if((inet_ntoa(((user_info*)node)->clientaddr.sin_addr) == inet_ntoa(addr.sin_addr))&&
                  (ntohs(((user_info*)node)->clientaddr.sin_port) == ntohs(addr.sin_port)))
               {

                   node=node->next;
                   continue;

               }
               sendto(sockfd,&msg,strlen(msg),0,(struct sockaddr*)&(((user_info*)node)->clientaddr),addrlen);

            node=node->next;
           }

       }

   DLinkListNode* find_usr(DLinkList* list,struct sockaddr_in addr)
   {
       DLinkListNode* node=(DLinkListNode*)list;
        printf("*************\n");
       while(node != NULL)
       {
           if((strcmp(inet_ntoa(((user_info*)node)->clientaddr.sin_addr),
inet_ntoa(addr.sin_addr)) == 0)&&
             (ntohs(((user_info*)node)->clientaddr.sin_port) == ntohs(addr.sin_port)))
           {


               return node;
           }
          node=node->next;
       }
       printf("find user fail\n");
       return NULL;
   }



       we_chat(int sockfd, DLinkList* list,char* buf,struct sockaddr_in addr)
       {
           printf("chat\n");
           char msg[N];
           DLinkListNode* node=find_usr(list,addr);
           printf("%s\n",((user_info*)node)->name);
           printf("%s\n",buf+1);
           sprintf(msg,"%s said:%s",((user_info*)node)->name,buf+1);
           printf("%s\n",msg);
           user_tosend_msg(list,msg,addr,sockfd);
           printf("chat sucess\n");

       }

       user_login(int sockfd, DLinkList* list,char* buf,struct sockaddr_in addr)
       {
           printf("login\n");
           char msg[N];
           user_info user;
           DLinkListNode* node=(DLinkListNode*)list;
           user.name=(buf+1);
           printf("%s\n",user.name);
           user.clientaddr=addr;
           DLinkList_Insert(list,(DLinkListNode*)&user,0);
           node=node->next;
           printf("%s\n",((user_info*)node)->name);
           sprintf(msg,"%s login",buf+1);
           user_tosend_msg(list,msg,addr,sockfd);
           printf("login sucess\n");
       }


       user_quit(int sockfd, DLinkList* list,struct sockaddr_in addr)
       {
           printf("quit\n");
           char msg[N];

           DLinkListNode* node = find_usr(list,addr);

           sprintf(msg,"%s quit",((user_info*)node)->name);
           user_tosend_msg(list,msg,addr,sockfd);

           DLinkList_DeleteNode(list,(DLinkListNode*)&addr);

       }


       void child_process_recv(int sockfd)
       {
           char buf[N];
           struct sockaddr_in serveraddr, clientaddr;
           socklen_t  addrlen = sizeof(struct sockaddr);
           DLinkList* list=DLinkList_Create();

           while(1)
           {
               if(recvfrom(sockfd, &buf, N, 0, (struct sockaddr*)&clientaddr, &addrlen) < 0)
               {
                   err_log("fail to recvfrom\n");
               }
               printf("%s\n",buf);

               switch(buf[0])
               {
               case 'C':
                   we_chat(sockfd, list,buf,clientaddr);
                   break;
               case 'Q':
                   user_quit(sockfd, list,clientaddr);
                   break;
               case 'L':
                   user_login(sockfd, list,buf,clientaddr);
                   break;
               }



           }
       }
       void parent_process_send(int sockfd)
       {
           char buf[N];
           char msg[N + 20];
           struct sockaddr_in serveraddr, clientaddr;
           socklen_t  addrlen = sizeof(struct sockaddr);


           while(1)
           {
               fgets(buf, N, stdin);
               buf[strlen(buf)-1] = '\0';
               sprintf(msg,"server:%s",buf);
               if(sendto(sockfd, &msg, N, 0, (struct sockaddr *)&serveraddr, addrlen) < 0)
               {
                   err_log("fail to sendto\n");
               }

           }
       }

       int socket_init(const char* family,const char* addr,const char* port)
       {
           int sockfd;
           struct sockaddr_in serveraddr;

           if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
           {
               err_log("fail to socket\n");
           }

           printf("sockfd = %d\n", sockfd);

           serveraddr.sin_family = AF_INET;
           serveraddr.sin_addr.s_addr = inet_addr(addr);
           serveraddr.sin_port = htons(atoi(port));

           if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
           {
               err_log("fail to bind\n");
           }

           return sockfd;
       }

       int main(int argc, const char *argv[])
       {

           int sockfd;
           struct sockaddr_in serveraddr, clientaddr;
           char buf[N] = {};
           pid_t pid;

           if(argc < 3)
           {
               fprintf(stderr, "usage:%s serverip port.\n", argv[0]);
               return -1;
           }

           sockfd= socket_init("AF_INET",argv[1],argv[2]);
           pid =fork();
           if(pid < 0)
           {
               err_log("fail to fork\n");
           }
           if(pid ==0)
           {
               printf("child\n");
               child_process_recv(sockfd);
           }
           if(pid > 0)
           {
               parent_process_send(sockfd);
           }

           close(sockfd);

           return 0;
       }
