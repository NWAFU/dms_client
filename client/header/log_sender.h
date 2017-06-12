#ifndef LOG_SENDER_H
#define LOG_SENDER_H

#include <list>
#include <header/data.h>

using std::list;

class LogSender
{
private:
public:
    LogSender();
    ~LogSender();
    virtual void sendLog(list<MatchedLogRec> & matched_log)=0;
};

#endif // LOG_SENDER_H
