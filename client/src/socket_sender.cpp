#include "header/socket_sender.h"
#include <linux/socket.h>//socket()
#include <iostream>
#include <netinet/in.h>//sockaddr_in
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define __DEFINED__ 1
#define __DEBUG__ __DEFINED__
#define SERVER_PORT 4096
#define SERVER_IP_ADDRESS "127.0.0.1"

using std::cout;
using std::endl;

/**************************************************
*作者：Liu Huisen
*日期：
*函数名：connectServer
*功能：Create a connection to server.
*输入参数：none
*输出参数：none
*返回值：none
**************************************************/
void SocketSender::connectServer()
{
    socket_fd=socket(AF_INET,SOCK_STREAM,0);//create a socket
    if (socket_fd<0)
    {
#ifdef __DEBUG__
        cout<<"error:client create socket!"<<endl;
#endif
        return;
    }
    else
    {
#ifdef __DEBUG__
        cout<<"ok:client create socket."<<endl;
#endif
    }

    sockaddr_in server_sockaddr;
    memset(&server_sockaddr,0,sizeof(sockaddr_in));
    server_sockaddr.sin_family=AF_INET;
    server_sockaddr.sin_addr.s_addr=inet_addr(SERVER_IP_ADDRESS);
    server_sockaddr.sin_port=htons(SERVER_PORT);

    int connet_fd=connect(socket_fd,(struct sockaddr *)&server_sockaddr,sizeof(struct sockaddr));
    if (connet_fd<0)
    {
#ifdef __DEBUG__
        cout<<"error:client connect to server!"<<endl;
#endif
        return;
    }
    else
    {
#ifdef __DEBUG__
        cout<<"ok:client connect to server."<<endl;
#endif
    }
}
