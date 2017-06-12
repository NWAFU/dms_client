#include "header/backup_exception.h"

BackupException::BackupException()
{
    ClientException("备份时异常");
}

BackupException::BackupException(const std::string &msg)
{
    ClientException(msg);
}
