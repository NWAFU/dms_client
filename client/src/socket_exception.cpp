#include "header/socket_exception.h"

SocketException::SocketException()
{
    ClientException("套接字异常");
}

SocketException::SocketException(const string &msg):ClientException(msg)
{

}

