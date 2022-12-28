/**
 * LeetCode - 4.寻找两个正序数组的中位数
 * 给定两个大小分别为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。请你找出并返回这两个正序数组的 中位数 。
 * 算法的时间复杂度应该为 O(log (m+n)) 。
 **/

#include<iostream>
#include<vector>

double FindMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2);

int main()
{
	// TestCase1
	std::vector<int> veclTestCase1{ 1, 3 };
	std::vector<int> vecrTestCase1{ 2 };
	std::cout << "TestCase2:" << FindMedianSortedArrays(veclTestCase1, vecrTestCase1) << std::endl;
	// TestCase2
	std::vector<int> veclTestCase2{ 1,2 };
	std::vector<int> vecrTestCase2{ 3,4 };
	std::cout << "TestCase2:" << FindMedianSortedArrays(veclTestCase2, vecrTestCase2) << std::endl;

	std::cin.get();
	return 0;
}

/// <summary>
/// LeetCode - 4.寻找两个正序数组的中位数
/// </summary>
/// <param name="nums1"></param>
/// <param name="nums2"></param>
/// <returns></returns>
double FindMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) 
{
	double median = 0;

	return median;
}