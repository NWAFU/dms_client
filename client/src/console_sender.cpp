#include "header/console_sender.h"
#include <iostream>

using std::cout;

void ConsoleSender::sendLog(list<MatchedLogRec> & matched_log)
{
    while (!matched_log.empty())
    {
        cout<<matched_log.front();
        matched_log.pop_front();
    }
}
