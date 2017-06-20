#ifndef LOG_READER_H
#define LOG_READER_H

#include <string>
#include <list>
#include <fstream>
#include <sys/stat.h>
#include "data.h"

using namespace std;

class  LogReader
{
private:
    string log_file;
    string backup_file;                     //由登录日志管理器生产的文件备份生成
    string unmatched_login_file;            //上一次未匹配成功的记录存入该文件
    list<LogRec> login_record;              //用来保存登入记录的集合
    list<LogRec> logout_record;             //用来保存登出记录的集合
    list<MatchedLogRec> matched_log_record; //用来保存匹配好的日志记录

    void backup();                          //备份日志记录文件：把原始日志记录文件备份成为以时间为后缀的文件
    void readBackupFile();                  //读取备份的日志文件，对读取到的数据进行提取，生成登入或登出日志记录，并把该记录存入到相应的集合中
    void readUnmatchedFile();               //读取上一次没有匹配的登入记录
    void match();                           //将用户登入/登出记录匹配为完整的登录记录存入matches
    void saveUnmatchedLogin();              //将匹配失败的日志记录保存在文件中
public:
    LogReader();
    LogReader(string log_file, string unmatched_login_file);
    ~LogReader();
    list<MatchedLogRec> & readLog();
};

#endif // LOG_READER_H
