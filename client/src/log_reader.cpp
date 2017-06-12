#include "header/log_reader.h"

LogReader::LogReader()
{

}

LogReader::LogReader(string backup_file, string unmatched_login_file)
{
    this->backup_file=backup_file;
    this->unmatched_login_file=unmatched_login_file;
}

LogReader::~LogReader()
{

}

void LogReader::readBackupFile()
{

}
