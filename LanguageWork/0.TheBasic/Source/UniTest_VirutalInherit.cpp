// 菱形继承与虚继承 - 单元测试
#include <iostream>
#include <memory>

namespace Joy
{
    namespace UnitTest
    {
        class Device
        {
        public:
            Device(int serialCode)
                : m_SerialCode(serialCode)
            {}

        public:
            int m_SerialCode;
        };

        class Laptop : public Device
        {
        public:
            Laptop(int serialCode)
                : Device(serialCode)
            {}
        };

        class Tabletop : public Device
        {
        public:
            Tabletop(int serialCode)
                : Device(serialCode)
            {}
        };

        class Convertible : public Laptop, public Tabletop
        {
        public:
            Convertible(int serialCode)
                : Laptop(serialCode)
                , Tabletop(serialCode + 1)
            {}

        public:
        };

    }   // namespace UnitTest
}   // namespace Joy

#pragma region VirtualInherit 单元测试

using namespace Joy::UnitTest;

void UniTest_VirutalInherit()
{
    std::unique_ptr<Convertible> pConvertible = std::make_unique<Convertible>(100);

    std::cout << "Tabletop::m_SerialCode: " << pConvertible->Tabletop::m_SerialCode << std::endl;
    std::cout << "Laptop::m_SerialCode: " << pConvertible->Laptop::m_SerialCode << std::endl;
    std::cout << "UniTest_VirutalInherit" << std::endl;
}

#pragma endregion