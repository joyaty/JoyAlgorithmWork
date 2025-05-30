#include "JoyUniquePointer.h"

#pragma region 单元测试-自定义唯一所有权智能指针

#include <iostream>
using Joy::JoyUniquePointer;

void UnitTest_JoyUniquePointer()
{
    {
        JoyUniquePointer<int> pInt(new int(100));
        JoyUniquePointer<int> pInt2(std::move(pInt));
    }
    std::cout << "UnitTest_JoyUniquePointer" << std::endl;
}

#pragma endregion