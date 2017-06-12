#ifndef BACKUP_EXCEPTION_H
#define BACKUP_EXCEPTION_H

#include "client_exception.h"

using std::string;

//1.备份异常
class BackupException:public ClientException
{
public:
    BackupException();
    BackupException(const string& msg);
};
#endif // BACKUP_EXCEPTION_H
