#include "header/console_sender.h"
#include <iostream>

using std::cout;
using std::endl;

void ConsoleSender::sendLog(list<MatchedLogRec> & matched_log)
{
    cout<<"Starting to output matched log to terminal..."<<endl;

    int i=0;
    for (list<MatchedLogRec>::iterator it=matched_log.begin();it!=matched_log.end();it++)
    {
        cout<<*it<<endl;
        ++i;
    }

    cout<<"OK:output matched log to terminal finished."<<endl;
}
