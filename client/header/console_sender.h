#ifndef CONSOLE_SENDER_H
#define CONSOLE_SENDER_H

#include "log_sender.h"

class ConsoleSender
{
private:
public:
    void sendLog(list<MatchedLogRec> & matched_log);
};

#endif // CONSOLE_SENDER_H
