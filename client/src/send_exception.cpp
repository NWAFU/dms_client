#include "header/send_exception.h"

/**************************************************
*作者：wu heng
*日期：2017.06.10
*函数名：SendException()
*功能：发送异常类默认构造函数
*输入参数：none
*输出参数：none
*返回值：none
**************************************************/
SendException::SendException():ClientException("Send Exception")
{
}

/**************************************************
*作者：wu heng
*日期：2017.06.10
*函数名：SendException(const string& msg)
*功能：发送异常类带参数构造函数
*输入参数：msg
*输出参数：none
*返回值：none
**************************************************/
SendException::SendException(const string &msg):ClientException(msg)
{
}
