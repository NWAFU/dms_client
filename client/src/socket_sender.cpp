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

//#define _DEBUG
#define _TEST

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

SocketSender::SocketSender()
{
    try
    {
        cout<<"Starting to read \"./config/sockaddr_info.txt\" configure file..."<<endl;

#ifdef _DEBUG
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

    cout<<"OK:read \"./config/sockaddr_info.txt\" configure file finished."<<endl;

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
        cout<<"Starting to connect to server..."<<endl;

        //create a socket for client.
        socket_fd=socket(AF_INET,SOCK_STREAM,0);
        if (socket_fd<0)
        {
            throw SocketException("Create socket failed");
        }
        else
        {
#ifdef _DEBUG
            cout<<"OK:client create socket."<<endl;
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
            throw SocketException("Connect to server failed");
        }
        else
        {
#ifdef _DEBUG
            cout<<"OK:connect to server succeeded."<<endl;
#endif
        }
#ifndef _DEBUG
        cout<<"OK:connect to server succeeded."<<endl;
#endif
    } catch (ClientException  & e)
    {
        cout<<e.what()<<endl;
        exit(1);
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
    cout<<"Starting to read unsended matched log..."<<endl;
#ifdef _TEST
    int num_read=0;
#endif
    try
    {
        //open the file storing unsended matched log.
        ifstream fin(unsended_file.c_str(),ifstream::in);
        if (fin.fail())
        {
            throw ReadException("Open file failed");
        }
        else
        {
#ifdef _DEBUG
            cout<<"OK:open \"unsended_matched_log\" file."<<endl;
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
            ++num_read;
        }

        //close the file.
        fin.close();

        cout<<"OK:read unsended matched log finished."<<endl;

#ifdef _TEST
        cout<<"Number of matched log read:"<<num_read<<endl;
#endif
    } catch (ClientException & e)
    {
        cout<<e.what()<<endl;
        exit(-1);
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
    int num_send_log=0;
    try
    {
        cout<<"Starting to send data to server..."<<endl;
        int send_num;
        for (list<MatchedLogRec>::iterator it=matched_log.begin();it!=matched_log.end();)
        {
#ifdef _DEBUG
            sleep(1);
#endif
            send_num=send(socket_fd,(void *)&(*it),sizeof(MatchedLogRec),0);
            if (send_num<0)
            {
                throw SendException("Send data to server failed");
            }
            else
            {
#ifdef _DEBUG
                cout<<*it<<endl;
                cout<<"OK:client socket sended."<<endl;
#endif
                ++num_send_log;
                it=matched_log.erase(it);
            }
        }
        cout<<"OK:send data to server finished."<<endl;
        cout<<"Total number of sended matched log: "<<num_send_log<<endl;
    } catch (ClientException & e)
    {
        cout<<e.what()<<endl;
        cout<<"Total number of sended matched log: "<<num_send_log<<endl;
        exit(-1);
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
#ifdef _TEST
    int num_save=0;
#endif

    try
    {
        cout<<"Starting to save unsended matched log..."<<endl;

        //open file.
        ofstream fout(unsended_file.c_str(),ofstream::out|ofstream::trunc);
        if (fout.fail())
        {
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
            ++num_save;
        }
        fout.close();

        //empty the matched log list.
        matched_log.clear();

        cout<<"OK:save unsended matched log finished."<<endl;

#ifdef _TEST
        cout<<"Number of matched log saved:"<<num_save<<endl;
#endif
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
#ifdef _TEST
    cout<<endl;
    cout<<"Before read unsended matched log file:"<<endl;
    cout<<"Number of matched log:"<<matched_log.size()<<endl;
#endif
    readUnsendedFile(matched_log);
#ifdef _TEST
    cout<<"After read unsended matched log file:"<<endl;
    cout<<"Number of matched log:"<<matched_log.size()<<endl;
    cout<<endl;
    cout<<"Before send data to server:"<<endl;
    cout<<"Number of matched log:"<<matched_log.size()<<endl;
#endif
    sendData(matched_log);
#ifdef _TEST
    cout<<"After send data to server:"<<endl;
    cout<<"Number of matched log:"<<endl;
    cout<<endl;
    cout<<"Before save unsended matched log into file:"<<endl;
    cout<<"Number of matched log:"<<matched_log.size()<<endl;
#endif
    saveUnsendedFile(matched_log);
#ifdef _TEST
    cout<<"After save unsended matched log into file:"<<endl;
#endif
}
