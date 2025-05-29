// 多态与续表 || 菱形继承与虚继承(基于虚基表实现) - 单元测试
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
            virtual void Show() const { std::cout << "This is Device class ." << std::endl; }
            virtual void Display() const
            {
                std::cout << "This is Device class from Display." << std::endl;
            }

        public:
            int m_SerialCode;
        };

        class Laptop : public Device
        {
        public:
            Laptop(int serialCode)
                : Device(serialCode)
                , m_Laptop(2)
            {}

        public:
            void Show() const override { std::cout << "This is Laptop class." << std::endl; }
             virtual void Display() const
            {
                 std::cout << "This is Device class from Display." << std::endl;
             }

        public:
            int m_Laptop;
        };

        class Tabletop : public Device
        {
        public:
            Tabletop(int serialCode)
                : Device(serialCode)
                , m_Tabletop(3)
            {}

        public:
            void Show() const override { std::cout << "This is Tabletop class." << std::endl; }
            //virtual void Display() const
            //{
            //    std::cout << "This is Device class from Display." << std::endl;
            //}

        public:
            int m_Tabletop;
        };

        class DeskTop : public Tabletop
        {
        public:
            DeskTop(int serialCode)
                : Tabletop(serialCode)
                , m_DeskTop(7)
            {}

        public:
            void Show() const override { std::cout << "This is DeskTop class." << std::endl; }
            virtual void Display() const
            {
                std::cout << "This is Device class from Display." << std::endl;
            }

        public:
            int m_DeskTop;
        };

        class Convertible : public Laptop, public Tabletop
        {
        public:
            Convertible(int serialCode)
                : Laptop(serialCode)
                , Tabletop(serialCode + 1)
                , m_Convertible(4)
            {}

        public:
            void Show() const override { std::cout << "This is Convertible class." << std::endl; }
            //virtual void Display() const
            //{
            //    std::cout << "This is Device class from Display." << std::endl;
            //}

        public:
            int m_Convertible;
        };

        class VirtualLaptop : virtual public Device
        {
        public:
            VirtualLaptop(int serialCode)
                : Device(serialCode)
                , m_Laptop(2)
            {}

        public:
            void Show() const override { std::cout << "This is VirtualLaptop class." << std::endl; }

        public:
            int m_Laptop;
        };

        class VirtualTabletop : virtual public Device
        {
        public:
            VirtualTabletop(int serialCode)
                : Device(serialCode)
                , m_Tabletop(3)
            {}

        public:
            void Show() const override
            {
                std::cout << "This is VirtualTabletop class." << std::endl;
            }

        public:
            int m_Tabletop;
        };

        class VirutalConvertible : public VirtualLaptop, public VirtualTabletop
        {
        public:
            VirutalConvertible(int serialCode)
                : Device(serialCode + 9)
                , VirtualLaptop(serialCode)
                , VirtualTabletop(serialCode + 1)
                , m_Convertible(4)
            {}

        public:
            void Show() const override
            {
                std::cout << "This is VirutalConvertible class." << std::endl;
            }

        public:
            int m_Convertible;
        };

    }   // namespace UnitTest
}   // namespace Joy

#pragma region VirtualInherit 单元测试

using namespace Joy::UnitTest;

void UniTest_VirutalInherit()
{
    std::unique_ptr<Convertible> pConvertible = std::make_unique<Convertible>(0);
    void*                        p0           = &pConvertible->Laptop::m_SerialCode;
    void*                        p1           = &pConvertible->Tabletop::m_SerialCode;
    void*                        p2           = &pConvertible->m_Laptop;
    void*                        p3           = &pConvertible->m_Tabletop;
    void*                        p4           = &pConvertible->m_Convertible;
    int                          size1        = sizeof(*pConvertible);

    std::unique_ptr<VirutalConvertible> pvConvertible = std::make_unique<VirutalConvertible>(0);
    void*                               pv1           = &pvConvertible->m_SerialCode;
    void*                               pv2           = &pvConvertible->m_Laptop;
    void*                               pv3           = &pvConvertible->m_Tabletop;
    void*                               pv4           = &(pvConvertible->m_Convertible);
    int                                 size2         = sizeof(*pvConvertible);


    std::unique_ptr<Laptop> pLaptop = std::make_unique<Laptop>(1);
    int                     size3   = sizeof(*pLaptop);

    std::unique_ptr<DeskTop> pDeskTop = std::make_unique<DeskTop>(2);
    int                      size4    = sizeof(*pDeskTop);

    std::cout << "UniTest_VirutalInherit" << std::endl;
}

#pragma endregion