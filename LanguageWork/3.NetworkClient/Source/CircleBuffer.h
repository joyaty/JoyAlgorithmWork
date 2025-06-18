
#pragma once

#include <atomic>
#include <cstddef>
#include <mutex>
#include <queue>

namespace Joy
{
    // 环形缓冲区，用于应用层的网络数据发送和接收缓冲区
    class CircleBuffer final
    {
    public:
        CircleBuffer(size_t chunkSize = kChunkSize);
        ~CircleBuffer();

    public:
        // 获取环形缓冲区有效数据长度
        size_t GetBufferSize() const;
        // 数据写入缓冲区
        void Write(char* pData, size_t dataSize);
        // 缓冲区读出数据
        void Read(char* pData, size_t& dataSize);

    private:
        // 工作buffer队列尾部添加新的缓冲块
        void AddLast();
        // 移除工作buffer队列头部的缓冲块
        void RemoveFirst();

    private:
        // 缓冲区的工作Buffer
        std::queue<char*> m_WorkBuffer{};
        // 缓冲区的空闲Buffer，用于缓冲块回收复用
        std::queue<char*> m_FreeBuffer{};
        // 每个缓冲块的大小
        size_t m_ChunkSize;
        // 数据段头索引偏移，仅读取线程操作
        std::atomic<size_t> m_HeadIndex;
        // 数据段尾索引偏移，仅写入线程操作
        std::atomic<size_t> m_TailIndex;

    private:
        // 队列操作锁，用于队列操作多线程安全
        std::mutex m_Mutex{};

    private:
        // 每个缓冲块大小
        constexpr static size_t kChunkSize = 8192;
    };
}   // namespace Joy