#include "header/send_exception.h"

SendException::SendException()
{
    ClientException("发送时异常");
}

SendException::SendException(const string &msg):ClientException(msg)
{

}
