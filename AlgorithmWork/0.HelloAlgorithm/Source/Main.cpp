
#include <iostream>
#include <thread>

template <int N> class TestClass
{
    
};

constexpr int FivePlus(int x)
{
    return 5 + x;
}

void TestFunc1(const int x)
{
    // TestClass<x> c1{};
    TestClass<FivePlus(5)> c2{};
}

long long total = 0;

void ThreadFunc()
{
    for (long long i = 0; i < 100000000LL; ++i)
    {
        total += 1;
    }
}

int main()
{
    int testValue = 100;
    std::cout << testValue << std::endl;
    std::thread firstThread(ThreadFunc);
    std::thread secondThread(ThreadFunc);

    //firstThread.join();
    //secondThread.join();

    std::cout << total << std::endl;
//    const char* strHello1 = "Hello Algorithm char";
//    const wchar_t* strHello2 = L"Hello Algorithm wchar_t";
//    const char16_t* strHello3 = u"Hello Algorithm char16_t";
//    const char32_t* strHello4 = U"Hello Algorithm char32_t";
//	std::cout << strHello1 << std::endl;
//    
//    char strHello[] = "Hello World";
//    strHello[1] = 'a';
//    std::cout << strHello << std::endl;
//    
//    const char* strR = R"(line1 \0
//line2
//line3)";
//    
//    std::cout << strR << strlen(strR) << std::endl;
//    
//    // lambda表达式中使用mutable修饰，使按值传递的外部变量允许在lambda表达式内部被修改
//    int x = 10;
//    auto f = [=]() mutable
//    {
//        ++x;// 使用mutable修饰了lambda表达式，允许修改x，否则编译报错
//        std::cout << x << std::endl; // 这里输出11
//    };
//    f();
//    std::cout << x << std::endl; // 这里输出10，按值传递，不改变原始值
    
    
    std::cin.get();
	return 0;
}
