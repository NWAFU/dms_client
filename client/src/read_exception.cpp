#include "header/read_exception.h"

ReadException::ReadException()
{
    ClientException("读取时异常");
}

ReadException::ReadException(const string &msg):ClientException(msg)
{

}
