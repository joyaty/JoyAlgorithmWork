#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>

static void RunOnThread()
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "运行在线程中的方法：" << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}

void TestSTLThreadFunc()
{
    std::thread myThread = std::thread(RunOnThread);
    myThread.join();
}
