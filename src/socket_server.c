#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "socket_server.h"

#define MAX_CONN 10

typedef struct ClientInfo
{
    struct sockaddr_in addr;
    int clientfd;
    int isConn;
    int index;
}ClientInfo;
struct socket_server_t
{
    int             socket_server_fd;
    struct          sockaddr_in socket_server_addr;

    int             activeConnCount;
    ClientInfo      clients[MAX_CONN];
    pthread_t       clients_thread_id[MAX_CONN];
    pthread_mutex_t clients_mutex[MAX_CONN];

    pthread_mutex_t activeConnMutex;
    pthread_cond_t  connDis;//断开socket连接
    int             socket_server_status;//socket server状态:0 停止，1 运行
};

void send_thread(void *arg);
void recv_thread(void *arg);

socket_server_p socket_server_create(int port,SOCKET_MODE mode)
{
    socket_server_p server = (socket_server_p)malloc(sizeof(struct socket_server_t));
    if(server == NULL)
    {
        printf("socket_server:malloc failed...\n");
        return NULL;
    }

    if(mode == TCP)
    {
        server->socket_server_fd = socket(AF_INET,SOCK_STREAM,0);
    }

    if(server->socket_server_fd == -1)
    {
        printf("socket_server: create socket failed...\n");
        return NULL;
    }

    memset(&(server->socket_server_addr),0,sizeof(server->socket_server_addr));

    server->socket_server_addr.sin_family = AF_INET;
    server->socket_server_addr.sin_port = htons(port);
    server->socket_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int optval = 1;
    if(setsockopt(server->socket_server_fd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval)) < 0)
    {
        printf("setsockopt failed...\n");
        return NULL;
    }
    if(bind(server->socket_server_fd,(struct sockaddr*)&server->socket_server_addr,sizeof(server->socket_server_addr)) == -1)
    {
        printf("socket_server:bind failed...\n");
        return NULL;
    }

    if(listen(server->socket_server_fd,10) == -1)
    {
        printf("socket_server:listen failed...\n");
        return NULL;
    }

    server->activeConnCount = 0;
    server->socket_server_status = 0;//默认socket没有运行
    pthread_mutex_init(&server->activeConnMutex,NULL);
    pthread_cond_init(&server->connDis,NULL);
    for(int i = 0; i < MAX_CONN;i++)
    {
        server->clients[i].isConn = 0;
    }
    return server;
}
void socket_server_start(socket_server_p server)
{
    if(server == NULL)
    {
        return;
    }

    server->socket_server_status = 1;//socket运行
    //设置客户端
    while(1)
    {
        if(server->socket_server_status == 0)
        {
            printf("server exit...\n");
            //退出服务器
            return;
        }

        pthread_mutex_lock(&(server->activeConnMutex));
        if(server->activeConnCount >= MAX_CONN)
        {
            pthread_cond_wait(&(server->connDis),&(server->activeConnMutex));
        }
        pthread_mutex_unlock(&(server->activeConnMutex));

        //查找一个空的位置放置新的连接
        int i = 0;
        while (i < MAX_CONN)
        {
            pthread_mutex_lock(&(server->clients_mutex[i]));
            if(!(server->clients[i].isConn))
            {
                pthread_mutex_unlock(&(server->clients_mutex[i]));
                break;
            }

            pthread_mutex_unlock(&(server->clients_mutex[i]));
            i++;
        }

        //接受客户端的连接

        struct sockaddr_in socket_client_addr;
        socklen_t client_addr_len = sizeof(socket_client_addr);
        int socket_client_fd = accept(server->socket_server_fd,(struct sockaddr *)&socket_client_addr,&client_addr_len);
        if(socket_client_fd == -1)
        {
            sleep(1);
            continue;
        }

        server->activeConnCount++;
        printf("newConnection:%d\n",i);
        //执行到此说明新的连接创建成功
        pthread_mutex_lock(&server->clients_mutex[i]);
        server->clients[i].clientfd = socket_client_fd;
        server->clients[i].isConn = 1;
        server->clients[i].index = i;
        pthread_mutex_unlock(&(server->clients_mutex[i]));
        if(pthread_create(&(server->clients_thread_id[i]),NULL,(void *)recv_thread,&server->clients[i]) != 0)
        {
            printf("create thread failed...\n");
        }
    }

}

void send_thread(void *arg)
{

}
void recv_thread(void *arg)
{
    printf("-------1\n");
    if(arg == NULL)
    {
        return;
    }
    ClientInfo *client = (ClientInfo *)arg;

    int flag = 0;
    char buf[1024] = {0};
    printf("------- isConn = %d\n",client->isConn);
    while(client->isConn)
    {
        printf("\n等待接收数据:\n");
        fflush(NULL);
        flag = recv(client->clientfd,buf,sizeof(buf),0);
        if(flag == 0)
        {
            printf("对方已关闭连接\n");
            return;
        }
        else if(flag == -1)
        {
            printf("client index = %d recv message failed...\n",client->index);
        }
        printf("from:%s:%s\n",inet_ntoa(client->addr.sin_addr),buf);
        memset(buf,0,sizeof(buf));
    }
    return;
}
//
// void socket_set_message_callback(char *msg)
// {
//
// }
// int socket_send_message(int socketfd,char *msg)
// {
//     return 0;
// }
