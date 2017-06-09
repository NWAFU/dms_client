#ifndef LOG_READER_H
#define LOG_READER_H

#include <string>
#include <list>
#include <fstream>
#include "data.h"

using namespace std;

class  LogReader
{
private:
    string backup_file;
    string unmatched_login_file;
    list<LogRec> login_record;
    list<LogRec> logout_record;
    list<MatchedLogRec> matched_log_record;
private:
    LogReader();
    LogReader(string backup_file, string unmatched_login_file);
    ~LogReader();
    void backup();
    void readUnmatchedFile();
    void match();
    void saveUnmatchedLogin();
public:
    list<MatchedLogRec> & readLog();
    void readBackupFile();
};

#endif // LOG_READER_H
