
#include <algorithm>
#include <functional>
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

            bool CompareLowerFunc(const T& lhs, const T& rhs) { return lhs >= rhs; }

            bool CompareUpperFunc(const T& lhs, const T& rhs)
            {
                return !CompareLowerFunc(lhs, rhs);
            }
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
            return lhs >= rhs;
        }

        template<typename T> bool CompareUpperFunc(const T& lhs, const T& rhs)
        {
            return !CompareLowerFunc(lhs, rhs);
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

struct X
{
    void f(int value) { std::cout << value << std::endl; }
    int  a;
};


void UnitTest_Callback()
{
    void (X::*pmf)(int);   // 一个类成员函数指针变量pmf的定义
    pmf = &X::f;           // 类成员函数指针变量pmf被赋值

    X ins, *p;
    p = &ins;
    (ins.*pmf)(101);   // 对实例ins，调用成员函数指针变量pmf所指的函数
    (p->*pmf)(102);    // 对p所指的实例，调用成员函数指针变量pmf所指的函数

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
    bool ret = (compareObj.*classFuncPtr)(5, 10);
    // std::sort(std::begin(arr), std::end(arr), (compareObj.*classFuncPtr));
    // std::cout << "After func pointer with class func:" << std::endl;
    // std::for_each(std::begin(arr),
    //               std::end(arr),
    //               std::bind(&PrintInfo<int>, std::ref(std::cout), ' ', std::placeholders::_1));

    // std::cout << std::endl;
    std::cout << "====================================" << std::endl;

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