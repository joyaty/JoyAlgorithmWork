#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string>

struct RetSubThread 
{
    int retValue;
    std::string retDesc;
};

void* RunOnSubThread(void* pArgs)
{
    int sum = 0;
    for (int i = 0; i < 10; ++i)
    {
        sum += i;
    }
    std::cout << "SubThread End. Sub Thread ID: " << pthread_self() << std::endl;
    for (int i = 0; i < 5; ++i)
    {
        sum += i;
    }
    std::cout << "Sum: " << sum << std::endl;
    RetSubThread* pRetData = static_cast<RetSubThread*>(pArgs);
    pRetData->retValue = sum;
    pRetData->retDesc = "I am back";
    // 子线程退出，可通过参数retData回传返回数据
    pthread_exit(pRetData);
    return nullptr;
}

pthread_mutex_t g_Mutex;

constexpr int kMaxCount = 50;

int g_Number = 0;

void* RunOnSubThreadA(void* pArgs)
{
    for (int i = 0; i < kMaxCount; ++i)
    {
        pthread_mutex_lock(&g_Mutex);
        int cur = g_Number;
        ++cur;
        g_Number = cur;
        printf("Thread A, ID: %lu, Count: %d \n", pthread_self(), g_Number);
        pthread_mutex_unlock(&g_Mutex);
        usleep(3);
    }
}

void* RunOnSubThreadB(void* pArgs)
{
    for (int i = 0; i < kMaxCount; ++i)
    {
        pthread_mutex_lock(&g_Mutex);
        int cur = g_Number;
        ++cur;
        usleep(10);
        g_Number = cur;
        pthread_mutex_unlock(&g_Mutex);
        printf("Thread B, ID: %lu, Count: %d \n", pthread_self(), g_Number);
    }
}

void TestThreadBase()
{
    pthread_t threadID = pthread_self();
    std::cout << "Main Thread ID:" << threadID << std::endl;
    pthread_t subThreadID{};
    // 调用线程栈上的一块内存，可以用于传入参数到子线程，也可用于子线程退出时回传数据给调用线程
    RetSubThread retData{};
    pthread_create(&subThreadID, nullptr, RunOnSubThread, &retData);
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "MainThread Output: " << i << std::endl;
    }
    void* pRetFromSubThread = nullptr;
//    // 当前线程阻塞，等待SubThread执行结束，pRetFromSubThread可用于接收子线程的回传数据
//    // pthread_join函数会回收线程在内核去的一些资源
//    pthread_join(subThreadID, &pRetFromSubThread);
//    RetSubThread* pRetValue = static_cast<RetSubThread*>(pRetFromSubThread);
//    std::cout << "RetValue: " << pRetValue->retValue << ", RetDesc: " << pRetValue->retDesc << std::endl;
    // 线程分离，子线程资源自己回收，不会阻塞调用线程
    pthread_cancel(subThreadID);
    // pthread_detach(subThreadID);
    pthread_exit(nullptr);
}

void TestThreadAndMutex()
{
    pthread_t subThreadA;
    pthread_t subThreadB;
    
    pthread_mutex_init(&g_Mutex, nullptr);
    
    pthread_create(&subThreadA, nullptr, RunOnSubThreadA, nullptr);
    pthread_create(&subThreadB, nullptr, RunOnSubThreadB, nullptr);
    
    pthread_join(subThreadA, nullptr);
    pthread_join(subThreadB, nullptr);
    
    pthread_mutex_destroy(&g_Mutex);
}

void TestThreadFunc()
{
    TestThreadAndMutex();
}
