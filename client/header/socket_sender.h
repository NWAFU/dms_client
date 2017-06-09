#ifndef SOCKET_SENDER_H
#define SOCKET_SENDER_H

#include "log_sender.h"

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
    void saveUnsendedFile(liste<MatchedLogRec> & matched_log);
public:
    void sendLog(list<MatchedLogRec> & matched_log);
};

#endif // SOCKET_SENDER_H
