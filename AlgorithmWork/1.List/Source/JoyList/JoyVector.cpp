/**
 * 自定义ADT - 向量
 * 实现STL中vector
 */
#include "JoyVector.h"
#include <algorithm>

#pragma region 单元测试 - 自定义 Vector 数据结构，向量

#include <iostream>
#include <vector>

namespace Joy
{
    namespace UnitTest_JoyVector
    {
        // 打印列表辅助输出仿函数
        template<typename T>
        class PrintVector
        {
        public:
            void operator()(const T& value) const { std::cout << value << " "; }
        };
    }   // namespace UnitTest_JoyVector
}   // namespace Joy

using Joy::JoyVector;
using Joy::UnitTest_JoyVector::PrintVector;

void UnitTest_JoyVector()
{
    PrintVector<int> printer{};
    // 单元测试 - JoyVector
    std::cout << "Hello JoyVector ============================" << std::endl;
    {
        JoyVector<int> vecInts{};
        std::cout << "capacity = " << vecInts.Capacity() << ", size = " << vecInts.Size() << std::endl;
        for (int i = 0; i < 20; ++i)
        {
            vecInts.Push_Back(i + 100);
            std::cout << "capacity = " << vecInts.Capacity() << ", size = " << vecInts.Size() << std::endl;
        }
        std::cout << "After Init Vector:";
        std::for_each(vecInts.Begin(), vecInts.End(), printer);
        std::cout << std::endl;

        vecInts.Pop_Back();
        vecInts.Resize(10);
        std::cout << "Front = " << vecInts.Front() << ", Iter Begin = " << *vecInts.Begin() << std::endl;
        std::cout << "Back = " << vecInts.Back() << ", Iter End = " << *vecInts.End() << std::endl;

        std::cout << "Before Destory Vector:";
        std::for_each(vecInts.Begin(), vecInts.End(), printer);
        std::cout << std::endl;
    }
    // 单元测试 - std::vector
    std::cout << std::endl;
    std::cout << "Hello std::vector ============================" << std::endl;
    {
        std::vector<int> vecInts{};
        for (int i = 0; i < 20; ++i)
        {
            vecInts.push_back(i + 100);
            std::cout << "capacity = " << vecInts.capacity() << ", size = " << vecInts.size() << std::endl;
        }
        std::cout << "After Init Vector:";
        std::for_each(vecInts.cbegin(), vecInts.cend(), printer);
        std::cout << std::endl;

        vecInts.pop_back();
        vecInts.resize(10);
        std::cout << "Front = " << vecInts.front() << ", Iter Begin = " << *vecInts.cbegin() << std::endl;
        std::cout << "Back = " << vecInts.back() << ", Iter End = " << *vecInts.cend() << std::endl;

        auto iterInsert = vecInts.insert(vecInts.begin() + 4, 200);
        std::cout << "insert = " << *iterInsert << std::endl;
        auto iterErase = vecInts.erase(vecInts.begin() + 7);
        std::cout << "erase = " << *iterErase << std::endl;

        std::cout << "Before Destory Vector:";
        std::for_each(vecInts.cbegin(), vecInts.cend(), printer);
        std::cout << std::endl;
    }
}



#pragma endregion