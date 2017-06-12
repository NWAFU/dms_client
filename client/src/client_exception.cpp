#include "header/client_exception.h"

ClientException::ClientException()
{
    this -> msg = "客户机异常!";
}

ClientException::ClientException(const std::string& msg)
{
    this -> msg = "客户端异常————";
    this -> msg += msg;
    this -> msg += "!";
}

ClientException::~ClientException() throw(){}

const char* ClientException::whatMsg()
{
    return this -> msg.c_str();
}
