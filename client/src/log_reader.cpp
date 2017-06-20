#include "header/log_reader.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>

//#define _DEBUG 1

using std::cout;
using std::endl;
using std::ios;
using std::ifstream;
using std::ofstream;

LogReader::LogReader()
{

}

LogReader::LogReader(string backup_file, string unmatched_login_file)
{
    this->backup_file=backup_file;
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
    //code
    char cmd[] = "../client/script/backup.sh wtmpx";
    int status = system(cmd);
    int ret = WEXITSTATUS(status);
    if(ret == 1) cout<<"backup wrong!"<<endl;
    else if(ret == 2) cout<<"clear wrong!"<<endl;
    else if(ret == 0) cout<<"backup success!"<<endl;

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
        ifstream fin;
        int size;
        LogRec loginRec;
        //unmatched_login_file = "/unmatched_login_file";
        pid_t pid;
        short type;
        int time;
        fin.open(unmatched_login_file.c_str(), ios::in|ios::binary);
        if(fin.fail())
        {
            cout << "read unmatched file failed!" << endl;
        }
        else
        {
            fin.seekg(0, ios::end);
            size = fin.tellg();
            cout << "File unmatched file size: " << size << endl;
        }
        //read unmatched file.
        for(int i=0;i<(size/72);i++)
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
    ifstream fin;
    long size;                      //binary file size.
    LogRec lr;                      //
    backup_file = "/home/viki/dms_client/client/wtmpx.20170322103850";

    short type;
    pid_t pid;
    int time;

    fin.open(backup_file.c_str(), ios::in|ios::binary);
    if (fin.fail())
    {
        cout<<"read backup file failed!"<<endl;
        return;
    }
    else
    {
        fin.seekg(0, ios::end);
        size = fin.tellg();
        cout << "File wtmpx size: " << size << endl;
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
            cout << "login pushed" << endl;
        }
        else if(type == 8)
        {
            logout_record.push_back(lr);
            //cout << "logout pushed" << endl;
        }
        else
        {
            cout << "name: " << lr.log_name <<" tpye wrong!" << "can't push." << endl;
        }
    }
    fin.close();
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
    LogRec logoutRec;
    LogRec loginRec;
    MatchedLogRec matchedLogRec;
    list<LogRec>::iterator itor1;       //logout iterator
    list<LogRec>::iterator itor2;       //login iterator
    itor1 = logout_record.begin();
    while(itor1 != logout_record.end())
    {
        logoutRec = *itor1;
        itor2 = login_record.begin();
        while(itor2 != login_record.end())
        {
            loginRec = *itor2;
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

                logout_record.erase(itor2);
                break;
            }
            else
            {
                if(login_record.empty())
                {
                    cout << "login_record is empty!" << endl;
                    return;
                }
            }
            itor2++;
        }
        itor1++;
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
    if(login_record.empty())
    {
        cout << "don't have unmatched login." << endl;
        return;
    }
    ofstream fout;
    LogRec loginRec;
    list<LogRec>::iterator itor;
    itor = login_record.begin();
    pid_t pid;
    short type;
    int time;

    fout.open(unmatched_login_file.c_str(), ios::ate|ios::out|ios::binary);     //everytime clear this file.
    if(fout.fail())
    {
            cout << "open file error!" << endl;
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
        fout.write((char *)&time, 2);
        fout.write(loginRec.log_ip, 32);
        itor++;
    }

    fout.close();

    login_record.clear();       //clear this list.
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
