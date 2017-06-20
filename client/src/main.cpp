#include "header/client.h"
#include "header/socket_sender.h"

using namespace std;

int main()
{
    LogReader log_reader;
    SocketSender sock_sender;
    Client client(log_reader,sock_sender);
    client.dataMine();
    return 0;
}
