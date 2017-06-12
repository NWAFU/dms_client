#ifndef CLIENT_H
#define CLIENT_H

#include "header/log_reader.h"
#include "header/log_sender.h"
class Client
{
private:
    LogReader& log_reader;
    LogSender& log_sender;
public:
    Client(LogReader& log_reader, LogSender & log_sender);
    void dataMine();
};

#endif // CLIENT_H
