#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>


class Person
{
    friend class FriendPerson;

public:
    Person() { MemFunc(); }
    Person(int id, std::string name)
        : m_ID(id)
        , m_Name(name)
    {}
    virtual ~Person() = default;

public:
    virtual void GetDesc() const
    {
        std::cout << "Id = " << m_ID << ", Name = " << m_Name << std::endl;
    }

    const std::string& GetName() const { return m_Name; }
    int                GetID() const { return m_ID; }

    virtual void MemFunc() { std::cout << "Call Person::MemFunc()" << std::endl; }

protected:
    std::string m_Name;

private:
    int m_ID;
};

class Student : public Person
{
protected:
    using Person::m_Name;
    using Person::GetID;

public:
    Student()
        : Person()
        , m_StudentID(0)
    {
        MemFunc();
    }

    Student(int id, int studentID, std::string name)
        : Person(id, name)
        , m_StudentID(studentID)
    {}

    void MemFunc() override { std::cout << "Call Student::MemFunc()" << std::endl; }
    void MemFunc(int param) { std::cout << "Call Student::MemFunc(int param)" << std::endl; }

public:
    void GetDesc() const override
    {
        std::cout << "Id = " << GetID() << ", Name = " << m_Name << ", StudentID = " << m_StudentID
                  << std::endl;
    }

protected:
    int m_StudentID;
};

class Senior : public Student
{
public:
    void GetDesc() const override
    {
        std::cout << "Id = " << GetID() << ", Name = " << m_Name << ", StudentID = " << m_StudentID
                  << std::endl;
    }
};

class FriendPerson
{
public:
    void PrintID(const Person& person) const { std::cout << "Id = " << person.m_ID << std::endl; }

    void PrintStudentID(const Student& student) const
    {
        // 友元关系不可继承，由继承的各个部分各自负责
        // FriendPerson是Person类的友元，不是Student类的友元
        // 故Student::m_StudentID不可访问（友元关系不继承）
        // 但Student::m_ID从基类Person继承而来，Friend是Person的友元，可以访问Student中基类Person的私有部分（继承的各个部分各自负责友元关系）
        // std::cout << "Name = " << student.m_Name << "StudentID = " << std::endl; //
        // student.m_ID正确，student.m_StudentID错误
    }
};

class SmallInt
{
public:
    SmallInt()
        : m_iValue(0)
    {}
    // 转换构造函数，不加explict关键字，则可将std::size_t隐式转换为SmallInt类型。
    SmallInt(int iValue)
        : m_iValue(iValue)
    {}
    // 转换操作符，可以将SmallInt转换为int类型，接受隐式转换
    operator int() const { return m_iValue; }
    // 转换操作符，可以将SmallInt转换为bool类型，不接受隐式转换，但是在条件表达式中可以接受作为隐式转换，其他情况必须显式转换
    explicit operator bool() const { return m_iValue; }

private:
    std::size_t m_iValue;
};

class ClassX
{
public:
    ClassX()
        : m_iX(0)
        , m_iY(0)
    {}

    ClassX(const ClassX& x)
        : m_iX(x.m_iX)
        , m_iY(x.m_iY)
    {}

private:
    int m_iX;
    int m_iY;
};

class ClassY
{
public:
    ClassY()                    = default;
    ClassY(const ClassY& y)     = default;
    ClassY(ClassY&& y) noexcept = default;

private:
    ClassX m_cX;
};

class HasPtr
{
    friend void          swap(HasPtr&, HasPtr&);
    friend bool          operator<(const HasPtr&, const HasPtr&);
    friend std::ostream& operator<<(std::ostream&, const HasPtr&);

public:
    HasPtr(const std::string& str, int index)
        : m_pStr(new std::string(str))
        , m_iIndex(index)
        , m_pUse(new std::size_t(1))
    {}

    HasPtr(const HasPtr& rhs)
        : m_pStr(rhs.m_pStr)
        , m_iIndex(rhs.m_iIndex)
        , m_pUse(rhs.m_pUse)
    {
        std::cout << "HasPtr::HasPtr(const HasPtr& rhs)" << std::endl;
        ++(*m_pUse);
    }

    HasPtr(HasPtr&& rhs) noexcept
        : m_pStr(rhs.m_pStr)
        , m_iIndex(rhs.m_iIndex)
        , m_pUse(rhs.m_pUse)
    {
        rhs.m_pStr   = nullptr;
        rhs.m_iIndex = 0;
        rhs.m_pUse   = nullptr;
    }

    // HasPtr& operator=(const HasPtr& rhs)
    //{
    //	std::cout << "HasPtr::operator=" << std::endl;
    //	++(*rhs.m_pUse); // 优先加赋值操作符右侧的引用计数，避免自赋值时析构掉自己
    //	if ((--(*m_pUse)) == 0)
    //	{
    //		delete m_pStr;
    //		delete m_pUse;
    //	}
    //	m_pStr = rhs.m_pStr;
    //	m_iIndex = rhs.m_iIndex;
    //	m_pUse = rhs.m_pUse;
    //	return *this;
    // }

    HasPtr& operator=(HasPtr rhs)
    {
        std::cout << "HasPtr::operator=" << std::endl;
        swap(*this, rhs);
        return *this;
    }

    ~HasPtr()
    {
        std::cout << "HasPtr::~HasPtr()" << std::endl;
        if (m_pUse && (--(*m_pUse)) == 0)
        {
            delete m_pStr;
            delete m_pUse;
        }
    }

private:
    std::string* m_pStr;
    int          m_iIndex;
    std::size_t* m_pUse;   // 用于记录有多少个对象指向m_pStr的对象
};

inline void swap(HasPtr& lhs, HasPtr& rhs)
{
    using std::swap;
    swap(lhs.m_pStr, rhs.m_pStr);
    swap(lhs.m_iIndex, rhs.m_iIndex);
    swap(lhs.m_pUse, rhs.m_pUse);
}

inline bool operator<(const HasPtr& lhs, const HasPtr& rhs)
{
    return lhs.m_iIndex < rhs.m_iIndex;
}

std::ostream& operator<<(std::ostream& os, const HasPtr& hasPtr)
{
    os << *hasPtr.m_pStr << ", " << hasPtr.m_iIndex << ", useRef = " << *hasPtr.m_pUse;
    return os;
}

class Example
{
public:
    static double              rate;
    static constexpr int       vecSize = 20;
    static std::vector<double> vec;
};

double Example::rate = 6.5;
// 即使一个常量静态成员在类内初始化了，也应该在类的外部定义一下该成员
constexpr int       Example::vecSize;
std::vector<double> Example::vec(Example::vecSize);


class Debug
{
public:
    constexpr Debug(bool b = true)
        : hw(b)
        , io(b)
        , other(b)
    {}
    constexpr Debug(bool h, bool i, bool o)
        : hw(h)
        , io(i)
        , other(o)
    {}
    constexpr bool Any() const { return hw || io || other; }
    void           Set_io(bool b) { io = b; }
    void           Set_hw(bool b) { hw = b; }
    void           Set_other(bool b) { other = b; }

private:
    bool hw;      // 硬件错误
    bool io;      // IO错误
    bool other;   // 其他错误
};


class ClassTemp
{
public:
    explicit ClassTemp(const char* str)
        : m_strMember(str)
    {
        std::cout << "Single Paramter Constractor" << std::endl;
    }

    ClassTemp()
        : ClassTemp("")
    {
        std::cout << "Default Constractor" << std::endl;
    }

    ClassTemp(const ClassTemp& temp)
        : m_strMember(temp.m_strMember)
    {
        std::cout << "Copy Constractor" << std::endl;
    }

    ClassTemp(ClassTemp&& temp) noexcept
        : m_strMember(temp.m_strMember)
    {
        std::cout << "Move Constractor" << std::endl;
    }

    void TestThrowException() { throw std::runtime_error("Test Runtime Error!"); }

public:
    std::string m_strMember;
    int         m_iMember;
};

void PrintArray(const int (*arr)[5])
{
    for (int value : *arr)
    {
        std::cout << value << std::endl;
    }
}

void PopBackArray(int (*arr)[5])
{
    int arrbackup[] = {(*arr)[4], (*arr)[3], (*arr)[2], (*arr)[1], (*arr)[0]};

    int i = 0;
    for (const int& value : arrbackup)
    {
        (*arr)[i] = value;
        ++i;
    }

    PrintArray(arr);
}

static int s_ArrayInts[] = {5, 4, 3, 2, 1};

int (*GetArray1())[5]
{
    return &s_ArrayInts;
}

auto GetArray2() -> int (*)[5]
{
    return &s_ArrayInts;
}

using ArrayInt = int[5];
ArrayInt* GetArray3()
{
    return &s_ArrayInts;
}

decltype(s_ArrayInts)* GetArray4()
{
    return &s_ArrayInts;
}

constexpr int GetArraySize(int iScale)
{
    return 5 * iScale;
}

inline bool CompareString(const std::string& lstr, const std::string& rstr)
{
    return lstr.size() <= rstr.size();
}

typedef bool Func1(const std::string&, const std::string&);
using Func2 = bool(const std::string&, const std::string&);

typedef bool (*FuncPtr1)(const std::string&, const std::string&);
using FuncPtr2 = bool (*)(const std::string&, const std::string&);

const std::string& UseBigger(const std::string& lstr, const std::string& rstr, FuncPtr2 func)
{
    return func(lstr, rstr) ? rstr : lstr;
}

Func1* GetFunc()
{
    return CompareString;
}
FuncPtr2 GetFunc2()
{
    return CompareString;
}

bool (*GetFunc3())(const std::string&, const std::string&)
{
    return CompareString;
}

auto GetFunc4() -> bool (*)(const std::string&, const std::string&)
{
    return CompareString;
}

auto GetFunc5() -> Func2*
{
    return CompareString;
}

auto GetFunc6() -> FuncPtr1
{
    return CompareString;
}

int Add(int lhs, int rhs)
{
    return lhs + rhs;
}

template<typename T1, typename T2> auto TestFun(T1 a, T2 b) -> decltype(a + b)
{
    return a + b;
}

typedef struct
{

} MyTypedefStruct;

struct MyTypeStruct2
{
    MyTypeStruct2* pStruct = nullptr;
};

;

int TestFunc(int* pIntParam1, const std::string& strParam2)
{
    // ...
    return 0;
}

int TestFunc2(float* pFloatParam1, const std::string& strParam2)
{
    // ...
    return 0;
}

typedef int (*MyFunc)(int* pIntParam1, const std::string& strParam2);
using MyFuncPtr = int (*)(int* pIntParam1, const std::string& strParam2);

int* TestFunc3(int, char*)
{
    std::cout << "" << std::endl;
    return nullptr;
}

// 声明 MyIntPtr 为 int*的别名
typedef int MyInt, *MyIntPtr;
// 声明 MyIntArray 为 int[50]的别名
typedef int MyIntArray[50];

using MyIntPtr1   = int*;
using MyIntArray1 = int[50];

#ifdef WIN32
typedef int64_t Int64;
#elif __linux__
typedef long long Int64;
#endif

// 辅助函数，用于尾递归
long long factorialHelper(int n, long long accumulator)
{
    if (n == 0)
    {
        return accumulator;
    }
    return factorialHelper(n - 1, n * accumulator);
}

// 尾递归函数
long long factorial(int n)
{
    return factorialHelper(n, 1);
}


void UnitTest_JoyString();
void UniTest_VirutalInherit();

int main()
{
    int n = 10;
    std::cout << n << "!=" << factorial(n) << std::endl;

    // int* (*a[5])(int, char*);

    // a[0] = TestFunc3;

    // void (*b[10])(void (*)());
    // b[0] = [](void (*Func)()) { Func(); };
    // // doube(*)() (*v)[9];

    // int (*Func)(int* pIntParam1, const std::string& strParam2) = TestFunc;
    // // std::cout << "Hello World" << TestFun(1, 2) << std::endl;

    // MyFunc    myFuncPtr  = TestFunc;
    // MyFuncPtr myFuncPtr2 = TestFunc;

    // // 常规声明多个指针或数组，需要每个变量都带*或[]
    // int *pInt1, *pInt2, *pInt3;
    // int  arrInts1[50], arrInts2[50];
    // // typedef声明了别名后
    // MyIntPtr1   pMyInt1, pMyInt2, pMyInt3;
    // MyIntArray1 arrMyInts1, arrMyInts2;

    // int        value     = 1;
    // const int  cValue    = value;
    // auto       value3    = cValue;
    // auto       value4    = value;
    // auto&      refValue1 = value;
    // auto&      refValue2 = cValue;
    // auto       pValue1   = &value;
    // auto       pValue2   = &cValue;
    // const auto pValue3   = &value;
    // const auto pValue4   = &cValue;

    // auto&& rValue = 100;

    // decltype(*pValue1)   pdVal  = value;
    // decltype(rValue + 1) pdVal2 = value;
    // decltype((rValue)) pdValue3 = rValue;

    // decltype(++value) pdValue4 = rValue;

    // int  lambdaCatchValue = 100;
    // auto lambdaFunc       = [lambdaCatchValue]()  mutable
    // {
    //     ++lambdaCatchValue;
    //     std::cout << "Inner Lambda: " << lambdaCatchValue << std::endl;
    //     return lambdaCatchValue;
    // };

    // lambdaFunc();
    // lambdaCatchValue = 1000;
    // lambdaFunc();
    // std::cout << "Outter Lambda: " << lambdaCatchValue << std::endl;

    // char strContent1[20] = "Hello ";
    // std::cout << "str1 length: " << strlen(strContent1) << std::endl;
    // const char* strContent2 = "World";
    // std::cout << "str2 length: " << strlen(strContent2) << std::endl;
    // strcpy(strContent1 + strlen(strContent1), strContent2);
    // std::cout << "strcpy: " << strContent1 << std::endl;
    // const char* strContent3 = ", JoyatY!";
    // std::cout << "strcat: " << strcat(strContent1, strContent3) << std::endl;

    // float a   = 45.5f;
    // float b   = -54.3f;
    // int*  pia = reinterpret_cast<int*>(&a);
    // int*  pib = reinterpret_cast<int*>(&b);
    // std::cout << "a:" << a << ", *pia: " << *pia << std::endl;
    // std::cout << "b:" << b << ", *pib: " << *pib << std::endl;
    // *pia = (*pia) ^ (*pib);
    // *pib = (*pia) ^ (*pib);
    // *pia = (*pia) ^ (*pib);
    // a    = *(reinterpret_cast<float*>(pia));
    // b    = *(reinterpret_cast<float*>(pib));

    // std::cout << "*pia: " << *pia << ", a = " << a << std::endl;
    // std::cout << "*pib: " << *pib << ", b = " << b << std::endl;

    // int c = 0;
    // int d = 56545;
    // c     = c ^ d;
    // d     = c ^ d;
    // c     = c ^ d;
    // std::cout << "c: " << c << std::endl;
    // std::cout << "d: " << d << std::endl;


    // int value = 1;
    // const int cValue = 2;
    // int* pValue = &value;
    // const int* cpValue = &value;
    // int* const pcValue = &value;
    // const int* const cpcValue = &value;

    // ++(*pcValue);

    // int& refValue = value;
    // const int& crefValue = value;


    // auto aValue = value;
    // auto caValue = cValue;
    // auto paValue = pValue;
    // auto cpaValue = cpValue;
    // auto cpcaValue = cpcValue;
    // auto pcaValue = pcValue;


    // auto crefAvalue = crefValue;

    // MyInt value = 100;

    // class EmptyA {};
    // std::cout << sizeof(EmptyA) << std::endl;

    // std::cout << "Hello, The Basic" << std::endl;

    // int size1 = sizeof(ClassTemp);
    // ClassTemp objClassTmp("123412342134");
    // ClassTemp objClassTmp2;

    // int i = 108, j = 5;
    // float r = static_cast<float>(j) / i;
    // void* p = &i;
    // int* pi = static_cast<int*>(p);
    // char* pc = reinterpret_cast<char*>(pi);
    // std::string stri = pc;
    // constexpr int k = 10;
    // const int* cpk = &k;
    // int* pk = const_cast<int*>(cpk);
    // *pk = 50;

    // std::cout << stri << std::endl;

    // bool bSwitchFlag = false;
    // switch (bSwitchFlag)
    // {
    // case true:
    // 	int value;
    // 	break;
    // case false:
    // 	value = 10;
    // 	std::cout << "value = " << value << std::endl;
    // 	break;
    // }

    // try
    // {
    // 	objClassTmp.TestThrowException();
    // }
    // catch (const std::runtime_error& error)
    // {
    // 	std::cout << error.what() << std::endl;
    // }

    // PopBackArray(GetArray1());
    // PopBackArray(GetArray2());
    // PopBackArray(GetArray3());
    // PopBackArray(GetArray4());

    // std::cout << "=============================" << std::endl;
    // int arrayInts1[GetArraySize(2)];
    // int iValue = 1;
    // int sizeValue = GetArraySize(iValue);
    // std::cout << sizeValue << std::endl;

    // std::cout << "=============================" << std::endl;
    // Func1* func = GetFunc();
    // std::cout << UseBigger("Hello", " World!", func) << std::endl;

    // std::cout << "=============================" << std::endl;
    // constexpr Debug io_sub(false, true, false);
    // if (io_sub.Any())
    // {
    //     std::cerr << "Print some error message." << std::endl;
    // }

    // std::cout << "=============================" << std::endl;
    // HasPtr pHasPtr1("Hello", 1);
    // HasPtr pHasPtr2("World", 2);
    // HasPtr pHasPtr3("Joy", 5);
    // HasPtr pHasPtr4("Yang", 3);


    // std::cout << "=============================" << std::endl;
    // std::vector<HasPtr> vecHasPtrs = { pHasPtr2, pHasPtr4, pHasPtr1, pHasPtr3, pHasPtr4 };
    // std::cout << "=============================" << std::endl;
    // using std::sort;
    // std::cout << "=================================" << std::endl;
    // sort(vecHasPtrs.begin(), vecHasPtrs.end());
    // std::cout << "=================================" << std::endl;
    // for (const HasPtr& hasPtr : vecHasPtrs)
    // {
    // 	std::cout << hasPtr << std::endl;
    // }
    // std::cout << "====================================" << std::endl;
    // std::vector<int> vecs;
    // std::cout << vecs.capacity() << ", " << vecs.size() << std::endl;

    // ClassX x;
    // ClassX xi = std::move(x);

    // ClassY y;
    // ClassY yi = std::move(y);

    // std::string s1 = "Hello", s2 = "World";
    // auto n = (s1 + s2).find("o");
    // s1 + s2 = "Wow";

    // struct Divide
    // {
    // 	int operator()(int lhs, int rhs) { return lhs / rhs; }
    // };

    // std::cout << "=========================================" << std::endl;
    // std::function<int(int, int)> funcDivide = Divide();
    // std::function<int(int, int)> funcMod = [](int lhs, int rhs) { return lhs % rhs; };
    // std::unordered_map<std::string, std::function<int(int, int)>> mapOperations;
    // mapOperations.emplace("+", Add);
    // mapOperations.emplace("%", funcMod);
    // mapOperations.emplace("/", funcDivide);
    // std::cout << mapOperations["+"](10, 5) << " " << mapOperations["/"](9, 3) << " " <<
    // mapOperations["%"](15, 4) << std::endl;

    // std::cout << "==============================================" << std::endl;
    // SmallInt value = 10;
    // if (value)
    // {
    // 	std::cout << "cast to bool" << std::endl;
    // }
    // std::cout << value + 23 << std::endl;

    // Person person1(1001, "Hello");
    // Person person2(1002, "World");
    // Student student1(1003, 9001, "Welcome");
    // Student student2(1004, 9002, "Student");

    // Person* pPer1 = &person1, * pPer2 = &student1;

    // pPer1->MemFunc();
    // pPer2->MemFunc();

    // // student1.MemFunc(); //
    // 错误，继承与基类的MemFunc函数，由于派生类中定义了同名函数，但参数列表不一致，导致基类中的MemFunc函数被隐藏，可通过添加基类域调用。
    // student1.MemFunc(); // 正确，通过添加基类的域调用被隐藏的函数
    // student2.MemFunc(1);

    // std::cout << "=============================" << std::endl;
    // Person person3;
    // Student student3;
    // std::cout << "=============================" << std::endl;

    // FriendPerson friends;
    // friends.PrintID(person1);
    // // friends.PrintID(student2);
    // friends.PrintStudentID(student1);
    // 单元测试 - C风格字符串自定义封装
    // UnitTest_JoyString();
    // 单元测试 - VirtualInherit
    UniTest_VirutalInherit();

    std::cin.get();

    return 0;
}
