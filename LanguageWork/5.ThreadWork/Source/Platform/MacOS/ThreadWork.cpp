#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <list>

/// 线程传入传出参数结构体
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

/// 线程的常用基本方法
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


// 互斥锁
pthread_mutex_t g_Mutex;
// 最大循环次数
constexpr int kMaxCount = 50;
// 全局变量，各线程都可访问
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

/// 线程同步 - 互斥锁
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

pthread_rwlock_t g_Rwlock;

void* RunOnSubThreadForRead(void* pArgs)
{
    for (int i = 0; i < kMaxCount; ++i)
    {
        pthread_rwlock_rdlock(&g_Rwlock);
        printf("Thread Read, ID: %lu, Value: %d \n", pthread_self(), g_Number);
        pthread_rwlock_unlock(&g_Rwlock);
        usleep(rand() % 3);
    }
    pthread_exit(nullptr);
    return nullptr;
}

void* RunOnSubThreadForWrite(void* pArgs)
{
    for (int i = 0; i < kMaxCount; ++i)
    {
        pthread_rwlock_wrlock(&g_Rwlock);
        int cur = g_Number;
        ++cur;
        g_Number = cur;
        printf("Thread Write, ID: %lu, Value: %d \n", pthread_self(), g_Number);
        pthread_rwlock_unlock(&g_Rwlock);
        usleep(rand() % 3);
    }
    pthread_exit(nullptr);
    return nullptr;
}

/// 线程同步 - 读写锁
void TestRwlock()
{
    // 初始化读写锁
    pthread_rwlock_init(&g_Rwlock, nullptr);
    
    pthread_t tReads[5];
    pthread_t tWrites[3];
    // 创建写子线程
    for (int i = 0; i < 3; ++i)
    {
        pthread_create(&tWrites[i], nullptr, RunOnSubThreadForWrite, nullptr);
    }
    // 创建读子线程
    for (int i = 0; i < 5; ++i)
    {
        pthread_create(&tReads[i], nullptr, RunOnSubThreadForRead, nullptr);
    }
    
    // 阻塞当前线程，等待子线程结束
    for (int i = 0; i < 5; ++i)
    {
        pthread_join(tReads[i], nullptr);
    }
    for (int i = 0; i < 3; ++i)
    {
        pthread_join(tWrites[i], nullptr);
    }
    // 销毁读写锁
    pthread_rwlock_destroy(&g_Rwlock);
}

/// 链表节点
struct LinkNode
{
    int m_nValue;
    LinkNode* m_pNext;
};
// 链表结构体
struct LinkList
{
    LinkNode* m_pHeader;
};
// 全局链表
LinkList g_List{};
// 全局条件变量
pthread_cond_t g_Condition;

struct STLListElement
{
    int m_nValue;
};
std::list<STLListElement> g_stlList;
// 链表空条件
pthread_cond_t g_EmptyCondition;
// 链表满条件
pthread_cond_t g_FullCondition;

/// 生产者线程 - 向全局链表添加节点
void* Producer(void* pArgs)
{
    // 自定义链表
//    while (true)
//    {
//        // 新节点加入到链表头部
//        pthread_mutex_lock(&g_Mutex);
//        LinkNode* pNewNode = new LinkNode();
//        pNewNode->m_nValue = ++g_Number;
//        pNewNode->m_pNext = g_List.m_pHeader;
//        g_List.m_pHeader = pNewNode;
//        printf("Thread Producer. ID: %ld. Value: +%d \n", pthread_self(), pNewNode->m_nValue);
//        pthread_mutex_unlock(&g_Mutex);
//        // 生产完成，通知消费者线程，可以唤醒开始消费
//        pthread_cond_broadcast(&g_Condition);
//        // 生产完成，随机休眠0-2秒
//        sleep(rand() % 3);
//    }
    
    // STL中的链表
    while (true)
    {
        pthread_mutex_lock(&g_Mutex);
        while (g_stlList.size() >= 5)
        {
            printf("Thread Producer sleep, wait consumer. ID: %ld \n", pthread_self());
            pthread_cond_wait(&g_FullCondition, &g_Mutex);
        }
        STLListElement newElement;
        newElement.m_nValue = ++g_Number;
        g_stlList.push_front(newElement);
        printf("Thread Producer. ID: %ld. Value: +%d \n", pthread_self(), newElement.m_nValue);
        pthread_cond_broadcast(&g_EmptyCondition);
        pthread_mutex_unlock(&g_Mutex);
        sleep(rand() % 3);
    }
    return nullptr;
}

/// 消费者线程 - 全局链表移除节点
void* Consumer(void* pArgs)
{
    // 自定义链表
//    while (true)
//    {
//        pthread_mutex_lock(&g_Mutex);
//        while (g_List.m_pHeader == nullptr)
//        {
//            printf("Thread Consumer sleep, wait produce. ID: %ld \n", pthread_self());
//            // 全局队列为空，消费者线程挂起，等待生产者生产
//            pthread_cond_wait(&g_Condition, &g_Mutex);
//        }
//        LinkNode* pNode = g_List.m_pHeader;
//        g_List.m_pHeader = g_List.m_pHeader->m_pNext;
//        printf("Thread Consumer. ID: %ld, Value: -%d \n", pthread_self(), pNode->m_nValue);
//        delete pNode;
//        pNode = nullptr;
//        pthread_mutex_unlock(&g_Mutex);
//        sleep(rand() % 3);
//    }
    
    // STL中的链表
    while (true)
    {
        pthread_mutex_lock(&g_Mutex);
        while (g_stlList.size() <= 0)
        {
            printf("Thread Consumer sleep, wait produce. ID: %ld \n", pthread_self());
            // 共享队列为空，通知满队列而阻塞的生产者线程开始生产
            pthread_cond_broadcast(&g_FullCondition);
            // 消费者线程休眠，等待生产者生产后唤醒
            pthread_cond_wait(&g_EmptyCondition, &g_Mutex);
        }
        const STLListElement& element = g_stlList.front();
        printf("Thread Consumer. ID: %ld, Value: -%d \n", pthread_self(), element.m_nValue);
        g_stlList.pop_front(); // 移除头节点
        pthread_mutex_unlock(&g_Mutex);
        sleep(rand() % 3);
    }
    
    return nullptr;
}

/// 条件变量
void TestCondition()
{
    pthread_cond_init(&g_Condition, nullptr);
    pthread_cond_init(&g_FullCondition, nullptr);
    pthread_cond_init(&g_EmptyCondition, nullptr);
    pthread_mutex_init(&g_Mutex, nullptr);
    
    const int kProducerCount = 5;
    const int kConsumerCount = 3;
    pthread_t producer[kProducerCount];
    pthread_t consumer[kConsumerCount];
    for (int i = 0; i < kProducerCount; ++i)
    {
        pthread_create(&producer[i], nullptr, Producer, nullptr);
    }
    for (int i = 0; i < kConsumerCount; ++i)
    {
        pthread_create(&consumer[i], nullptr, Consumer, nullptr);
    }
    
    for (int i = 0; i < kProducerCount; ++i)
    {
        pthread_join(producer[i], nullptr);
    }
    
    for (int i = 0; i < kConsumerCount; ++i)
    {
        pthread_join(consumer[i], nullptr);
    }
    pthread_mutex_destroy(&g_Mutex);
    pthread_cond_destroy(&g_EmptyCondition);
    pthread_cond_destroy(&g_FullCondition);
    pthread_cond_destroy(&g_Condition);
}

void TestThreadFunc()
{
//    TestThreadBase()();
//    TestThreadAndMutex();
//    TestRwlock();
    TestCondition();
}
