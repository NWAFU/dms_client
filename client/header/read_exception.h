#ifndef READ_EXCEPTION_H
#define READ_EXCEPTION_H

#include "client_exception.h"

using std::string;

//2.读取异常
class ReadException:public ClientException
{
public:
    ReadException();
    ReadException(const string& msg);
};

#endif // READ_EXCEPTION_H
