
#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <queue>
#include <string>

namespace Joy
{
    // 环形缓冲区，用于应用层的网络数据发送和接收缓冲区
    class CircleBuffer final
    {
    public:
        CircleBuffer(size_t chunkSize = kChunkSize);
        ~CircleBuffer();

    public:
        /**
         * @brief 获取环形缓冲区有效数据长度
         * 
         * @return size_t 
         */
        size_t GetBufferSize() const;
        
        /**
         * @brief 获取可写入位置指针
         * 
         * @return void* 
         */
        void* GetWritePtr() const;
        
        /**
         * @brief 获取当前可写入的缓冲区长度
         * 
         * @return size_t 
         */
        size_t GetCanWriteLength() const;

        /**
         * @brief 数据写入缓冲区
         * 
         * @param pData [In] 写入缓冲区的数据地址
         * @param dataSize [In] 写入的数据长度
         */
        void Write(const char* pData, size_t dataSize);
        
        /**
         * @brief 读取一个字节，转换为int8_t
         * 
         * @return int8_t
         */
        int8_t ReadInt8();

        /**
         * @brief 读取一个字节，转换为uint8_t
         * 
         * @return uint8_t
         */
        uint8_t ReadUInt8();

        /**
         * @brief 读取两个字节，转换为int16_t
         * 
         * @return int16_t 
         */
        int16_t ReadInt16();

        /**
         * @brief 读取两个字节，转换为uint16_t
         * 
         * @return uint16_t 
         */
        uint16_t ReadUInt16();

        /**
         * @brief 读取4个字节，转换为int32_t
         * 
         * @return int32_t 
         */
        int32_t ReadInt32();

        /**
         * @brief 读取4个字节，转换为uint32_t
         * 
         * @return uint32_t 
         */
        uint32_t ReadUInt32();

        /**
         * @brief 读取8字节，转换为int64_t
         * 
         * @return int64_t 
         */
        int64_t ReadInt64();

        /**
         * @brief 读取8字节，转换为uint64_t
         * 
         * @return uint64_t 
         */
        uint64_t ReadUInt64();
           
        /**
         * @brief 读取指定长度的缓冲区数据，拷贝到std::string字符串
         * 
         * @param str 
         * @param readLength 
         */
        void ReadString(std::string& str, size_t& readLength);

        /**
         * @brief 读取指定长度的缓冲区数据，拷贝到指针指向位置
         * 
         * @param pData [In, Out] 数据写入的位置
         * @param offset [In] 数据写入偏移
         * @param readLength [In, Out] 数据读取长度，In-预期读取长度，Out-实际读取长度(假如缓冲区数据不足读取)
         */
        void Read(char* pData, size_t offset, size_t& readLength);

        /**
         * @brief 抓取(当前缓冲块)全部数据(数据头索引不偏移，后续需配合Move操作)，用于发送子线程
         * 
         * @param pData 
         * @param dataSize 
         */
        void PeekToEnd(void** pData, size_t& dataSize);

        /**
         * @brief 移动数据头索引偏移，只允许向后移动。(配合Peek操作使用)
         * 
         * @param offset 
         */
        void MoveHeadTo(size_t offset);

    private:
        /**
         * @brief 工作buffer队列尾部添加新的缓冲块
         * 
         */
        void AddLast();
        /**
         * @brief 移除工作buffer队列头部的缓冲块
         * 
         */
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