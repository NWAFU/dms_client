#include <iostream>
#include "header/log_reader.h"
#include "header/read_exception.h"

using namespace std;

int main()
{
    LogReader logRead;
    logRead.backup();
    cout << "hello, world!" << endl;

    return 0;

}

