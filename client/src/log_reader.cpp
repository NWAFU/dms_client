#include "header/log_reader.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#include "header/client_exception.h"
#include "header/read_exception.h"
#include "header/save_exception.h"
#include "header/backup_exception.h"

#define _DEBUG 1

using std::cout;
using std::endl;
using std::ios;
using std::ifstream;
using std::ofstream;

LogReader::LogReader()
{
    this->log_file="wtmpx";
    this->unmatched_login_file="unmatched_login_file";
}

LogReader::LogReader(string log_file, string unmatched_login_file)
{
    this->log_file=log_file;
    this->unmatched_login_file=unmatched_login_file;
}

LogReader::~LogReader()
{

}

/**************************************************
*作者：Yang Xiaotian
*日期：2017.06.14
*函数名：backup
*功能：Read the total dispatch function of the log file to get a matched collection
*输入参数：The original log file that needs to be backed up is fixed and does not need to be imported
*输出参数：return 1,backup wrong;return 2,clear wrong;return 0,correct
*返回值：none
**************************************************/
void LogReader::backup()
{
#ifdef _DEBUG
    cout << "backup start!......" << endl;
#endif
    try
    {
        time_t t = time(0);
        char tmp[64];
        char wtmpx[] ="wtmpx.";
        strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", localtime(&t));
        backup_file = strcat(wtmpx, tmp);
#ifdef _DEBUG
        char cmd[] = "../client/script/backup.sh wtmpx wtmpx.";
#else
        char cmd[] = "./script/backup.sh wtmpx wtmpx.";
#endif
        strcat(cmd, tmp);
        int status = system(cmd);
        int ret = WEXITSTATUS(status);
        if(ret == 1)
        {
            throw BackupException("backup fail");
        }
        else if(ret == 2)
        {
            throw BackupException("clean fail");
        }
        else if(ret == 0)
        {
            cout << "ok: backup is complete!" <<endl;
//            cout << tmp << endl;
//            backup_file = strcat(wtmpx, tmp);
//            cout << backup_file << endl;
        }
    }
    catch (ClientException &e)
    {
        cout << e.what() << endl;
    }

#ifdef _DEBUG
    cout << "backup end!......" << endl;
#endif
}

/**************************************************
*作者：Yang Xiaotian
*日期：2017.06.16
*函数名：readUnmatchedFile
*功能：Read the last login record that did not match
*输入参数：The login file that didn't matched is fixed and does not need to be imported
*输出参数：void
*返回值：none
**************************************************/
void LogReader::readUnmatchedFile()
{
    try
    {
        ifstream fin;
        int size;
        LogRec loginRec;
        pid_t pid;
        short type;
        int time;
        fin.open(unmatched_login_file.c_str(), ios::in|ios::binary);
        if(fin.fail())
        {
            throw ReadException("unmatched login file read fail");
        }
        else
        {
            fin.seekg(0, ios::end);
            size = fin.tellg();
#ifdef  _DEBUG
            cout << "File unmatched file size: " << size << endl;
#endif
        }
        //read unmatched file.
        for(int i=0;i<(size/74);i++)
        {
            fin.seekg(0, ios::beg);
            fin.read(loginRec.log_name, 32);

            fin.read((char *)&pid, 4);
            loginRec.pid = pid;

            fin.read((char *)&type, 2);
            loginRec.log_type = type;

            fin.read((char *)&time, 4);
            loginRec.log_time = time;

            fin.read(loginRec.log_ip, 32);

            login_record.push_back(loginRec);
        }
        fin.close();
    }
    catch (ClientException & e)
    {
        cout << e.what() << endl;
    }

}


/**************************************************
*作者：Yang Xiaotian
*日期：2017.06.15
*函数名：readBackupFile
*功能：Read the backup log file, extract the read data, generate login or log out of the log, and the record into the corresponding collection
*输入参数：void
*输出参数：void
*返回值：none
**************************************************/

void LogReader::readBackupFile()
{
    try
    {
        ifstream fin;
        long size;
        LogRec lr;
        short type;
        pid_t pid;
        int time;
        fin.open(backup_file.c_str(), ios::in|ios::binary);
        if (fin.fail())
        {
            throw ReadException("read backup file failed");
        }
        else
        {
            fin.seekg(0, ios::end);
            size = fin.tellg();
#ifdef _DEBUG
            cout << "File wtmpx size: " << size << endl;
#endif
        }
        //read file.
        for(int begin = 0;begin < size;begin += 372)
        {
            fin.seekg(begin, ios::beg);
            fin.read(lr.log_name, 32);

            fin.seekg(36, ios::cur);
            fin.read((char *)&pid, 4);
            pid = ntohl(pid);
            lr.pid = pid;

            fin.read((char *)&type, 2);
            type = ntohs(type);
            lr.log_type = type;

            fin.seekg(6, ios::cur);
            fin.read((char *)&time, 4);
            time = ntohl(time);
            lr.log_time = time;

            fin.seekg(30, ios::cur);
            fin.read(lr.log_ip, 32);

            if(type ==7)
            {
                login_record.push_back(lr);
#ifdef _DEBUG
                cout << "login pushed" << endl;
#endif
            }
            else if(type == 8)
            {
                logout_record.push_back(lr);
#ifdef _DEBUG
                cout << "logout pushed" << endl;
#endif
            }
#ifdef _DEBUG
            else
            {
                cout << "name: " << lr.log_name <<" tpye wrong!" << "can't push." << endl;
            }
#endif
        }
        cout << "ok: read date in the file is complete." << endl;
        fin.close();
    }
    catch(ClientException & e)
    {
        cout << e.what() << endl;
    }
}

/**************************************************
*作者：Yang Xiaotian
*日期：2017.06.16
*函数名：match
*功能：Match the user login / logout record into a complete login record into matches
*输入参数：void
*输出参数：void
*返回值：none
**************************************************/

void LogReader::match()
{
    try
    {
        LogRec logoutRec;
        LogRec loginRec;
        MatchedLogRec matchedLogRec;
        list<LogRec>::iterator iter1;       //logout iterator
        list<LogRec>::iterator iter2;       //login iterator
        iter1 = logout_record.begin();
        while(iter1 != logout_record.end())
        {
            logoutRec = *iter1;
            iter2 = login_record.begin();
            while(iter2 != login_record.end())
            {
                loginRec = *iter2;
                if(strcmp(loginRec.log_name, logoutRec.log_name) == 0 &&
                        loginRec.pid == logoutRec.pid &&
                        strcmp(loginRec.log_ip, logoutRec.log_ip) == 0)
                {
                    strcpy(matchedLogRec.log_name, logoutRec.log_name);
                    matchedLogRec.pid = logoutRec.pid;
                    matchedLogRec.login_time = loginRec.log_time;
                    matchedLogRec.logout_time = logoutRec.log_time;
                    matchedLogRec.duration = matchedLogRec.logout_time - matchedLogRec.login_time;
                    strcpy(matchedLogRec.log_ip, logoutRec.log_ip);

                    matched_log_record.push_back(matchedLogRec);

                    iter1=logout_record.erase(iter1);
                    iter2=login_record.erase(iter2);
                    iter1--;
                    break;
                }
                else
                {
                    if(login_record.empty())
                    {
                        throw ClientException("login_record empty");
                    }
                }
                iter2++;
            }
            if(iter2 == login_record.end())
            {
                iter1 = logout_record.erase(iter1);
                continue;
            }
            iter1++;
        }
        cout << "ok: date match is complete." << endl;
    }
    catch(ClientException & e)
    {
        cout << e.what() << endl;
    }
}

/**************************************************
*作者：Yang Xiaotian
*日期：2017.06.16
*函数名：saveUnmatchedLogin
*功能：Save the failed log records in the file
*输入参数：void
*输出参数：void
*返回值：none
**************************************************/

void LogReader::saveUnmatchedLogin()
{
    try
    {
        if(login_record.empty())
        {
            throw SaveException("don't have unmatched login");
        }
        ofstream fout;
        LogRec loginRec;
        list<LogRec>::iterator itor;
        itor = login_record.begin();
        pid_t pid;
        short type;
        int time;

        fout.open(unmatched_login_file.c_str(), ios::trunc|ios::out|ios::binary);     //everytime clear this file.
        if(fout.fail())
        {
            throw SaveException("create unmatched login file fail");
        }
        while(itor != login_record.end())
        {
            loginRec = *itor;
            fout.write(loginRec.log_name, 32);
            pid = loginRec.pid;
            fout.write((char *)&pid, 4);
            type = loginRec.log_type;
            fout.write((char *)&type, 2);
            time = loginRec.log_time;
            fout.write((char *)&time, 4);
            fout.write(loginRec.log_ip, 32);
            itor++;
        }
        fout.close();
        login_record.clear();       //clear this list.
    }
    catch(ClientException & e)
    {
        cout << e.what() << endl;
    }
}

/**************************************************
*作者：Yang Xiaotian
*日期：2017.06.14
*函数名：readLog
*功能：Read the total dispatch function of the log file
*输入参数：No parameters, the need to back up the original log file, need to read the unmatched login log file path is fixed, so do not need to pass
*输出参数：void
*返回值：list<MatchedLogRec>& Will return a good set of login records, and later passed to the data sending module
**************************************************/

list<MatchedLogRec>& LogReader::readLog()
{
    backup();
    readUnmatchedFile();
    readBackupFile();
    match();
    saveUnmatchedLogin();
    return matched_log_record;
}
