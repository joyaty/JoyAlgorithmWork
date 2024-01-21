#include <iostream>
#include <string>
#include <vector>

// 前置声明类模版
template <typename T> class FooPtr;
// 前置声明类模板
template <typename T> class Foo;
// 前置声明函数模板
template <typename T> bool operator== (const FooPtr<T>&, const FooPtr<T>&);

template <typename T> class Foo
{
	// 友元声明，对应模板参数实例化的类模板FooPtr<T>和函数模板operator==都是相应模板参数实例化的Food<T>的友元
	friend class FooPtr<T>;
	friend bool operator==(const FooPtr<T>&, const FooPtr<T>&);

	// C++11新标准！类型模板参数T也可作为友元
	friend T; // 注意，模板参数T不一定是一个class，这里friend申明不需要加class

	// 类模板也可有静态函数和静态成员，与普通类一样，静态函数和静态成员每个模板参数实例化的类共享，不同模板参数实例化的类间不共享
public:
	static int GetSize() { return s_Count; }
private:
	static int s_Count;
};

// 类模板的静态成员定义并且初始化
template <typename T> int Foo<T>::s_Count = 100;

template <typename T> class FooPtr
{
	// 类定义 ...
};

template <typename T> bool operator== (const FooPtr<T>& lhs, const FooPtr<T>& rhs)
{
	// 函数体实现 ...
	return false;
}

// 模板别名
typedef Foo<std::string> strFoo;
// C++11 新标准，为类模板定义类型别名
template<typename T> using Twin = std::pair<T, T>;
Twin<std::string> strTwin; // strTwin是std::pair<std::string, std::string>类型

// 别名可以固定一到多个模板参数
template<typename T> using PairNo = std::pair<T, unsigned int>;
PairNo<float> fPairNo; // fPairNo是std::pair<float, unsigned int>类型

// 使用typename关键字，指定跟在模板参数T::之后的是一个类型，而不是静态变量
template <typename T> typename T::value_type Top(const T& container)
{
	if (!container.empty())
	{
		return container.back();
	}
	else
	{
		return typename T::value_type();
	}
}

// 与函数参数一样，模板参数也可提供默认模板参数，规则也是其右侧所有的模板参数都有默认模板参数时，当前模板参数才允许有默认参数
template <typename T, typename F = std::less<T>> int Compare(const T& lhs, const T& rhs, F f = F())
{
	if (f(lhs, rhs)) return -1;
	if (f(rhs, lhs)) return 1;
	return 0;
}

template <typename T = std::string> class TypeWrap
{
public:
	explicit TypeWrap(const T& value) : m_Value(value) {}

private:
	T m_Value;
};
// 使用默认模板参数类型std::string，必须用空的<>来定义使用默认模板参数的模板实例化类
TypeWrap<> strWrap("");
// 使用自定义模板参数
TypeWrap<int> intWrap(1000);

int main()
{
	Foo<std::string> foo{};
	std::cout << "Template Static Member: " << foo.GetSize() << std::endl;

	std::vector<int> vecInts{};
	std::vector<int>::value_type value = Top(vecInts);
	std::cout << "Use keyword 'typename' to tell compiler that is a type after '::': " << value << std::endl;

	std::cout << "Default template param: " << Compare(2, 1) << ", " << Compare(1, 10) << ", " << Compare(10, 10) << std::endl;

	std::cin.get();
	return 0;
}