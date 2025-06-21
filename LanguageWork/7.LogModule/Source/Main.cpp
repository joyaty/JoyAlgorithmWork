
#include "LogModule/LogUtil.h"
#include <iostream>

int main(int argc, char* args[])
{
    Joy::LogUtil::Instance().Init();

    LOG_INFO("HelloWorld");

    std::cin.get();
    return 0;
}