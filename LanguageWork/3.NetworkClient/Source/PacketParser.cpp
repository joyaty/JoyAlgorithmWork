
#include "PacketParser.h"
#include "CircleBuffer.h"
#include <cstdio>
#include <mutex>
#include <string>

namespace Joy
{
    PacketParser::PacketParser()
        : m_eParseStep(EnumParseStep::kStepParseHeader)
        , m_nCurrentPackBodySize(0)
        , m_pBack(new std::queue<Packet>())
        , m_pFront(new std::queue<Packet>())
    {
    }

    void PacketParser::Parse(CircleBuffer& recvBuffer)
    {
        while (true)
        {
            if (m_eParseStep == EnumParseStep::kStepParseHeader)
            {   // 解析包头阶段
                if (recvBuffer.GetBufferSize() >= Packet::kHeaderSize)
                {   // 数据长度足够一个包头，尝试读取消息包
                    uint8_t checkCode1 = recvBuffer.ReadUInt8();
                    if (checkCode1 == 0x5A)
                    {
                        uint8_t checkCode2 = recvBuffer.ReadUInt8();
                        if (checkCode2 == 0xA5)
                        {
                            m_nCurrentPackBodySize = recvBuffer.ReadUInt32();
                            m_eParseStep           = EnumParseStep::kStepParseBody;   // 可以解析包体
                            if (m_nCurrentPackBodySize == 0)
                            {
                                printf("Warnning! MsgBodyLengh = 0, Ignore.");
                                m_eParseStep = EnumParseStep::kStepParseHeader;
                            }
                        }
                        else
                        {
                            // 包头格式校验2失败，丢弃2字节的数据
                            printf("Error! Parse CheckCode2, throw 2 byte data.");
                            continue;
                        }
                    }
                    else
                    {
                        // TODO 异常处理 包头格式校验1失败，1字节的数据被丢弃
                        printf("Error! Parse CheckCode1, throw 1 byte data.");
                        continue;
                    }
                }
                else
                {
                    // 数据不足，等待继续写入完全
                    break;
                }
            }
            if (m_eParseStep == EnumParseStep::kStepParseBody)
            {   // 解析包体阶段
                if (recvBuffer.GetBufferSize() >= m_nCurrentPackBodySize)
                {
                    std::string msg;   
                    recvBuffer.ReadString(msg, m_nCurrentPackBodySize);
                    {
                        std::lock_guard<std::mutex> lock_guard(m_Mutex);
                        GetBack().emplace(msg);   // 完整消息包体数据写入到后缓冲中
                    }
                    m_eParseStep = EnumParseStep::kStepParseHeader;   // 当前消息包读取完成，解析下一个消息包
                }
                else
                {
                    // 数据不足，等待继续写入完全
                    break;
                }
            }
        }
    }

    void PacketParser::Swap()
    {
        {   // 交换前后缓冲队列指针
            std::lock_guard<std::mutex> lock_guard(m_Mutex);
            if (!m_pBack->empty())
            {
                std::queue<Packet>* pTemp = m_pBack;
                m_pBack                   = m_pFront;
                m_pFront                  = pTemp;
            }
        }
    }
}   // namespace Joy