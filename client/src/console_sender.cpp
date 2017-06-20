#include "header/console_sender.h"
#include <iostream>

using std::cout;
using std::endl;

void ConsoleSender::sendLog(list<MatchedLogRec> & matched_log)
{
    while (!matched_log.empty())
    {
        cout<<matched_log.front()<<endl;
        matched_log.pop_front();
    }
}
