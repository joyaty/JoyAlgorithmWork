#include <iostream>
#include <string>
#include <vector>

// ǰ��������ģ��
template <typename T> class FooPtr;
// ǰ��������ģ��
template <typename T> class Foo;
// ǰ����������ģ��
template <typename T> bool operator== (const FooPtr<T>&, const FooPtr<T>&);

template <typename T> class Foo
{
	// ��Ԫ��������Ӧģ�����ʵ��������ģ��FooPtr<T>�ͺ���ģ��operator==������Ӧģ�����ʵ������Food<T>����Ԫ
	friend class FooPtr<T>;
	friend bool operator==(const FooPtr<T>&, const FooPtr<T>&);

	// C++11�±�׼������ģ�����TҲ����Ϊ��Ԫ
	friend T; // ע�⣬ģ�����T��һ����һ��class������friend��������Ҫ��class

	// ��ģ��Ҳ���о�̬�����;�̬��Ա������ͨ��һ������̬�����;�̬��Աÿ��ģ�����ʵ�������๲����ͬģ�����ʵ��������䲻����
public:
	static int GetSize() { return s_Count; }
private:
	static int s_Count;
};

// ��ģ��ľ�̬��Ա���岢�ҳ�ʼ��
template <typename T> int Foo<T>::s_Count = 100;

template <typename T> class FooPtr
{
	// �ඨ�� ...
};

template <typename T> bool operator== (const FooPtr<T>& lhs, const FooPtr<T>& rhs)
{
	// ������ʵ�� ...
	return false;
}

// ģ�����
typedef Foo<std::string> strFoo;
// C++11 �±�׼��Ϊ��ģ�嶨�����ͱ���
template<typename T> using Twin = std::pair<T, T>;
Twin<std::string> strTwin; // strTwin��std::pair<std::string, std::string>����

// �������Թ̶�һ�����ģ�����
template<typename T> using PairNo = std::pair<T, unsigned int>;
PairNo<float> fPairNo; // fPairNo��std::pair<float, unsigned int>����

// ʹ��typename�ؼ��֣�ָ������ģ�����T::֮�����һ�����ͣ������Ǿ�̬����
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

// �뺯������һ����ģ�����Ҳ���ṩĬ��ģ�����������Ҳ�����Ҳ����е�ģ���������Ĭ��ģ�����ʱ����ǰģ�������������Ĭ�ϲ���
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
// ʹ��Ĭ��ģ���������std::string�������ÿյ�<>������ʹ��Ĭ��ģ�������ģ��ʵ������
TypeWrap<> strWrap("");
// ʹ���Զ���ģ�����
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