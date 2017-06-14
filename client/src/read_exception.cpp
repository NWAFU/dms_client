#include "header/read_exception.h"

/**************************************************
*作者：吴恒
*函数名：ReadException()
*功能：读取异常类默认构造函数
*输入参数：none
*输出参数：none
*返回值：none
**************************************************/
ReadException::ReadException():ClientException("读取时异常")
{
}

/**************************************************
*作者：吴恒
*函数名：ReadException(const string& msg)
*功能：读取异常类带参数构造函数
*输入参数：const string& msg
*输出参数：none
*返回值：none
**************************************************/
ReadException::ReadException(const string &msg):ClientException(msg)
{
}
