#include "header/socket_sender.h"
#include "header/data.h"

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

//#define __DEBUG__

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

SocketSender::SocketSender()
{
    try
    {
        cout<<"Start to read ./config/sockaddr_info.txt configure file..."<<endl;
#ifdef __DEBUG__
        ifstream fin("../client/config/sockaddr_info.txt",ifstream::in);
#else
        ifstream fin("./config/sockaddr_info.txt",ifstream::in);
#endif
        if (!fin.is_open())
        {
            throw ReadException("Open ./config/sockaddr_info.txt failed");
        }
        fin>>this->server_ip;
        fin>>this->server_port;
        fin.close();
    } catch(ClientException & e)
    {
        cout<<e.what()<<endl;
    }
    cout<<"ok: read ./config/sockaddr_info.txt configure file finished."<<endl;
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
        cout<<"Start to connect to server..."<<endl;

        //create a socket for client.
        socket_fd=socket(AF_INET,SOCK_STREAM,0);
        if (socket_fd<0)
        {
           throw SocketException("Create socket failed");
        }
        else
        {
#ifdef __DEBUG__
            cout<<"ok:client create socket."<<endl;
#endif
        }
    } catch (ClientException & e)
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
            throw SocketException("Connect to server failed");
        }
        else
        {
            cout<<"ok: connect to server."<<endl;
        }
    } catch (ClientException  & e)
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
    cout<<"Start to read unsended matched log..."<<endl;
    try
    {
        //open the file storing unsended matched log.
        ifstream fin(unsended_file.c_str(),ifstream::in);
        if (fin.fail())
        {
//#ifdef __DEBUG__
//            cout<<"error:socket sender fail to open file!"<<endl;
//#endif
            throw ReadException("Open file failed");
        }
        else
        {
#ifdef __DEBUG__
            cout<<"ok:open \"unsended_matched_log\" file."<<endl;
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
                    throw ReadException("Read unsended log failed");
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
    } catch (ClientException & e)
    {
        cout<<e.what()<<endl;
    }
    cout<<"ok:read unsended matched log finished."<<endl;
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
        int num_send_log=0;
        int send_num;
        for (list<MatchedLogRec>::iterator it=matched_log.begin();it!=matched_log.end();)
        {
#ifdef __DEBUG__
            sleep(1);
#endif
            send_num=send(socket_fd,(void *)&(*it),sizeof(MatchedLogRec),0);
            if (send_num<0)
            {
//#ifdef __DEBUG__
//                cout<<"error:client socket fail to send data!"<<endl;
//#endif
                throw SendException("Send data to server failed");
            }
            else
            {
#ifdef __DEBUG__
                cout<<*it<<endl;
                cout<<"ok:client socket sended."<<endl;
#endif
                ++num_send_log;
                it=matched_log.erase(it);
            }
        }
        cout<<"Number of sended matched log: "<<num_send_log<<endl;
    } catch (ClientException & e)
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
        ofstream fout(unsended_file.c_str(),ofstream::out|ofstream::trunc);
        if (fout.fail())
        {
//#ifdef __DEBUG__
//            cout<<"error:socket sender fail to open file!"<<endl;
//#endif
            throw SaveException("Open file failed");
        }

        //write unsended log.
        for (list<MatchedLogRec>::const_iterator it=matched_log.begin();it!=matched_log.end();it++)
        {
            fout<<*it<<endl;
            if (fout.fail())
            {
                throw SaveException("Write unsended log failed!");
            }
        }
        fout.close();

        //empty the matched log list.
        matched_log.clear();
    } catch (ClientException & e)
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
