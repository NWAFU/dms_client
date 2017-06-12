#include "header/log_reader.h"
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;
using std::ios;
using std::ifstream;

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

void LogReader::backup()
{
#ifdef _DEBUG
    cout << "backup start!......" << endl;
#endif
    //code
    char cmd[] = "./backup.sh wtmpx wtmpx.20170612094000";
    int status = system(cmd);
    int ret = WEXITSTATUS(status);
    if(ret == 1) cout<<"backup wrong!"<<endl;
    else if(ret == 2) cout<<"clear wrong!"<<endl;
    printf("%d\n", ret);

#ifdef _DEBUG
    cout << "backup end!......" << endl;
#endif
}

void LogReader::readBackupFile()
{
    ifstream fin(backup_file.c_str(),ifstream::in);
    if (fin.fail())
    {
        cout<<"read backup file failed!"<<endl;
        return;
    }
}

void LogReader::readUnmatchedFile()
{

}

void LogReader::match()
{

}

void LogReader::saveUnmatchedLogin()
{

}

list<MatchedLogRec>& LogReader::readLog()
{
    backup();
    readUnmatchedFile();
    readBackupFile();
    match();
    saveUnmatchedLogin();
    return matched_log_record;
}


