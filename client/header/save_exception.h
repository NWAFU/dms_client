#ifndef SAVE_EXCEPTION_H
#define SAVE_EXCEPTION_H

#include "client_exception.h"

using std::string;

//5.保存异常
class SaveException:public ClientException
{
public:
    SaveException();
    SaveException(const string& msg);
};

#endif // SAVE_EXCEPTION_H
