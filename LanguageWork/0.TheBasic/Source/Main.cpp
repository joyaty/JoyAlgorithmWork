#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

class Example
{
public:
    static double rate;
    static constexpr int vecSize = 20;
    static std::vector<double> vec;
};

double Example::rate = 6.5;
// 即使一个常量静态成员在类内初始化了，也应该在类的外部定义一下该成员
constexpr int Example::vecSize;
std::vector<double> Example::vec(Example::vecSize);


class Debug
{
public:
    constexpr Debug(bool b = true) : hw(b), io(b), other(b) {}
    constexpr Debug(bool h, bool i, bool o) : hw(h), io(i), other(o) {}
    constexpr bool Any() const { return hw || io || other; }
    void Set_io(bool b) { io = b; }
    void Set_hw(bool b) { hw = b; }
    void Set_other(bool b) { other = b; }
private:
    bool hw;    // 硬件错误
    bool io;    // IO错误
    bool other; // 其他错误
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
        std::cout << "Default Constractor" <<std::endl;
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

	void TestThrowException()
	{
		throw std::runtime_error("Test Runtime Error!");
	}

public:
	std::string m_strMember;
	int m_iMember;
};

void PrintArray(const int (*arr)[5])
{
	for (int value : *arr)
	{
		std::cout << value << std::endl;
	}
}

void PopBackArray(int(*arr)[5])
{
	int arrbackup[] = { (*arr)[4], (*arr)[3], (*arr)[2], (*arr)[1], (*arr)[0] };

	int i = 0;
	for (const int& value : arrbackup)
	{
		(*arr)[i] = value;
		++i;
	}

	PrintArray(arr);

}

static int s_ArrayInts[] = { 5, 4, 3, 2, 1 };

int (*GetArray1())[5]
{
	return &s_ArrayInts;
}

auto GetArray2() -> int(*)[5]
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
using FuncPtr2 = bool(*)(const std::string&, const std::string&);

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

auto GetFunc4() -> bool(*)(const std::string&, const std::string&)
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

int main()
{
	std::cout << "Hello, The Basic" << std::endl;

	int size1 = sizeof(ClassTemp);
	ClassTemp objClassTmp("123412342134");
    ClassTemp objClassTmp2;

	int i = 108, j = 5;
	float r = static_cast<float>(j) / i;
	void* p = &i;
	int* pi = static_cast<int*>(p);
	char* pc = reinterpret_cast<char*>(pi);
	std::string stri = pc;
	constexpr int k = 10;
	const int* cpk = &k;
	int* pk = const_cast<int*>(cpk);
	*pk = 50;

	std::cout << stri << std::endl;

	bool bSwitchFlag = false;
	switch (bSwitchFlag)
	{
	case true:
		int value;
		break;
	case false:
		value = 10;
		std::cout << "value = " << value << std::endl;
		break;
	}

	try
	{
		objClassTmp.TestThrowException();
	}
	catch (const std::runtime_error& error)
	{
		std::cout << error.what() << std::endl;
	}

	PopBackArray(GetArray1());
	PopBackArray(GetArray2());
	PopBackArray(GetArray3());
	PopBackArray(GetArray4());

	std::cout << "=============================" << std::endl;
	int arrayInts1[GetArraySize(2)];
	int iValue = 1;
	int sizeValue = GetArraySize(iValue);
	std::cout << sizeValue << std::endl;

	std::cout << "=============================" << std::endl;
	Func1* func = GetFunc();
	std::cout << UseBigger("Hello", " World!", func) << std::endl;
    
    std::cout << "=============================" << std::endl;
    constexpr Debug io_sub(false, true, false);
    if (io_sub.Any())
    {
        std::cerr << "Print some error message." << std::endl;
    }
    
    using std::swap;
    std::string strHello{ "Hello" };
    std::string strWorld{ "World" };
    swap(strHello, strWorld);
    
    int iHello{ 10 };
    int iWorld{ 99 };
    swap(iHello, iWorld);

	std::cin.get();
	return 0;
}
