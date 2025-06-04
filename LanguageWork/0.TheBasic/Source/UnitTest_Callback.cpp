
#include <algorithm>
#include <iterator>
namespace Joy
{
    namespace UnitTest_Callback
    {
        template<typename T> class CustomComparator
        {
        public:
            bool operator()(const T& lhs, const T& rhs) { return lhs >= rhs; }
        };
    }   // namespace UnitTest_Callback
}   // namespace Joy

#pragma region 单元测试-回调的实现方式

#include <iostream>

using Joy::UnitTest_Callback::CustomComparator;

void UnitTest_Callback()
{
    int arr[10] = {99, 5, 85, 25, 26, 31, 55, 3, 5, 3};
    std::sort(std::begin(arr), std::end(arr), CustomComparator<int>());
    for (const auto& item : arr)
    {
        std::cout << item << std::endl;
    }
}

#pragma endregion