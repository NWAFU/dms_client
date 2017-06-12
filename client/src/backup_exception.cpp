#include "header/backup_exception.h"

using namespace std;

BackupException::BackupException()
{
    ClientException("备份时异常");
}

BackupException::BackupException(const string &msg):ClientException(msg)
{

}
