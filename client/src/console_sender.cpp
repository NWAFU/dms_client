#include "header/console_sender.h"
#include <iostream>

using std::cout;
using std::endl;

void ConsoleSender::sendLog(list<MatchedLogRec> & matched_log)
{
    for (list<MatchedLogRec>::iterator it=matched_log.begin();it!=matched_log.end();it++)
    {
        cout<<*it<<endl;
    }
}
