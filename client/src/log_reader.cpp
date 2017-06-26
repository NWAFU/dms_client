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

//#define _DEBUG

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
    cout << "Starting to backup..." << endl;
    try
    {
        time_t t = time(0);
        char tmp[16] = {0};           //time
        char wtmpx[24] ="wtmpx.";
        strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", localtime(&t));
        backup_file = strcat(wtmpx, tmp);           //The size of the first argument must be greater than the size of the merged parameter.
#ifdef _DEBUG
        char cmd[64] = "../client/script/backup.sh wtmpx wtmpx.";
#else
        char cmd[64] = "./script/backup.sh wtmpx wtmpx.";
#endif
        strcat(cmd, tmp);
        int status = system(cmd);
        int ret = WEXITSTATUS(status);
        if(ret == 1)
        {
            throw BackupException("Backup failed");
            exit(1);
        }
        else if(ret == 2)
        {
            throw BackupException("Clean failed");
        }
        else if(ret == 0)
        {
            cout << "OK:backup finished." <<endl;
        }
    }
    catch (ClientException &e)
    {
        cout << e.what() << endl;
    }
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
    cout << "Starting to read data..." << endl;
    try
    {
        ifstream fin;
        int size;
        LogRec login_rec;
        pid_t pid;
        short type;
        int time;
        fin.open(unmatched_login_file.c_str(), ios::in|ios::binary);
        if(fin.fail())
        {
            throw ReadException("Read unmatched login file failed");
            exit(1);
        }
        else
        {
            fin.seekg(0, ios::end);
            size = fin.tellg();
        }
        //read unmatched file.
        for(int i=0;i<(size/74);i++)
        {
            fin.seekg(0, ios::beg);
            fin.read(login_rec.log_name, 32);

            fin.read((char *)&pid, 4);
            login_rec.pid = pid;

            fin.read((char *)&type, 2);
            login_rec.log_type = type;

            fin.read((char *)&time, 4);
            login_rec.log_time = time;

            fin.read(login_rec.log_ip, 32);

            login_record.push_back(login_rec);
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
        LogRec log_rec;
        short type;
        pid_t pid;
        int time;
        fin.open(backup_file.c_str(), ios::in|ios::binary);
        if (fin.fail())
        {
            throw ReadException("Read backup file failed");
            exit(1);
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
            fin.read(log_rec.log_name, 32);

            fin.seekg(36, ios::cur);
            fin.read((char *)&pid, 4);
            pid = ntohl(pid);
            log_rec.pid = pid;

            fin.read((char *)&type, 2);
            type = ntohs(type);
            log_rec.log_type = type;

            fin.seekg(6, ios::cur);
            fin.read((char *)&time, 4);
            time = ntohl(time);
            log_rec.log_time = time;

            fin.seekg(30, ios::cur);
            fin.read(log_rec.log_ip, 32);

            if(type ==7)
            {
                login_record.push_back(log_rec);
#ifdef _DEBUG
                cout << "login pushed" << endl;
#endif
            }
            else if(type == 8)
            {
                logout_record.push_back(log_rec);
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
        cout << "OK:read data in the file finished." << endl;
        cout << "Login number: " << login_record.size() << endl;
        cout << "Logout number: " << logout_record.size() << endl;

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
    cout << "Starting to data match..." << endl;
    try
    {
        LogRec logout_rec;
        LogRec login_rec;
        MatchedLogRec matched_log_rec;
        list<LogRec>::iterator iter1;       //logout iterator
        list<LogRec>::iterator iter2;       //login iterator
        iter1 = logout_record.begin();
        while(iter1 != logout_record.end())
        {
            logout_rec = *iter1;
            iter2 = login_record.begin();
            while(iter2 != login_record.end())
            {
                login_rec = *iter2;
                if(strcmp(login_rec.log_name, logout_rec.log_name) == 0 &&
                        login_rec.pid == logout_rec.pid &&
                        strcmp(login_rec.log_ip, logout_rec.log_ip) == 0)
                {
                    strcpy(matched_log_rec.log_name, logout_rec.log_name);
                    matched_log_rec.pid = logout_rec.pid;
                    matched_log_rec.login_time = login_rec.log_time;
                    matched_log_rec.logout_time = logout_rec.log_time;
                    matched_log_rec.duration = matched_log_rec.logout_time - matched_log_rec.login_time;
                    strcpy(matched_log_rec.log_ip, logout_rec.log_ip);

                    matched_log_record.push_back(matched_log_rec);

                    iter1=logout_record.erase(iter1);
                    iter2=login_record.erase(iter2);
                    iter1--;
                    break;
                }
                else
                {
                    if(login_record.empty())
                    {
                        throw ClientException("Login_record empty cause match failed");
                    }
                }
                iter2++;
            }
            if(iter2 == login_record.end())         //If there is no login log and logout match, leave the logout record.
            {
                iter1 = logout_record.erase(iter1);
                continue;
            }
            iter1++;
        }
        cout << "OK:data match finished." << endl;
        cout << "Matched record number: " << matched_log_record.size() << endl;
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
    cout << "Starting to save unmatched login file..." << endl;
    try
    {
        if(login_record.empty())
        {
            throw SaveException("Unmatched_login_rec empty cause save unmatched login file failed");
        }
        ofstream fout;
        LogRec login_rec;
        list<LogRec>::iterator itor;
        itor = login_record.begin();
        pid_t pid;
        short type;
        int time;

        fout.open(unmatched_login_file.c_str(), ios::trunc|ios::out|ios::binary);     //everytime clear this file.
        if(fout.fail())
        {
            throw SaveException("Create unmatched login file failed");
            exit(1);
        }
        while(itor != login_record.end())
        {
            login_rec = *itor;
            fout.write(login_rec.log_name, 32);
            pid = login_rec.pid;
            fout.write((char *)&pid, 4);
            type = login_rec.log_type;
            fout.write((char *)&type, 2);
            time = login_rec.log_time;
            fout.write((char *)&time, 4);
            fout.write(login_rec.log_ip, 32);
            itor++;
        }
        fout.close();
        login_record.clear();       //clear this list.
        cout << "OK:save unmatched login file finished." << endl;
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
