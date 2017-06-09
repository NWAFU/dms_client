#ifndef CLIENT_EXCEPTION_H
#define CLIENT_EXCEPTION_H

#include <exception>

class ClientException:public exception
{
private:
    string msg;
public:
    ClientException():msg("Client Exception!")
    {
        Exception();
    }
    ClientException(string const & msg):msg("Client Exception:")
    {
        Exception();
        this->msg+=msg;
        this->msg+="!";
    }
};

class BackupException:public ClientException
{
private:

};

#endif // CLIENT_EXCEPTION_H
