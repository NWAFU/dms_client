#include "header/socket_sender.h"
#include "header/data.h"
#include <linux/socket.h>//socket()
#include <iostream>
#include <netinet/in.h>//sockaddr_in
#include <string.h>
#include <fstream>

#define __DEBUG__ __DEFINED__
#define SERVER_PORT 4096
#define SERVER_IP_ADDRESS "127.0.0.1"

using namespace std::out;
using namespace std::endl;

SocketSender::SocketSender()
{
    this->server_ip=SERVER_IP_ADDRESS;
    this->server_port=SERVER_PORT;
    this->unsended_file="unsended_matched_log.txt";
}

SocketSender::~SocketSender()
{
    close(this->socket_fd);
    close(this->connent_fd);
}

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
    //create a socket for client
    socket_fd=socket(AF_FAMILY,SOCKET_STREAM,0);
    if (socket_fd<0)
    {
#ifdef __DEBUG__ __DEFINED__
        cout<<"error:client create socket!"<<endl;
#endif
        return -1;
    }
    else
    {
#ifdef __DEBUG__ __DEFINED__
        cout<<"ok:client create socket."<<endl;
#endif
    }

    //server sockaddr info.
    sockaddr_in server_sockaddr;
    memset(&server_sockaddr,0,sizeof(sockaddr_in));
    server_sockaddr.sin_family=AF_INET;
    server_sockaddr.sin_addr.s_addr=inet_addr(server_ip);
    server_sockaddr.sin_port=htons(server_port);

    //create a connection request to server.
    int connet_fd=connect(socket_fd,(struct sockaddr *)server_sockaddr,sizeof(struct sockaddr));
    if (connet_fd<0)
    {
#ifdef __DEBUG__ __DEFINED__
        cout<<"error:client connect to server!"<<endl;
#endif
        return -1;
    }
    else
    {
#ifdef __DEBUG__ __DEFINED__
        cout<<"ok:client connect to server."<<endl;
#endif
    }
}

void SocketSender::readUnsendedFile(list<MatchedLogRec> & matched_log)
{
    //open the file storing unsended matched log.
    ifstream fin("unsended_matched_log.txt",ios::nocreate);
    if (fin.fail())
    {
#ifdef __DEBUG__ __DEFINED__
        cout<<"error:socket sender open file!"<<endl;
#endif
        return -1;
    }
    else
    {
#ifdef __DEBUG__ __DEFINED__
        cout<<"ok:socket sender open file."<<endl;
#endif
    }

    //read the matched log and insert into the matched_log list.
    MatchedLogRec log;
    while (fin>>log)
    {
        matched_log.push_front(log);
    }

    //close the file.
    fin.close();
}
