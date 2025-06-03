#include "JoyUniquePointer.h"

#pragma region 单元测试-自定义唯一所有权智能指针

#include <iostream>
#include <memory>
#include <cstdlib>

using Joy::JoyUniquePointer;

class TestCaseClass
{
public:
    TestCaseClass()
        : m_fValue(3.3f)
        , m_nValue(7)
    {}

    TestCaseClass(int nValue, float fValue)
        : m_nValue(nValue)
        , m_fValue(fValue)
    {}

    ~TestCaseClass()
    {
        std::cout << "Release object: " << m_nValue << ", " << m_fValue << std::endl;
    }

public:
    int   GetN() const { return m_nValue; }
    float GetF() const { return m_fValue; }

public:
    friend std::ostream& operator<<(std::ostream& out, const TestCaseClass& instance);

private:
    int   m_nValue{};
    float m_fValue{};
};

std::ostream& operator<<(std::ostream& out, const TestCaseClass& instance)
{
    out << "Address:" << &instance << ", m_nValue = " << instance.m_nValue
        << ", m_fValue = " << instance.m_fValue;
    return out;
}

class TestCaseClass2
{
public:
    TestCaseClass2()
        : b(100)
        , c(1000)
        , d(10000)
    {}

private:
    int  a[1000];
    int  b;
    int  c;
    bool d;
};

void UnitTest_JoyUniquePointer()
{
    {
        JoyUniquePointer<TestCaseClass> pTestCase1 =
            JoyUniquePointer<TestCaseClass>(new TestCaseClass());
        JoyUniquePointer<TestCaseClass> pTestCase2 =
            JoyUniquePointer<TestCaseClass>(std::move(pTestCase1));

        std::unique_ptr<TestCaseClass> pTestCase3 = std::make_unique<TestCaseClass>(100, 56.7f);
        *pTestCase3                               = *pTestCase2;

        JoyUniquePointer<TestCaseClass> pTestCase4 =
            JoyUniquePointer<TestCaseClass>(new TestCaseClass(200, 10.2f));
        *pTestCase4 = *pTestCase2;

        std::cout << *pTestCase2 << std::endl;
        std::cout << *pTestCase3 << std::endl;
        std::cout << *pTestCase4 << std::endl;
    }

    {
        // Dangerous code.
        long long*           pTestMalloc = (long long*)malloc(1);
        *pTestMalloc = 88888ll;
        TestCaseClass2* pTestCase2  = new (pTestMalloc) TestCaseClass2();
        std::cout << "sizeof(*pTestCase2) = " << sizeof(*pTestCase2) << std::endl;
        void* pTestRelloc = realloc(pTestMalloc, 8);
        std::cout << "Malloc = " << pTestMalloc << ", Realloc = " << pTestRelloc << ", pTestCase2 = " << pTestCase2 << std::endl;
        TestCaseClass2* pTestCase3 = new (pTestRelloc) TestCaseClass2();
        free(pTestRelloc);

        void*           pTestMalloc2 = malloc(2);
        int* pInt = (int*)pTestMalloc2;
        int* pInt2 = (int*)pTestMalloc2 + 1;
        int* pInt3 = (int*)pTestMalloc2 + 6;
        free(pTestMalloc2);
    }
    std::cout << "UnitTest_JoyUniquePointer" << std::endl;
}

#pragma endregion