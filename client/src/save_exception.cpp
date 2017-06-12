#include "header/save_exception.h"

SaveException::SaveException()
{
    ClientException("保存时异常");
}

SaveException::SaveException(const std::string &msg)
{
    ClientException(msg);
}
