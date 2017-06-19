#include "header/socket_sender.h"
#include "header/data.h"

//#include <linux/socket.h>//socket()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>//sockaddr_in
#include <arpa/inet.h>
#include <string.h>//memset()
#include <stdlib.h>//system()
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
        //create a socket for client.
        socket_fd=socket(AF_INET,SOCK_STREAM,0);
        if (socket_fd<0)
        {
#ifdef __DEBUG__
            cout<<"error:client fail to create socket!"<<endl;
#endif
            SocketException se("Create socket failed!");
            SocketException & sock_e = se;
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
        cout<<e.what()<<endl;
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
            cout<<"error:client fail to connect to server!"<<endl;
#endif
            SocketException se("Connect to server failed!");
            SocketException & sock_e = se;
            throw sock_e;
        }
        else
        {
#ifdef __DEBUG__
            cout<<"ok:client connect to server."<<endl;
#endif
        }
    } catch (exception const & e)
    {
        cout<<e.what()<<endl;
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
            cout<<"error:socket sender fail to open file!"<<endl;
#endif
            SaveException se("Open file failed!");
            SaveException & save_e = se;
            throw save_e;
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
                    ReadException re("Read unsended log failed!");
                    ReadException & read_e = re;
                    throw read_e;
                    return;
                }
                else
                {
                    break;
                }

            }
            matched_log.push_back(log);
        }

        //close the file.
        fin.close();

        //clean up the unsended log file.
        char command[64]="../client/script/cleanup_file.sh ";
        int cleanup=system(strcat(command,unsended_file.c_str()));
        if (cleanup<0)
        {
#ifdef __DEBUG__
            cout<<"error:client fail to clean up unsended log file!"<<endl;
#endif
            SaveException se("Clean up unsended log file failed!");
            SaveException & save_e = se;
            throw save_e;
            return;
        }
        else
        {
#ifdef __DEBUG__
            cout<<"ok:clean up unsended log file."<<endl;
#endif
        }
    } catch (exception const & e)
    {
        cout<<e.what()<<endl;
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
        for (list<MatchedLogRec>::iterator it=matched_log.begin();it!=matched_log.end();)
        {
            send_num=send(socket_fd,(void *)&(*it),sizeof(log),0);
            if (send_num<0)
            {
#ifdef __DEBUG__
                cout<<"error:client socket fail to send data!"<<endl;
#endif
                SendException se("Send data to server failed!");
                SendException & send_e = se;
                throw send_e;
                return;
            }
            else
            {
#ifdef __DEBUG__
                cout<<"ok:client socket sended."<<endl;
#endif
            }
            it=matched_log.erase(it);
        }
    } catch (exception const & e)
    {
        cout<<e.what()<<endl;
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
        //open file.
        ofstream fout(unsended_file.c_str(),ofstream::out);
        if (fout.fail())
        {
#ifdef __DEBUG__
            cout<<"error:socket sender fail to open file!"<<endl;
#endif        
            SaveException se("Open file failed!");
            SaveException & save_e  = se;
            throw save_e;
            return;
        }

        //write unsended log.
        for (list<MatchedLogRec>::const_iterator it=matched_log.begin();it!=matched_log.end();it++)
        {
            fout<<*it<<endl;
            if (fout.fail())
            {
                SaveException se("Write unsended log failed!");
                SaveException & save_e = se;
                throw save_e;
                return;
            }
        }
        fout.close();

        //empty the matched log list.
        matched_log.clear();
    } catch (exception const & e)
    {
        cout<<e.what()<<endl;
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
