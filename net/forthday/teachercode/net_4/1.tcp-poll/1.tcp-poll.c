#include <string.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/select.h>  
#include <sys/time.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <poll.h>  
#include <errno.h>  

#define OPEN_MAX 100  
  
int main(int argc, char *argv[])  
{  
    //1.����tcp�����׽���  
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);  
      
    //2.��sockfd  
    struct sockaddr_in my_addr;  
    bzero(&my_addr, sizeof(my_addr));  
    my_addr.sin_family = AF_INET;  
    my_addr.sin_port = htons(8000);  
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
    bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr));  
      
    //3.����listen  
    listen(sockfd, 10);  
      
    //4.poll��Ӧ����׼��  
    struct pollfd client[OPEN_MAX];  
    int i = 0, maxi = 0;  
    for(;i<OPEN_MAX; i++)  
        client[i].fd = -1;//��ʼ��poll�ṹ�е��ļ�������fd  
      
    client[0].fd = sockfd;//��Ҫ����������  
    client[0].events = POLLIN;//��ͨ�����ȼ������ݿɶ�  
      
    //5.�������ӵĿͻ��˵����ݴ���  
    while(1)  
    {  
        int ret = poll(client, maxi+1, -1);//�Լ���poll�ṹ����������Ԫ�ؽ��м��  
          
        //5.1���sockfd(�����׽���)�Ƿ��������  
        if((client[0].revents & POLLIN) == POLLIN )  
        {  
            struct sockaddr_in cli_addr;  
            int clilen = sizeof(cli_addr);  
            int connfd = 0;  
            //5.1.1 ��tcp�����������ȡ�ͻ���  
            connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);  
              
            //5.1.2 ����ȡ����connfd����poll�ṹ�������У��Ա���poll�������  
            for(i=1; i<OPEN_MAX; i++)  
            {  
                if(client[i].fd < 0)  
                {  
                    client[i].fd = connfd;  
                    client[i].events = POLLIN;  
                    break;  
                }  
            }  
              
            //5.1.3 maxi����  
            if(i > maxi)  
                maxi = i;  
                  
            //5.1.4 ���û�о��������������ͼ���poll��⣬����������¿�  
            if(--ret <= 0)  
                continue;  
        }  
          
        //5.2������Ӧ������������  
        for(i=1; i<=maxi; i++)  
        {  
            if(client[i].fd < 0)  
                continue;  
              
            if(client[i].revents & (POLLIN | POLLERR))  
            {  
                int len = 0;  
                char buf[128] = "";  
                  
                //5.2.1���ܿͻ�������  
                if((len = recv(client[i].fd, buf, sizeof(buf), 0)) < 0)  
                {  
                    if(errno == ECONNRESET)//tcp���ӳ�ʱ��RST  
                    {  
                        close(client[i].fd);  
                        client[i].fd = -1;  
                    }  
                    else  
                        perror("read error:");  
                      
                }  
                else if(len == 0)//�ͻ��˹ر�����  
                {  
                    close(client[i].fd);  
                    client[i].fd = -1;  
                }  
                else//�������յ�������������  
                    send(client[i].fd, buf, len, 0);  
                  
                //5.2.2���еľ����������������ˣ����˳���ǰ��forѭ��������poll���  
                if(--ret <= 0)  
                    break;  
                  
            }  
        }  
    }  
    return 0;  
}  
