#ifndef SOCKET_SENDER_H
#define SOCKET_SENDER_H

#include "header/log_sender.h"
#include "header/data.h"
#include <string>

using std::string;


class SocketSender:public LogSender
{
private:
    int socket_fd;
    string server_ip;
    short server_port;
    string unsended_file;
private:
    void connectServer();
    void readUnsendedFile(list<MatchedLogRec> & matched_log);
    void sendData(list<MatchedLogRec> & matched_log);
    void saveUnsendedFile(list<MatchedLogRec> & matched_log);
public:
    ~SocketSender();
    void sendLog(list<MatchedLogRec> & matched_log);
};

#endif // SOCKET_SENDER_H
