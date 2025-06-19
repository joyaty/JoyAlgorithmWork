
#include "NetworkService.h"
#include "CircleBuffer.h"
#include "PacketParser.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include "nlohmann/json.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

namespace Joy
{
    NetworkService::NetworkService()
        : m_TcpSocket()
        , m_SendBuffer()
        , m_SendThread()
        , m_RecvBuffer()
        , m_RecvThread()
        , m_Parser()
        , m_IsConnected(false)
    {
    }

    void NetworkService::Start(const std::string& inAddress, uint16_t inPort)
    {
        SocketAddress addrInfo(inAddress, inPort);
        int           ret = m_TcpSocket.Connect(addrInfo);
        if (ret == 0)
        {
            m_IsConnected = true;
            // 启动发送线程
            StartSend();
            StartRecv();
        }
    }

    void NetworkService::Stop()
    {
        m_IsConnected = false;
        // 等待接收子线程和发送线程接收
        if (m_SendThread.joinable())
        {
            m_SendThread.join();
        }
        if (m_RecvThread.joinable())
        {
            m_RecvThread.join();
        }
        // socket关闭
        m_TcpSocket.Shutdown();
        m_TcpSocket.Close();
    }

    void NetworkService::Update()
    {
        while (!m_Parser.GetFront().empty())
        {
            PacketParser::Packet packet = m_Parser.GetFront().front();
            m_Parser.GetFront().pop();
            std::cout << "[Receive message] " << packet.msgBody << std::endl;
        }
        // 消息处理完毕，交换前后缓冲区
        m_Parser.Swap();
    }

    void NetworkService::SendMessage(const std::string& inMessage)
    {
        int                  size = static_cast<int>(inMessage.length());
        PacketParser::Packet packet(inMessage);
        m_Stream.Write(PacketParser::Packet::kCheckCode1);
        m_Stream.Write(PacketParser::Packet::kCheckCode2);
        m_Stream.Write(static_cast<uint32_t>(inMessage.length()));
        m_Stream.Write(inMessage.c_str(), inMessage.length());
        // 写入到发送缓冲区，准备发送
        m_SendBuffer.Write(m_Stream.GetBufferPtr(), m_Stream.GetLength());
        // 重置流后续复用
        m_Stream.Reset();
    }

    void NetworkService::StartSend()
    {
        m_SendThread = std::thread(&NetworkService::RunOnSendThread, this);
    }

    void NetworkService::RunOnSendThread()
    {
        while (true)
        {
            // TODO 添加信号量机制阻塞发送线程，主线程发送了消息(增加信号量)，从而激活发送线程执行发送(消耗信号量)，否则就一直阻塞，避免线程空转
            if (!m_IsConnected)
            {   // 网络连接断开，停止发送线程
                break;
            }
            while (m_SendBuffer.GetBufferSize() != 0)
            {   // 将发送缓冲区中数据全部发送出去(由于环形缓冲区性质，经过缓冲区结尾的消息，会被切分为两次Send！但是避免了一次数据拷贝)
                size_t nDataSize = 0;
                void*  pData{nullptr};
                m_SendBuffer.PeekToEnd(&pData, nDataSize);
                if (pData != nullptr && nDataSize > 0)
                {
                    int ret = m_TcpSocket.Send(pData, nDataSize);
                    if (ret < 0)
                    {
                        // TODO 异常处理
                    }
                    else
                    {   // ret表示被发送的数据长度，ret可能小于nDataSize(部分被发送)，必须使用ret来偏移应用层发送缓冲区的有效数据头偏移
                        m_SendBuffer.MoveHeadTo(static_cast<size_t>(ret));
                    }
                }
            }
            // 休眠10毫秒
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void NetworkService::StartRecv()
    {
        m_RecvThread = std::thread(&NetworkService::RunOnRecvThread, this);
    }

    void NetworkService::RunOnRecvThread()
    {
        uint8_t tempBuffer[8192];
        while (true)
        {
            if (!m_IsConnected)
            {   // 网络连接断开，停止发送线程
                break;
            }
            // 阻塞，等待接收到新的数据
            int ret = m_TcpSocket.Recv(tempBuffer, sizeof(tempBuffer));
            if (ret == 0)
            {   // 远端Socket断开
                m_IsConnected = false;
            }
            else if (ret > 0)
            {
                // 网络消息数据写入到接收缓冲区中
                m_RecvBuffer.Write(reinterpret_cast<char*>(tempBuffer), static_cast<size_t>(ret));
                // 读取缓冲区，解析消息包
                m_Parser.Parse(m_RecvBuffer);
            }
            else
            {
                // TODO 错误处理
            }
        }
    }
}   // namespace Joy
