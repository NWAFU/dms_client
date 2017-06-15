#include "header/socket_sender.h"

using namespace std;

int main()
{
    list<MatchedLogRec> matched_log;
    SocketSender sock_sender;
    sock_sender.sendLog(matched_log);

    return 0;
}
