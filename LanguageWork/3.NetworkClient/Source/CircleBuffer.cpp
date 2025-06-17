
#include "CircleBuffer.h"
#include <cstdlib>
#include <cstring>
#include <mutex>

namespace Joy
{
    CircleBuffer::CircleBuffer(size_t chunkSize /* = kChunkSize */)
        : m_ChunkSize(chunkSize)
        , m_HeadIndex(0)
        , m_TailIndex(0)
    {
        // 预申请两个缓冲块，放到空闲缓冲块中
        for (int i = 0; i < 2; ++i)
        {
            char* pFreeBuffer = static_cast<char*>(std::malloc(m_ChunkSize));
            m_FreeBuffer.push(pFreeBuffer);
        }
        // 添加一个到工作缓冲中
        AddLast();
    }

    CircleBuffer::~CircleBuffer()
    {
        // 释放空闲缓冲块队列内存
        while (!m_FreeBuffer.empty())
        {
            char* pBuffer = m_FreeBuffer.front();
            std::free(pBuffer);
            m_FreeBuffer.pop();
        }
        // 释放工作缓冲块队列内存
        while (!m_WorkBuffer.empty())
        {
            char* pBuffer = m_WorkBuffer.front();
            std::free(pBuffer);
            m_WorkBuffer.pop();
        }
    }

    size_t CircleBuffer::GetBufferSize() const
    {
        size_t length = 0;
        if (!m_WorkBuffer.empty())
        {   // 完整缓冲块的个数 * 缓冲块的大小 + 数据段头尾偏移
            length = (m_WorkBuffer.size() - 1) * m_ChunkSize + m_TailIndex - m_HeadIndex;
        }
        return length;
    }

    void CircleBuffer::Write(char* pData, size_t dataSize)
    {
        size_t tempTail        = m_TailIndex;
        size_t alreadyCopySize = 0;
        while (alreadyCopySize < dataSize)
        {
            if (tempTail == m_ChunkSize)
            {   // CircularBuffer已写满，添加一个新的缓冲块
                AddLast();
                tempTail = 0;
            }
            size_t writeLen     = dataSize - alreadyCopySize;   // 需要写入的数据长度
            size_t remainingLen = m_ChunkSize - tempTail;       // 工作缓冲区剩余空间大小
            if (remainingLen >= writeLen)
            {   // 剩余缓冲区足够写入
                std::memcpy(m_WorkBuffer.back() + tempTail, pData + alreadyCopySize, writeLen);
                tempTail += writeLen;
                alreadyCopySize += writeLen;
            }
            else
            {   // 当前工作缓冲区剩余空间不足，写入剩余空间长度的数据，下次循环新增缓冲块，写入剩余数据
                std::memcpy(m_WorkBuffer.back() + tempTail, pData + alreadyCopySize, remainingLen);
                tempTail += remainingLen;
                alreadyCopySize += remainingLen;
            }
        }
        // 数据完全写入完成，更新TailIndex，避免读取线程在没写完时就能读取
        m_TailIndex = tempTail;
    }

    void CircleBuffer::Read(char* pData, size_t& dataSize)
    {
        size_t tempHead   = m_HeadIndex;
        size_t bufferSize = GetBufferSize();   // 待读取的数据长度
        if (bufferSize == 0)
        {   // 无数据
            pData    = nullptr;
            dataSize = 0;
            return;
        }
        size_t readRemainingSize = m_ChunkSize - tempHead;   // 当前缓冲块剩余可读取空间
        if (bufferSize <= readRemainingSize)
        {   // 数据块未跨缓冲块，全部读取
            pData    = m_WorkBuffer.front() + tempHead;
            dataSize = bufferSize;
            tempHead += dataSize;
        }
        else
        {   // 待读取的数据跨缓冲块，先读取当前缓冲块的数据，后续数据等待下次读取
            pData    = m_WorkBuffer.front() + tempHead;
            dataSize = readRemainingSize;
            tempHead += dataSize;
        }
        if (tempHead == m_ChunkSize)
        {   // 头部缓冲块已全部读完，移除放入空闲等待复用
            RemoveFirst();
            tempHead = 0;
        }
        m_HeadIndex = tempHead;
    }

    void CircleBuffer::AddLast()
    {
        bool hasFreeBuffer = false;
        {
            std::lock_guard<std::mutex> lock_guard(m_Mutex);
            if (!m_FreeBuffer.empty())
            {   // 有空闲缓冲块，复用
                m_WorkBuffer.push(m_FreeBuffer.front());
                m_FreeBuffer.pop();
                hasFreeBuffer = true;
            }
        }
        if (!hasFreeBuffer)
        {   // 无空闲的缓冲块，只能分配新的缓冲块
            char*                       pNewBuffer = static_cast<char*>(std::malloc(m_ChunkSize));
            std::lock_guard<std::mutex> lock_guard(m_Mutex);
            m_WorkBuffer.push(pNewBuffer);
        }
    }

    void CircleBuffer::RemoveFirst()
    {
        std::lock_guard<std::mutex> lock_guard(m_Mutex);
        // 释放工作缓冲区头部空闲数据块，填充到空闲缓冲区队尾
        m_FreeBuffer.push(m_WorkBuffer.front());
        m_WorkBuffer.pop();
    }
}   // namespace Joy