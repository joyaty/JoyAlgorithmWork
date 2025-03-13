#include <chrono>
#include <iostream>
#include <thread>

static void RunOnThread1()
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Method run in thread1:" << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

static void RunOnThread2(int value) 
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Method run in thread2:" << i << std::endl;
        ++value;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

static void RunOnThread3(int* value) 
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Method run in thread3:" << i << std::endl;
        ++(*value);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

static void RunOnThread4(int& value)
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Method run in thread4:" << i << std::endl;
        ++value;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

class ClassA
{
public:
    void RunOnThread() 
    {
        for (int i = 0; i < 10; ++i)
        {
            std::cout << "Method run in thread5:" << i << std::endl;
            ++value;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

public:
    int value{0};
};

class ClassW
{
public:
    void operator()() 
    {
        for (int i = 0; i < 10; ++i)
        {
            std::cout << "Method run in thread6:" << i << std::endl;
            ++value;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

public:
    int value{0};
};

void TestSTLThreadFunc()
{
    std::thread myThread1 = std::thread(RunOnThread1);
    int         value1     = 0;
    int         value2    = 0;
    int         value3     = 0;
    std::thread myThread2 = std::thread(RunOnThread2, value1);
    std::thread myThread3  = std::thread(RunOnThread3, &value2);
    std::thread myThread4  = std::thread(RunOnThread4, std::ref(value3));
       
    ClassA      instA{};
    std::thread myThread5 = std::thread(&ClassA::RunOnThread, std::ref(instA));
    ClassA      instB{};
    std::thread myThread6 = std::thread(&ClassA::RunOnThread, &instB);

    ClassW instW1{};
    ClassW instW2{};
    std::thread myThread7 = std::thread(instW1);
    std::thread myThread8 = std::thread(std::ref(instW2));

    myThread1.join();
    myThread2.join();
    myThread3.join();
    myThread4.join();
    myThread5.join();
    myThread6.join();
    myThread7.join();
    myThread8.join();

    std::cout << "value1 = " << value1 << std::endl;
    std::cout << "value2 = " << value2 << std::endl;
    std::cout << "value3 = " << value3 << std::endl;
    std::cout << "instA.value = " << instA.value << std::endl;
    std::cout << "instB.value = " << instB.value << std::endl;

    std::cout << "instW1.value = " << instW1.value << std::endl;
    std::cout << "instW2.value = " << instW2.value << std::endl;
}
