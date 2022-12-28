/**
 * LeetCode - 1.两数之和
 * 给定一个整数数组 nums 和一个整数目标值 target，请你在该数组中找出 和为目标值 target  的那 两个 整数，并返回它们的数组下标。
 * 你可以假设每种输入只会对应一个答案。但是，数组中同一个元素在答案里不能重复出现。
 * 你可以按任意顺序返回答案
 */

#include <iostream>
#include <vector>
#include <unordered_map>

std::vector<int> TwoSum(const std::vector<int>& nums, int target);
void Output(const char* title, const std::vector<int>& retVector);

int main()
{
	// TestCase1 
	Output("TestCase1:", TwoSum({2, 7, 11, 15}, 9));
	// TestCase2
	Output("TestCase2:", TwoSum({ 3, 2, 4 }, 6));
	// TestCase3
	Output("TestCase3:", TwoSum({ 3, 3 }, 6));

	std::cin.get();
	return 0;
}

/// <summary>
/// 求解LeetCode - 1.两数之和
/// </summary>
/// <param name="nums"></param>
/// <param name="target"></param>
/// <returns></returns>
std::vector<int> TwoSum(const std::vector<int>& nums, int target)
{
	std::vector<int> retIndices{};
	// Key = 所需值，Value = 当前值索引，所需值 + 当前值 = target
	std::unordered_map<int, int> caches{};
	for (int i = 0; i < nums.size(); ++i)
	{
		// 获取当前值和所需值
		int currentValue = nums[i];
		int needValue = target - currentValue;
		// 检查当前值是否在所需值缓存中
		if (caches.find(currentValue) != caches.end())
		{
			retIndices.push_back(caches[currentValue]);
			retIndices.push_back(i);
			break;
		}
		caches.emplace(needValue, i);
	}
	return retIndices;
}

/// <summary>
/// 输出解
/// </summary>
/// <param name="title"></param>
/// <param name="retVector"></param>
void Output(const char* title, const std::vector<int>& retVector)
{
	std::cout << title << std::endl;
	for (int i = 0; i < retVector.size(); ++i)
	{
		std::cout << retVector[i] << std::endl;
	}
	std::cout << std::endl;
}