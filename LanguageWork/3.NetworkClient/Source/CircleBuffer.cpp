
#include "CircleBuffer.h"
#include <chrono>
#include <cstddef>
#include <cstdint>
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

    void* CircleBuffer::GetWritePtr() const
    {
        return m_WorkBuffer.back() + m_TailIndex;
    }

    size_t CircleBuffer::GetCanWriteLength() const
    {
        return m_ChunkSize - m_TailIndex;
    }

    void CircleBuffer::Write(const char* pData, size_t dataSize)
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

    int8_t CircleBuffer::ReadInt8()
    {
        int8_t value{};
        size_t readSize = sizeof(int8_t);
        Read(reinterpret_cast<char*>(&value), 0, readSize);
        return value;
    }

    uint8_t CircleBuffer::ReadUInt8()
    {
        uint8_t value{};
        size_t  readSize = sizeof(uint8_t);
        Read(reinterpret_cast<char*>(&value), 0, readSize);
        return value;
    }

    int16_t CircleBuffer::ReadInt16()
    {
        int16_t value{};
        size_t  readSize = sizeof(int16_t);
        Read(reinterpret_cast<char*>(&value), 0, readSize);
        return value;
    }

    uint16_t CircleBuffer::ReadUInt16()
    {
        uint16_t value{};
        size_t   readSize = sizeof(uint16_t);
        Read(reinterpret_cast<char*>(&value), 0, readSize);
        return value;
    }

    int32_t CircleBuffer::ReadInt32()
    {
        int32_t value{};
        size_t  readSize = sizeof(int32_t);
        Read(reinterpret_cast<char*>(&value), 0, readSize);
        return value;
    }

    uint32_t CircleBuffer::ReadUInt32()
    {
        uint32_t value{};
        size_t   readSize = sizeof(uint32_t);
        Read(reinterpret_cast<char*>(&value), 0, readSize);
        return value;
    }

    int64_t CircleBuffer::ReadInt64()
    {
        int64_t value{};
        size_t  readSize = sizeof(int64_t);
        Read(reinterpret_cast<char*>(&value), 0, readSize);
        return value;
    }

    uint64_t CircleBuffer::ReadUInt64()
    {
        uint64_t value{};
        size_t   readSize = sizeof(uint64_t);
        Read(reinterpret_cast<char*>(&value), 0, readSize);
        return value;
    }

    void CircleBuffer::ReadString(std::string& str, size_t& readLength)
    {
        size_t tempHead   = m_HeadIndex;
        size_t bufferSize = GetBufferSize();   // 环形缓冲区中可读取的数据长度
        if (readLength > bufferSize)
        {   // 缓冲区中的可读取数据不足，修正到可读取长度
            readLength = bufferSize;
        }
        size_t alreadyCopySize = 0;
        while (alreadyCopySize < readLength)
        {
            size_t n                  = readLength - alreadyCopySize;   // 本次需要读取的长度
            size_t chunkRemainingSize = m_ChunkSize - tempHead;         // 当前缓冲块剩余可读取长度
            if (n <= chunkRemainingSize)
            {   // 当前缓冲块内可全部读完数据
                //std::memcpy(pData + offset + alreadyCopySize, m_WorkBuffer.front() + tempHead, n);
                str.insert(str.length(), m_WorkBuffer.front() + tempHead, n);
                tempHead += n;
                alreadyCopySize += n;
            }
            else
            {   // 当前缓冲块数据不足读取全部，读完剩余空间，移动到一下缓冲块
                //std::memcpy(pData + offset + alreadyCopySize, m_WorkBuffer.front() + tempHead, chunkRemainingSize);
                str.insert(str.length(), m_WorkBuffer.front() + tempHead, chunkRemainingSize);
                alreadyCopySize += chunkRemainingSize;
                RemoveFirst();
                tempHead = 0;
            }
        }
        // 数据全部读取完，更新数据头部索引
        m_HeadIndex = tempHead;


        str.insert(str.length(), m_WorkBuffer.front() + m_HeadIndex, GetBufferSize());
        
    }

    void CircleBuffer::Read(char* pData, size_t offset, size_t& readLength)
    {
        size_t tempHead   = m_HeadIndex;
        size_t bufferSize = GetBufferSize();   // 环形缓冲区中可读取的数据长度
        if (readLength > bufferSize)
        {   // 缓冲区中的可读取数据不足，修正到可读取长度
            readLength = bufferSize;
        }
        size_t alreadyCopySize = 0;
        while (alreadyCopySize < readLength)
        {
            size_t n                  = readLength - alreadyCopySize;   // 本次需要读取的长度
            size_t chunkRemainingSize = m_ChunkSize - tempHead;         // 当前缓冲块剩余可读取长度
            if (n <= chunkRemainingSize)
            {   // 当前缓冲块内可全部读完数据
                std::memcpy(pData + offset + alreadyCopySize, m_WorkBuffer.front() + tempHead, n);
                tempHead += n;
                alreadyCopySize += n;
            }
            else
            {   // 当前缓冲块数据不足读取全部，读完剩余空间，移动到一下缓冲块
                std::memcpy(pData + offset + alreadyCopySize, m_WorkBuffer.front() + tempHead, chunkRemainingSize);
                alreadyCopySize += chunkRemainingSize;
                RemoveFirst();
                tempHead = 0;
            }
        }
        // 数据全部读取完，更新数据头部索引
        m_HeadIndex = tempHead;
    }

    void CircleBuffer::PeekToEnd(void** pData, size_t& dataSize)
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
            *pData = (m_WorkBuffer.front() + tempHead);
            dataSize          = bufferSize;
            tempHead += dataSize;
        }
        else
        {   // 待读取的数据跨缓冲块，先读取当前缓冲块的数据，后续数据等待下次读取
            *pData   = m_WorkBuffer.front() + tempHead;
            dataSize = readRemainingSize;
            tempHead += dataSize;
        }
        // if (tempHead == m_ChunkSize)
        // {   // 头部缓冲块已全部读完，移除放入空闲等待复用
        //     RemoveFirst();
        //     tempHead = 0;
        // }
        // m_HeadIndex = tempHead;
    }

    void CircleBuffer::MoveHeadTo(size_t offset)
    {
        size_t tempHead = m_HeadIndex;
        tempHead += offset;
        while (tempHead > m_ChunkSize)
        {
            RemoveFirst();
            tempHead -= m_ChunkSize;
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


#pragma region 单元测试 - CircleBuffer读写

#include "Serialization/SerializationUtils.h"
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

using Joy::CircleBuffer;

// 单元测试 - 消息包格式
struct UnitTestPacket
{
    static const int kHeaderSize = 6;
    //
    static const int kFlag1Offset  = 0;
    static const int kFlag2Offset  = 1;
    static const int kLengthOffset = 2;

    // 包头部分，校验符 + 包体内容长度
    uint8_t checkCode1 = 0x5A;
    uint8_t checkCode2 = 0xA5;
    // 包体部分，字符串数据
    std::string msgBody;

    void Serialize(OutputMemoryStream& refStream)
    {
        refStream.Write(checkCode1);
        refStream.Write(checkCode2);
        refStream.Write(static_cast<int32_t>(msgBody.length()));
        refStream.Write(msgBody.c_str(), msgBody.length());
    }
};

static CircleBuffer       buffer(4);
static OutputMemoryStream stream;

static void RunOnReadThread()
{
    int    readStage        = 0;
    size_t packetBodyLength = 0;
    while (true)
    {
        if (readStage == 0)
        {   // 解析包头阶段
            if (buffer.GetBufferSize() >= UnitTestPacket::kHeaderSize)
            {   // 数据长度足够一个包头，尝试读取消息包
                uint8_t checkCode1 = buffer.ReadUInt8();
                if (checkCode1 == 0x5A)
                {
                    uint8_t checkCode2 = buffer.ReadUInt8();
                    if (checkCode2 == 0xA5)
                    {
                        packetBodyLength = buffer.ReadUInt32();
                        readStage        = 1;   // 可以解析包体
                    }
                    else
                    {
                        std::cout << "packetHead checkCode2 Failed, throw 2 byte" << std::endl;
                    }
                }
                else
                {   // 包头格式校验失败
                    std::cout << "packetHead checkCode1 Failed, throw 1 byte" << std::endl;
                }
            }
            else
            {
                // 数据不足，等待继续写入完全
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
        }
        if (readStage == 1)
        {   // 解析包体阶段
            if (buffer.GetBufferSize() >= packetBodyLength)
            {
                std::unique_ptr<char[]> pData = std::make_unique<char[]>(packetBodyLength + 1);   // 多申请一个空间，作为字符串结束标志位'\0';
                pData[packetBodyLength]       = '\0';
                buffer.Read(pData.get(), 0, packetBodyLength);

                std::string data(pData.get());
                std::cout << "Receive -> " << data << std::endl;
                readStage = 0;   // 当前消息包读取完成，解析下一个消息包
            }
            else
            {
                // 数据不足，等待继续写入完全
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

static void RunOnWriteThread()
{
    std::vector<std::string> msgs{"Hello", "World", "", "你好吗,呀哈哈哈哈哈哈啊哈哈哈哈哈哈哈哈哈哈"};
    for (size_t i = 0; i < msgs.size(); ++i)
    {
        UnitTestPacket packet{};
        packet.msgBody = msgs[i];
        packet.Serialize(stream);
        // 消息包数据序列化后拷贝到CircleBuffer，以备读取
        std::cout << "send -> " << msgs[i] << std::endl;
        buffer.Write(stream.GetBufferPtr(), stream.GetLength());
        // 拷贝完成后，重置以备复用
        stream.Reset();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void UnitTest_CircleBuffer()
{
    std::thread writeThread(&RunOnWriteThread);
    std::thread readThread(&RunOnReadThread);
    writeThread.join();
    readThread.join();
}

#pragma endregion