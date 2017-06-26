#include "header/socket_exception.h"

/**************************************************
*作者：wu heng
*日期：2017.06.10
*函数名：SocketException()
*功能：套接字异常类默认构造函数
*输入参数：none
*输出参数：none
*返回值：none
**************************************************/
SocketException::SocketException():ClientException("Socket Exception")
{
}

/**************************************************
*作者：wu heng
*日期：2017.06.10
*函数名：SocketException(const string& msg)
*功能：套接字异常类带参数构造函数
*输入参数：msg
*输出参数：none
*返回值：none
**************************************************/
SocketException::SocketException(const string &msg):ClientException(msg)
{
}

