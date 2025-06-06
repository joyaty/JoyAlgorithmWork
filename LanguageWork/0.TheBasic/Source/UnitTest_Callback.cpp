
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <thread>

namespace Joy
{
    namespace UnitTest_Callback
    {
        // 仿函数实现可调用对象对比器
        template<typename T> class CustomComparator
        {
        public:
            bool operator()(const T& lhs, const T& rhs) { return lhs > rhs; }

            bool CompareLowerFunc(const T& lhs, const T& rhs) { return lhs > rhs; }

            bool CompareUpperFunc(const T& lhs, const T& rhs) { return lhs < rhs; }
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

        template<typename T> bool CompareLowerFunc(const T& lhs, const T& rhs)
        {
            return lhs > rhs;
        }

        template<typename T> bool CompareUpperFunc(const T& lhs, const T& rhs)
        {
            return lhs < rhs;
        }

        template<typename T>
        std::ostream& PrintInfo(std::ostream& os, char splitChar, const T& info)
        {
            os << info << splitChar;
            return os;
        }
    }   // namespace UnitTest_Callback
}   // namespace Joy

#pragma region 单元测试-回调的实现方式

using Joy::UnitTest_Callback::CustomComparator;
using Joy::UnitTest_Callback::TestCaseStruct;
using Joy::UnitTest_Callback::PrintInfo;
using Joy::UnitTest_Callback::CompareLowerFunc;
using Joy::UnitTest_Callback::CompareUpperFunc;

using CompareCallBackClassFunc = bool (CustomComparator<int>::*)(const int&, const int&);
using CompareCallBackFunc      = bool (*)(const int&, const int&);

void UnitTest_Callback()
{
    // 注意!
    // std中的比较器需要符合严格弱序规则(strict weak ordering)
    // 1. compare(a, a) = false; // 反自反性
    // 2. compare(a, b) = true, then compare(b, a) = false // 反对称性
    // 3. compare(a, b) = true && compare(b, c) = true, then compare(a, c) = true // 传递性
    int arr[10] = {99, 5, 85, 25, 26, 31, 55, 3, 5, 3};
    // 1. 使用仿函数对象作为回调
    std::sort(std::begin(arr), std::end(arr), CustomComparator<int>());
    std::cout << "After Functor:" << std::endl;
    for (const auto& item : arr)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    // 2. 使用Lambda表达式作为回调
    std::sort(std::begin(arr),
              std::end(arr),
              [](const int& lhs, const int& rhs) -> bool { return lhs < rhs; });
    std::cout << "After Lambda:" << std::endl;
    for (const auto& item : arr)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    // 3. 使用std::function可调用对象作为回调
    std::function<bool(int, int)> func = CompareLowerFunc<int>;
    std::sort(std::begin(arr), std::end(arr), func);
    std::cout << "After std::function:" << std::endl;
    for (const auto& item : arr)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    // 4. 使用std::bind绑定普通函数生成可调用对象作为回调
    auto bindFunc = std::bind(&CompareUpperFunc<int>, std::placeholders::_1, std::placeholders::_2);
    std::sort(std::begin(arr), std::end(arr), bindFunc);
    std::cout << "After std::bind func:" << std::endl;
    for (const auto& item : arr)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    // 5. 使用std::bind绑定类成员函数生成可调用对象作为回调
    CustomComparator<int> compareObj{};
    auto                  bindFunc2 = std::bind(&CustomComparator<int>::CompareLowerFunc,
                               &compareObj,
                               std::placeholders::_1,
                               std::placeholders::_2);
    std::sort(std::begin(arr), std::end(arr), bindFunc2);
    std::cout << "After std::bind with class::func:" << std::endl;
    // 使用引用的方式传递std::bind参数
    std::for_each(std::begin(arr),
                  std::end(arr),
                  std::bind(&PrintInfo<int>, std::ref(std::cout), ' ', std::placeholders::_1));
    std::cout << std::endl;

    // 6. 使用函数指针绑定普通函数作为回调
    CompareCallBackFunc funcPtr = &CompareUpperFunc<int>;
    std::sort(std::begin(arr), std::end(arr), funcPtr);
    std::cout << "After func pointer with common func:" << std::endl;
    std::for_each(std::begin(arr),
                  std::end(arr),
                  std::bind(&PrintInfo<int>, std::ref(std::cout), ' ', std::placeholders::_1));
    std::cout << std::endl;
    // 7. 使用函数指针绑定类成员函数作为回调
    CompareCallBackClassFunc classFuncPtr = &CustomComparator<int>::CompareLowerFunc;
    bool                     ret          = (compareObj.*classFuncPtr)(5, 10);
    // std::sort(std::begin(arr), std::end(arr), (compareObj.*classFuncPtr));
    // std::cout << "After func pointer with class func:" << std::endl;
    // std::for_each(std::begin(arr),
    //               std::end(arr),
    //               std::bind(&PrintInfo<int>, std::ref(std::cout), ' ', std::placeholders::_1));

    // std::cout << std::endl;
    std::cout << "====================================" << std::endl;

    // 测试shared_ptr +
    // lambda,避免lamda执行时，指针指向的内存区域被释放，lambda捕获的shared_ptr会拷贝一次，增加一次引用计数
    std::cout << "shared_ptr + lambda" << std::endl;
    std::thread testThread{};
    {
        std::shared_ptr<TestCaseStruct> pTestCaseStruct = std::make_shared<TestCaseStruct>(10);
        std::cout << "outter pTestCaseStruct Address = " << &pTestCaseStruct << std::endl;
        std::cout << "before create Lambda, ref_count = " << pTestCaseStruct.use_count()
                  << std::endl;
        auto callbackLambda = [pTestCaseStruct]() -> void
        {
            std::cout << "inner pTestCaseStruct Address = " << &pTestCaseStruct << std::endl;
            std::cout << "before apply Lambda, ref count = " << pTestCaseStruct.use_count()
                      << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));   // 线程休眠5秒
            ++(*pTestCaseStruct);
            std::cout << "after apply Lambda, ref_count = " << pTestCaseStruct.use_count()
                      << std::endl;
            std::cout << "after apply Lambda, value = " << pTestCaseStruct << std::endl;
        };
        std::cout << "before call Lambda, ref_count = " << pTestCaseStruct.use_count() << std::endl;
        callbackLambda();
        std::cout << "after call Lambda, ref_count = " << pTestCaseStruct.use_count()
                  << std::endl;   // ref_count = 2，Lambda按值捕获智能指针，智能指针拷贝，引用计数+1
        std::cout << "after call Lambda, value = " << pTestCaseStruct
                  << std::endl;   // value = 11,
                                  // lambda修改了指针指向内存的值，外部指针执行相同地址，一样被修改
        testThread = std::thread(callbackLambda);
        std::cout << "after create Thread, ref_count = " << pTestCaseStruct.use_count()
                  << std::endl;   // ref_cout = 3
                                  // 线程捕获Lambda，其中Lambda捕获的智能指针被拷贝一份，引用计数+1
    }
    testThread.join();   // 附加到主线程，主线程阻塞，等待线程执行完毕。
}

#pragma endregion