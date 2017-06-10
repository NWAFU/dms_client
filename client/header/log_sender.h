#ifndef LOG_SENDER_H
#define LOG_SENDER_H

class LogSender
{
private:
public:
    LogSender();
    ~LogSender();
    virtual void sendLog(list<MatchedLogRec> & matched_log)=0;
};

#endif // LOG_SENDER_H
