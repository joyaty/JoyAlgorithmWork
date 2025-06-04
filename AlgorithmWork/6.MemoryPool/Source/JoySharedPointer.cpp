#include "JoySharedPointer.h"

#pragma region 单元测试-自定义共享所有权智能指针

#include <iostream>
#include <memory>

namespace Joy
{
    namespace UnitTest_SharedPointer
    {
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
    }   // namespace UnitTest_SharedPointer
}   // namespace Joy

using Joy::JoySharedPointer;
using Joy::UnitTest_SharedPointer::TestCaseClass;


void UnitTest_JoySharedPointer()
{
    {
        // 标准库共享智能指针测试
        TestCaseClass*                 pNakedTestCase1 = new TestCaseClass(200, 100.0f);
        std::shared_ptr<TestCaseClass> pSharedTestCase1 =
            std::shared_ptr<TestCaseClass>(pNakedTestCase1);
        std::shared_ptr<TestCaseClass> pSharedTestCase2 =
            std::shared_ptr<TestCaseClass>(pSharedTestCase1);
        std::cout << "STD: pSharedTestCase1.refCount = " << pSharedTestCase1.use_count() << std::endl;
        std::cout << "STD: pSharedTestCase2.refCount = " << pSharedTestCase2.use_count() << std::endl;
    }
    {
        // 自定义共享智能指针测试
        TestCaseClass*                  pNakedTestCase1 = new TestCaseClass(500, 300.3f);
        JoySharedPointer<TestCaseClass> pSharedTestCase1 =
            JoySharedPointer<TestCaseClass>(pNakedTestCase1);
        JoySharedPointer<TestCaseClass> pSharedTestCase2 =
            JoySharedPointer<TestCaseClass>(pSharedTestCase1);

        JoySharedPointer<TestCaseClass> pSharedTestCase3 = std::move(pSharedTestCase2);

        std::cout << "Joy: pSharedTestCase1.refCount = " << pSharedTestCase1.GetRefCount() << std::endl;
        std::cout << "Joy: pSharedTestCase3.refCount = " << pSharedTestCase3.GetRefCount() << std::endl;
    }
}

#pragma endregion