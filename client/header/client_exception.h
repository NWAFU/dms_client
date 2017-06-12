#ifndef CLIENT_EXCEPTION_H
#define CLIENT_EXCEPTION_H

#include <exception>
#include <string>

using std::string;
using std::exception;

//0.客户端异常
class ClientException:public exception
{
private:
    std::string msg;
public:
    ClientException();
    ClientException(const string & msg);
    ~ClientException() throw();
    const char* whatMsg();
};

#endif      //CLIENTEXCEPTION_H
