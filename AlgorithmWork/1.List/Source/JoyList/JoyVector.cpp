/**
 * 自定义ADT - 向量
 * 实现STL中vector
 */

#include "JoyVector.h"

#pragma region 单元测试，自定义 Vector 数据结构，向量

#include <iostream>
#include <vector>

using Joy::JoyVector;

void UnitTest_JoyVector()
{
    // 测试自己实现的抽象数据类型表，栈和队列
    std::cout << "Hello JoyVector ============================" << std::endl;
    // JoyVector
    {
        JoyVector<int> vecInts(10);
        for (int i = 0; i < 20; ++i)
        {
            vecInts.Push_Back(i + 100);
        }

        for (int i = 0; i < vecInts.Size(); ++i)
        {
            std::cout << i << ", " << vecInts[i] << std::endl;
        }

        JoyVector<int> vecInts2 = vecInts;
        for (int i = 0; i < vecInts2.Size(); ++i)
        {
            std::cout << i << ", " << vecInts2[i] << std::endl;
        }
    }
    // std::vector
    {
        std::vector<int> vecInts{};

        for (int i = 0; i < 5; ++i)
        {
            vecInts.push_back(i + 100);
        }
    }
}

#pragma endregion