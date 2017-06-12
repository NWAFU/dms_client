#ifndef BACKUP_EXCEPTION_H
#define BACKUP_EXCEPTION_H

#include "client_exception.h"

//1.备份异常
class BackupException:public ClientException
{
public:
    BackupException();
    BackupException(const std::string& msg);
};
#endif // BACKUP_EXCEPTION_H
