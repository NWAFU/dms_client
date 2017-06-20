#include "header/backup_exception.h"

/**************************************************
*作者：wu heng
*函数名：BackupException()
*功能：备份异常类默认构造函数
*输入参数：none
*输出参数：none
*返回值：none
**************************************************/
BackupException::BackupException():ClientException("备份时异常")
{
}

/**************************************************
*作者：wu heng
*函数名：BackupException(const string& msg)
*功能：备份异常类带参数构造函数
*输入参数：msg
*输出参数：none
*返回值：none
**************************************************/
BackupException::BackupException(const string &msg):ClientException(msg)
{
}
