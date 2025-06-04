
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>

namespace Joy
{
    namespace UnitTest_Callback
    {
        // 仿函数实现可调用对象对比器
        template<typename T> class CustomComparator
        {
        public:
            bool operator()(const T& lhs, const T& rhs) { return lhs >= rhs; }
        };

        // 测试用例数据类
        class TestCaseStruct
        {
        public:
            explicit TestCaseStruct(int nValue)
                : m_nValue(nValue)
            {}

            ~TestCaseStruct() { std::cout << "~TestCaseStruct:" << m_nValue << std::endl; }

        public:
            int operator++() { return ++m_nValue; }

        public:
            friend std::ostream& operator<<(std::ostream& out, TestCaseStruct* pData);
            friend std::ostream& operator<<(std::ostream& out, const TestCaseStruct& data);

        private:
            int m_nValue;
        };

        std::ostream& operator<<(std::ostream& out, TestCaseStruct* pData)
        {
            out << ((pData == nullptr) ? 0 : pData->m_nValue);
            return out;
        }

        std::ostream& operator<<(std::ostream& out, const TestCaseStruct& data)
        {
            out << data.m_nValue;
            return out;
        }
    }   // namespace UnitTest_Callback
}   // namespace Joy

#pragma region 单元测试-回调的实现方式

using Joy::UnitTest_Callback::CustomComparator;
using Joy::UnitTest_Callback::TestCaseStruct;

void UnitTest_Callback()
{
    int arr[10] = {99, 5, 85, 25, 26, 31, 55, 3, 5, 3};
    std::sort(std::begin(arr), std::end(arr), CustomComparator<int>());
    std::cout << "After Functor:" << std::endl;
    for (const auto& item : arr)
    {
        std::cout << item << std::endl;
    }

    std::sort(std::begin(arr),
              std::end(arr),
              [](const int& lhs, const int& rhs) -> bool { return lhs < rhs; });
    std::cout << "After Lambda:" << std::endl;
    for (const auto& item : arr)
    {
        std::cout << item << std::endl;
    }
    // 测试shared_ptr + lambda,避免lamda执行时，指针指向的内存区域被释放
    std::cout << "shared_ptr + lambda" << std::endl;
    std::shared_ptr<TestCaseStruct> pIntValue = std::make_shared<TestCaseStruct>(100);
    {
        std::cout << "引用计数0:" << pIntValue.use_count() << ", " << pIntValue << std::endl;
        auto callback1 = [pIntValue]()
        {
            ++(*pIntValue);
            std::cout << "引用计数4:" << pIntValue.use_count() << ", " << pIntValue << std::endl;
        };
        std::cout << "引用计数1:" << pIntValue.use_count() << ", " << pIntValue << std::endl;
        pIntValue.reset();
        callback1();
        std::cout << "引用计数2:" << pIntValue.use_count() << ", " << pIntValue << std::endl;
    }
    std::cout << "引用计数3:" << pIntValue.use_count() << std::endl;
}

#pragma endregion