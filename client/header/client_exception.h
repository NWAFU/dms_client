#ifndef CLIENTEXCEPTION_H
#define CLIENTEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

//0.客户端异常
class ClientException:public exception
{
private:
	string msg;
public:
	ClientException()
	{
		this -> msg = "客户机异常!"
	}
	ClientException(const string& msg)
	{
		this -> msg = "客户端异常————";
		this -> msg += msg;
		this -> msg += "!";
	}
	~ClientException() throw(){}
	const char* whatMsg(){
		return this -> msg.c_str();
	}
};

//1.备份异常
class BackupException:public ClientException
{
public:
        BackupException():ClientException("备份时异常！"){}
	BackupException(const string& msg):ClientException(msg){}
};

//2.读取异常
class ReadException:
	public ClientException
{
public:
        ReadException():ClientException("读取时异常！"){}
	ReadException(const string& msg):ClientException(msg){}
};

//3.套接字异常
class SocketException:
	public ClientException
{
public:
        SocketException():ClientException("套接字异常！"){}
	SocketException(const string& msg):ClientException(msg){}
};

//4.发送异常
class SendException:
	public ClientException
{
public:
        SendException():ClientException("发送时异常！"){}
	SendException(const string& msg):ClientException(msg){}
};

//5.保存异常
class SaveException:
	public ClientException
{
public:
        SaveException():ClientException("保存时异常！"){}
	SaveException(const string& msg):ClientException(msg){}
};

#endif      //CLIENTEXCEPTION_H
