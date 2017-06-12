#include "header/send_exception.h"

using std::string;

SendException::SendException()
{
    ClientException("发送时异常");
}

SendException::SendException(const string &msg):ClientException(msg)
{
}
