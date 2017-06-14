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

#include "header/read_exception.h"
#include "header/save_exception.h"
#include "header/socket_exception.h"
#include "header/send_exception.h"

#define __DEBUG__ 1
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
    try
    {
        //create a socket for client
        socket_fd=socket(AF_INET,SOCK_STREAM,0);
        if (socket_fd<0)
        {
#ifdef __DEBUG__
            cout<<"error:client create socket!"<<endl;
#endif
            SocketException sock_e("Create socket failed!");
            throw sock_e;
            return;
        }
        else
        {
#ifdef __DEBUG__
            cout<<"ok:client create socket."<<endl;
#endif
        }
    } catch (exception const & e)
    {
        e.what();
    }

    //server sockaddr info.
    sockaddr_in server_sockaddr;
    memset(&server_sockaddr,0,sizeof(sockaddr_in));
    server_sockaddr.sin_family=AF_INET;
    server_sockaddr.sin_addr.s_addr=inet_addr(server_ip.c_str());
    server_sockaddr.sin_port=htons(server_port);

    try
    {
        //create a connection request to server.
        int connet_fd=connect(socket_fd,(struct sockaddr *)&server_sockaddr,sizeof(struct sockaddr));
        if (connet_fd<0)
        {
#ifdef __DEBUG__
            cout<<"error:client connect to server!"<<endl;
#endif
            SocketException sock_e("Connect to server failed!");
            throw sock_e;
            return;
        }
        else
        {
#ifdef __DEBUG__
            cout<<"ok:client connect to server."<<endl;
#endif
        }
    } catch (exception const & e)
    {
        e.what();
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
    try
    {
        //open the file storing unsended matched log.
        ifstream fin(unsended_file.c_str(),ifstream::in);
        if (fin.fail())
        {
#ifdef __DEBUG__
            cout<<"error:socket sender open file!"<<endl;
#endif
            SaveException read_e("Open file failed!");
            throw read_e;
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
            if (fin.fail())
            {
                if (!fin.eof())
                {
                    ReadException read_e("Read unsended log failed!");
                    throw read_e;
                    return;
                }
            }
            matched_log.push_front(log);
        }

        //close the file.
        fin.close();
    } catch (exception const & e)
    {
        e.what();
    }
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
    try
    {
        MatchedLogRec log;
        int send_num;
        while (!matched_log.empty())
        {
            log=matched_log.front();
            send_num=send(socket_fd,(void *)&log,sizeof(log),0);
            if (send_num<0)
            {
#ifdef __DEBUG__
                cout<<"error:client socket send failed!"<<endl;
#endif
                SendException send_e("Send data to server failed!");
                throw send_e;
                return;
            }
            else
            {
#ifdef __DEBUG__
                cout<<"ok:client socket sended."<<endl;
#endif
            }
        }
    } catch (exception const & e)
    {
        e.what();
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
    try
    {
        ofstream fout(unsended_file.c_str(),ofstream::out);
        if (fout.fail())
        {
#ifdef __DEBUG__
            cout<<"error:socket sender open file!"<<endl;
#endif
            SaveException save_e("Open file failed!");
            throw save_e;
            return;
        }

        while (!matched_log.empty())
        {
            fout<<matched_log.front();
            if (fout.fail())
            {
                SaveException save_e("Write unsended log failed!");
                throw save_e;
                return;
            }
            matched_log.pop_front();
        }

        fout.close();
    } catch (exception const & e)
    {
        e.what();
    }
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
