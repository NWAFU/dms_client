#ifndef CLIENT_H
#define CLIENT_H

class Client
{
private:
    LogReader& log_reader;
    Logsender& log_sender;
public:
    Client(LogReader& log_reader, LogSender &log_sender);
    void dataMine();
};

#endif // CLIENT_H
