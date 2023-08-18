#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>


class ClassX
{
public:
	ClassX()
		: m_iX(0)
		, m_iY(0)
	{
	}

	ClassX(const ClassX& x)
		: m_iX(x.m_iX)
		, m_iY(x.m_iY)
	{
	}

private:
	int m_iX;
	int m_iY;
};

class ClassY
{
public:
	ClassY() = default;
	ClassY(const ClassY& y) = default;
	ClassY(ClassY&& y) noexcept = default;

private:
	ClassX m_cX;
};

class HasPtr
{
	friend void swap(HasPtr&, HasPtr&);
	friend std::ostream& operator<<(std::ostream&, const HasPtr&);
public:
	HasPtr(const std::string& str, int index)
		: m_pStr(new std::string(str))
		, m_iIndex(index)
		, m_pUse(new std::size_t(1))
	{
	}

	HasPtr(const HasPtr& rhs)
		: m_pStr(rhs.m_pStr)
		, m_iIndex(rhs.m_iIndex)
		, m_pUse(rhs.m_pUse)
	{
		std::cout << "HasPtr::HasPtr(const HasPtr& rhs)" << std::endl;
		++(*m_pUse);
	}

	HasPtr& operator=(const HasPtr& rhs)
	{
		std::cout << "HasPtr::operator=" << std::endl;
		++(*rhs.m_pUse); // 优先加赋值操作符右侧的引用计数，避免自赋值时析构掉自己
		if ((--(*m_pUse)) == 0)
		{
			delete m_pStr;
			delete m_pUse;
		}
		m_pStr = rhs.m_pStr;
		m_iIndex = rhs.m_iIndex;
		m_pUse = rhs.m_pUse;
		return *this;
	}

	//HasPtr& operator=(HasPtr rhs)
	//{
	//	std::cout << "HasPtr::operator=" << std::endl;
	//	swap(*this, rhs);
	//	return *this;
	//}

	~HasPtr()
	{
		std::cout << "HasPtr::~HasPtr()" << std::endl;
		if ((--(*m_pUse)) == 0)
		{
			delete m_pStr;
			delete m_pUse;
		}
	}

	bool operator<(const HasPtr& rhs)
	{
		return m_iIndex < rhs.m_iIndex;
	}

private:
	std::string* m_pStr;
	int m_iIndex; 
	std::size_t* m_pUse; // 用于记录有多少个对象指向m_pStr的对象
};

inline void swap(HasPtr& lhs, HasPtr& rhs)
{
	using std::swap;
	swap(lhs.m_pStr, rhs.m_pStr);
	swap(lhs.m_iIndex, rhs.m_iIndex);
	swap(lhs.m_pUse, rhs.m_pUse);
}

std::ostream& operator<<(std::ostream& os, const HasPtr& hasPtr)
{
	os << *hasPtr.m_pStr << ", " << hasPtr.m_iIndex << ", useRef = " << *hasPtr.m_pUse;
	return os;
}

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
    
	std::cout << "=============================" << std::endl;
	HasPtr pHasPtr1("Hello", 1);
	HasPtr pHasPtr2("World", 2);
	HasPtr pHasPtr3("Joy", 5);
	HasPtr pHasPtr4("Yang", 3);


	std::cout << "=============================" << std::endl;
	std::vector<HasPtr> vecHasPtrs = { pHasPtr2, pHasPtr4, pHasPtr1, pHasPtr3, pHasPtr4 };
	std::cout << "=============================" << std::endl;
	using std::sort;
	std::cout << "=================================" << std::endl;
	sort(vecHasPtrs.begin(), vecHasPtrs.end());
	std::cout << "=================================" << std::endl;
	for (const HasPtr& hasPtr : vecHasPtrs)
	{
		std::cout << hasPtr << std::endl;
	}
	std::cout << "====================================" << std::endl;
	std::vector<int> vecs;
	std::cout << vecs.capacity() << ", " << vecs.size() << std::endl;

	ClassX x;
	ClassX xi = std::move(x);

	ClassY y;
	ClassY yi = std::move(y);

	std::cin.get();
	return 0;
}
