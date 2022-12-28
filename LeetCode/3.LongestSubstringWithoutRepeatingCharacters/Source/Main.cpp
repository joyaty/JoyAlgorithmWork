/**
 * LeetCode - 3.无重复字符的最长子串
 * 给定一个字符串 s ，请你找出其中不含有重复字符的 最长子串 的长度。
 **/

#include <iostream>
#include <string>
#include <set>

int LengthOfLongestSubstring(std::string s);

int main()
{
	// TestCase1
	//std::cout << "TestCase1: " << LengthOfLongestSubstring("abcabcbb") << std::endl;
	//// TestCase2
	//std::cout << "TestCase2: " << LengthOfLongestSubstring("bbbbb") << std::endl;
	//// TestCase3
	//std::cout << "TestCase3: " << LengthOfLongestSubstring("pwwkew") << std::endl;
	// TestCase4
	std::cout << "TestCase3: " << LengthOfLongestSubstring("dvdf") << std::endl;

	std::cin.get();
	return 0;
}

/// <summary>
/// LeetCode - 3.无重复字符的最长子串
/// </summary>
/// <param name="s"></param>
/// <returns></returns>
int LengthOfLongestSubstring(std::string s)
{
	int maxLength = 0;
	return maxLength;
}