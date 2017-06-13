#include "header/save_exception.h"

using std::string;

SaveException::SaveException():ClientException("保存时异常")
{
}

SaveException::SaveException(const string &msg):ClientException(msg)
{
}
