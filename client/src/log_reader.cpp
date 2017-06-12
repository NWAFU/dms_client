#include "/header/log_reader.h"

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

LogReader::backup()
{
#ifdef _DEBUG
    cout << "backup start!......" << endl;
#endif
    //code
    char cmd[] = "./backup.sh wtmpx wtmpx.20170612094000";
    int status = system(cmd);
    int ret = WEXITATATUS(status);
    if(ret == 1) throw client_exception("backup wrong!\n");
    else if(ret == 2) throw client_exception("clear wrong!\n");
    printf("%d\n", ret);

#ifdef _DEBUG
    cout << "backup end!......" << endl;
#endif
}

LogReader::readBackupFile()
{
    ifstream fin(backup_file,ios::nocreate|ios::binary);
    if (fin.fail())
    {
        cout<<"read backup file failed!"<<endl;
        return -1;
    }

}

LogReader::readLogs()
{
    backup();
    readUnmatchedFile();
    readBackupFile();
    match();
    saveUnmatchedLogin();
}


