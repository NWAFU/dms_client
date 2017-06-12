#ifndef SOCKET_EXCEPTION_H
#define SOCKET_EXCEPTION_H

#include "client_exception.h"

using std::string;

//3.套接字异常
class SocketException:public ClientException
{
public:
    SocketException();
    SocketException(const string& msg);
};

#endif // SOCKET_EXCEPTION_H
