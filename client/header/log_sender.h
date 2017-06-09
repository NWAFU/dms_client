#ifndef LOG_SENDER_H
#define LOG_SENDER_H

class LogSender
{
private:
public:
    LogSender();
    virtual void sendLog(list<MatchedLogRec> & matched_log);
};

#endif // LOG_SENDER_H
