#ifndef SEND_EXCEPTION_H
#define SEND_EXCEPTION_H

#include "client_exception.h"

using std::string;

//4.发送异常
class SendException:public ClientException
{
public:
    SendException();
    SendException(const string& msg);
};

#endif
