#include "header/socket_sender.h"
#include "header/data.h"
//#include <linux/socket.h>//socket()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>//sockaddr_in
#include <arpa/inet.h>
#include <string.h>//memset()
#include <iostream>
#include <fstream>

#define __DEFINED__ 1
#define __DEBUG__ __DEFINED__
#define SERVER_PORT 4096
#define SERVER_IP_ADDRESS "127.0.0.1"

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

SocketSender::SocketSender()
{
    this->server_ip=SERVER_IP_ADDRESS;
    this->server_port=SERVER_PORT;
    this->unsended_file="unsended_matched_log.txt";
}

SocketSender::~SocketSender()
{
    close(socket_fd);
    close(connent_fd);
}

/**************************************************
*作者：Liu Huisen
*日期：2017.06.10
*函数名：connectServer
*功能：Create a connection to server.
*输入参数：none
*输出参数：none
*返回值：none
**************************************************/
void SocketSender::connectServer()
{
    //create a socket for client
    socket_fd=socket(AF_INET,SOCK_STREAM,0);
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

    //server sockaddr info.
    sockaddr_in server_sockaddr;
    memset(&server_sockaddr,0,sizeof(sockaddr_in));
    server_sockaddr.sin_family=AF_INET;
    server_sockaddr.sin_addr.s_addr=inet_addr(server_ip.c_str());
    server_sockaddr.sin_port=htons(server_port);

    //create a connection request to server.
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

/**************************************************
*作者：Liu Huisen
*日期：2017.06.12
*函数名：readUnsendedFile
*功能：Read the file storing log records fialing to
*     be sended.
*输入参数：matched_log
*输出参数：none
*返回值：none
**************************************************/
void SocketSender::readUnsendedFile(list<MatchedLogRec> & matched_log)
{
    //open the file storing unsended matched log.
    ifstream fin(unsended_file.c_str(),ifstream::in);
    if (fin.fail())
    {
#ifdef __DEBUG__
        cout<<"error:socket sender open file!"<<endl;
#endif
        return;
    }
    else
    {
#ifdef __DEBUG__
        cout<<"ok:socket sender open file."<<endl;
#endif
    }

    //read the matched log and insert into the matched_log list.
    MatchedLogRec log;

    while (!fin.eof())
    {
        fin>>log;
        matched_log.push_front(log);
    }

    //close the file.
    fin.close();
}

/**************************************************
*作者：Liu Huisen
*日期：2017.06.13
*函数名：sendData
*功能：Send matched log to server though socket connection.
*输入参数：matched_log
*输出参数：none
*返回值：none
**************************************************/
void SocketSender::sendData(list<MatchedLogRec> & matched_log)
{
    MatchedLogRec log;
    char send_buf[128];
    int send_num;
    while (!matched_log.empty())
    {
        log=matched_log.front();
        sprintf(send_buf,"%s %d %d %d %d %s\n",log.log_name,log.pid,log.login_time,log.logout_time,log.duration,log.log_ip);
        send_num=send(socket_fd,send_buf,sizeof(send_buf),0);
        if (send_num<0)
        {
#ifdef __DEBUG__
            cout<<"error:client socket send failed!"<<endl;
#endif
            return;
        }
        else
        {
#ifdef __DEBUG__
            cout<<"ok:client socket sended."<<endl;
#endif
        }
    }
}

/**************************************************
*作者：Liu Huisen
*日期：2017.06.12
*函数名：saveUnsendedFile
*功能：Save log record failing to be sended into file.
*输入参数：matched_log
*输出参数：none
*返回值：none
**************************************************/
void SocketSender::saveUnsendedFile(list<MatchedLogRec> & matched_log)
{
    ofstream fout(unsended_file.c_str(),ofstream::out);
    if (fout.fail())
    {
#ifdef __DEBUG__
        cout<<"error:socket sender open file!"<<endl;
#endif
        return;
    }

    while (!matched_log.empty())
    {
        fout<<matched_log.front();
        matched_log.pop_front();
    }

    fout.close();
}

/**************************************************
*作者：Liu Huisen
*日期：2017.06.12
*函数名：sendLog
*功能：Send log to server.
*输入参数：matched_log
*输出参数：none
*返回值：none
**************************************************/
void SocketSender::sendLog(list<MatchedLogRec> & matched_log)
{
    connectServer();
    readUnsendedFile(matched_log);
    sendData(matched_log);
    saveUnsendedFile(matched_log);
}
