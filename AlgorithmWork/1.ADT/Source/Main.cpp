/**
 * 1.ADT 抽象数据类型
 */

#include <iostream>
#include "JoyADT/JoyVector.h"
#include "JoyADT/JoyList.hpp"
#include <vector>
#include <list>

void TestJoyVector()
{
    // 测试自己实现的抽象数据类型表，栈和队列
    std::cout<<"Hello JoyVector"<<std::endl;
    
    JoyVector<int> vecInts(10);
    for (int i = 0; i < 20; ++i)
    {
        vecInts.Push_Back(i + 100);
    }

    for(int i = 0; i < vecInts.Size(); ++i)
    {
        std::cout << i << ", " << vecInts[i] << std::endl;
    }
    
    JoyVector<int> vecInts2 = vecInts;
    for(int i = 0; i < vecInts2.Size(); ++i)
    {
        std::cout << i << ", " << vecInts2[i] << std::endl;
    }
}

void TestSTDVector()
{
    std::vector<int> vecInts{};
    
    for(int i = 0; i < 5; ++i)
    {
        vecInts.push_back(i + 100);
    }
}

void TestSTDList()
{
    std::list<int> lstInts{};
}

int main()
{
    TestJoyVector();
	std::cin.get();
	return 0;
}
