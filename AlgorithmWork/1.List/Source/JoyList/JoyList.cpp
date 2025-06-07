/**
 * JoyList.cpp
 * 自定义List数据结构，双向链表
 */


#pragma region 单元测试 - 自定义 List 数据结构，双向链表

#include "JoyList.h"
#include <algorithm>
#include <iostream>
#include <list>

namespace Joy
{
    namespace UnitTest_JoyList
    {
        // 打印列表辅助输出仿函数
        template<typename T>
        class PrintList
        {
        public:
            void operator()(const T& value) const { std::cout << value << " "; }
        };

    }   // namespace UnitTest_JoyList
}   // namespace Joy

using Joy::JoyList;
using Joy::UnitTest_JoyList::PrintList;

void UnitTest_JoyList()
{
    std::cout << "Hello JoyList ============================" << std::endl;
    PrintList<int> printListHelper{};
    {
        JoyList<int> myList{};
    }
    {
        std::list<int> myList{};
        myList.push_back(10);
        myList.push_back(5);
        myList.push_back(7);

        std::for_each(myList.begin(), myList.end(), printListHelper);
    }
}

#pragma endregion
