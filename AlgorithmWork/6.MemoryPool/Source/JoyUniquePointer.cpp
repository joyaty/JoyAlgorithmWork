#include "JoyUniquePointer.h"

#pragma region 单元测试-自定义唯一所有权智能指针

#include <iostream>
#include <memory>
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
    std::cout << "UnitTest_JoyUniquePointer" << std::endl;
}

#pragma endregion