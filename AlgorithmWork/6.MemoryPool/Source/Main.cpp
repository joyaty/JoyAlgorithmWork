
#include <iostream>

// 单元测试 - 唯一所有权智能指针封装
extern void UnitTest_JoyUniquePointer();
// 单元测试 - 共享所有权智能指针封装
extern void UnitTest_JoySharedPointer();

int main()
{
    // // 单元测试 - 智能指针封装
    // UnitTest_JoyUniquePointer();
    // 单元测试 - 共享所有权智能指针封装
    UnitTest_JoySharedPointer();
    std::cout << "Hello MemoryPool" << std::endl;
    // 卡输入，避免程序直接结束
    std::cin.get();
}