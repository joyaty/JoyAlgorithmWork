/**
 * 1.ADT 抽象数据类型
 */

#include <iostream>
#include "JoyADT/JoyVector.h"
#include <vector>

void TestJoyVector()
{
    
}

void TestSTDVector()
{
    std::vector<int> vecInts{};
    
    for(int i = 0; i < 5; ++i)
    {
        vecInts.push_back(i + 100);
    }
    
    std::vector<int>::iterator iter = vecInts.begin();
    ++iter;
}

int main()
{
	// 测试自己实现的抽象数据类型表，栈和队列
    std::cout<<"Hello JoyVector"<<std::endl;
    
    JoyVector<int> vecInts(10);
    vecInts.Push_Back(1);
    vecInts.Push_Back(2);
    vecInts.Pop_Back();
    vecInts.Push_Back(3);

    for(int i = 0; i < vecInts.Size(); ++i)
    {
        std::cout << i << ", " << vecInts[i] << std::endl;
    }
    
    JoyVector<int> vecInts2 = vecInts;
    for(int i = 0; i < vecInts2.Size(); ++i)
    {
        std::cout << i << ", " << vecInts2[i] << std::endl;
    }

    
	std::cin.get();
	return 0;
}
