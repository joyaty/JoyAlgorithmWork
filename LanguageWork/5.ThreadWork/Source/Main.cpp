
// 测试系统线程库
#include <iostream>

extern void TestThreadFunc();
// 测试STL线程库(依赖C++11以上)
extern void TestSTLThreadFunc();

int main()
{
    // TestThreadFunc();
    TestSTLThreadFunc();
}
