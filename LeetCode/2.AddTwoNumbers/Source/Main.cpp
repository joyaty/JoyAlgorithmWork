/**
 * LeetCode - 2.两数相加
 * 给你两个 非空 的链表，表示两个非负的整数。它们每位数字都是按照 逆序 的方式存储的，并且每个节点只能存储 一位 数字。
 * 请你将两个数相加，并以相同形式返回一个表示和的链表。
 * 你可以假设除了数字 0 之外，这两个数都不会以 0 开头。
 */

#include <iostream>
#include <vector>
#include <unordered_map>

/// <summary>
/// 单链表结构
/// </summary>
struct ListNode 
{
	int val;
	ListNode* next;

	ListNode()
		: val(0)
		, next(nullptr)
	{}

	ListNode(int x)
		: val(x)
		, next(nullptr)
	{}

	ListNode(int x, ListNode* next)
		: val(x)
		, next(next)
	{}
};

ListNode* AddTwoNumbers(ListNode* l1, ListNode* l2);
void Output(const char* title, const ListNode* ret);

int main()
{
	// TestCase1
	ListNode testCase1L1(2);
	ListNode testCase1L2(4);
	ListNode testCase1L3(3);
	testCase1L1.next = &testCase1L2;
	testCase1L2.next = &testCase1L3;
	ListNode testCase1R1(5);
	ListNode testCase1R2(6);
	ListNode testCase1R3(4);
	testCase1R1.next = &testCase1R2;
	testCase1R2.next = &testCase1R3;
	Output("TestCase1:", AddTwoNumbers(&testCase1L1, &testCase1R1));

	// TestCase2
	ListNode testCase2L1(0);
	ListNode testCase2R1(0);
	Output("TestCase2:", AddTwoNumbers(&testCase2L1, &testCase2R1));

	// TestCase3
	ListNode testCase3L1(9);
	ListNode testCase3L2(9);
	ListNode testCase3L3(9);
	ListNode testCase3L4(9);
	ListNode testCase3L5(9);
	ListNode testCase3L6(9);
	ListNode testCase3L7(9);
	testCase3L1.next = &testCase3L2;
	testCase3L2.next = &testCase3L3;
	testCase3L3.next = &testCase3L4;
	testCase3L4.next = &testCase3L5;
	testCase3L5.next = &testCase3L6;
	testCase3L6.next = &testCase3L7;
	ListNode testCase3R1(9);
	ListNode testCase3R2(9);
	ListNode testCase3R3(9);
	ListNode testCase3R4(9);
	testCase3R1.next = &testCase3R2;
	testCase3R2.next = &testCase3R3;
	testCase3R3.next = &testCase3R4;
	Output("TestCase3:", AddTwoNumbers(&testCase3L1, &testCase3R1));

	std::cin.get();
	return 0;
}

/// <summary>
/// 求解LeetCode - 2.两数相加
/// </summary>
/// <param name="l1"></param>
/// <param name="l2"></param>
/// <returns></returns>
ListNode* AddTwoNumbers(ListNode* l1, ListNode* l2)
{
	if (l1 == nullptr)
	{
		return l2;
	}
	if (l2 == nullptr)
	{
		return l1;
	}
	// 进位
	int carry = 0;
	ListNode* pRetHeader = nullptr;
	ListNode* pRetCurrent = pRetHeader;
	while (l1 != nullptr || l2 != nullptr)
	{
		int lVal = l1 != nullptr ? l1->val : 0;
		int rVal = l2 != nullptr ? l2->val : 0;
		int retValue = lVal + rVal + carry;
		if (retValue >= 10)
		{
			carry = retValue / 10;
			retValue = retValue % 10;
		}
		else
		{
			carry = 0;
		}
		ListNode* retNode = new ListNode(retValue);
		if (pRetHeader == nullptr)
		{
			pRetHeader = retNode;
			pRetCurrent = retNode;
		}
		else
		{
			pRetCurrent->next = retNode;
			pRetCurrent = retNode;
		}
		l1 = l1 == nullptr ? nullptr : l1->next;
		l2 = l2 == nullptr ? nullptr : l2->next;
	}
	if (carry > 0)
	{
		pRetCurrent->next = new ListNode(carry);
	}
	return pRetHeader;

}

/// <summary>
/// 输出解
/// </summary>
/// <param name="title"></param>
/// <param name="ret"></param>
void Output(const char* title, const ListNode* ret)
{
	std::cout << title << std::endl;
	while (ret != nullptr)
	{
		std::cout << ret->val << std::endl;
		ret = ret->next;
	}
	std::cout << std::endl;
}