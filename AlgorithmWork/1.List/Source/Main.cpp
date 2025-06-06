/**
 * 1.ADT 抽象数据类型
 */

#include <iostream>

// 单元测试 - 自定义向量
extern void UnitTest_JoyVector();
// 单元测试 - 自定义链表
extern void UnitTest_JoyList();

int main()
{
    UnitTest_JoyVector();
    UnitTest_JoyList();
	std::cin.get();
	return 0;
}
