#include "header/client_exception.h"

/**************************************************
*作者：吴恒
*函数名：ClientException()
*功能：客户端异常类默认构造函数
*输入参数：none
*输出参数：none
*返回值：none
**************************************************/
ClientException::ClientException()
{
    this -> msg = "客户机异常!";
}

/**************************************************
*作者：吴恒
*函数名：ClientException(const string& msg)
*功能：客户端异常类带参数构造函数
*输入参数：const string& msg
*输出参数：none
*返回值：none
**************************************************/
ClientException::ClientException(const string& msg)
{
    this -> msg = "客户端异常————";
    this -> msg += msg;
    this -> msg += "!";
}

ClientException::~ClientException() throw(){}

/**************************************************
*作者：吴恒
*函数名：const char* what()
*功能：返回客户端异常类错误字符串msg
*输入参数：none
*输出参数：none
*返回值：const char*
**************************************************/
const char* ClientException::what()
{
    return this -> msg.c_str();
}
