
#include "NetworkService.h"
#include "CircleBuffer.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include <chrono>
#include <cstdint>
#include <thread>

namespace Joy
{
    NetworkService::NetworkService()
        : m_TcpSocket()
        , m_SendBuffer()
        , m_SendThread()
        , m_RecvBuffer()
        , m_RecvThread()
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

    void NetworkService::SendMessage(const std::string& inMessage)
    {
        int size = static_cast<int>(inMessage.length());

        // m_SendBuffer.Write(char *pData, size_t dataSize)
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
                char*  pData     = nullptr;
                size_t nDataSize = 0;
                m_SendBuffer.ReadToEnd(pData, nDataSize);
                if (pData != nullptr && nDataSize > 0)
                {
                    int ret = m_TcpSocket.Send(pData, nDataSize);
                    if (ret < 0)
                    {
                        // TODO 异常处理
                    }
                    else if (ret < nDataSize)
                    {   // 数据被部分发送，需要回移数据游标
                        // m_SendBuffer.MoveHeadIndex(ret - static_cast<int>(nDataSize));
                    }
                }
            }
            // 休眠10毫秒
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void NetworkService::StartRecv()
    {
        m_RecvThread = std::thread(&NetworkService::RunOnSendThread, this);
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

            // m_RecvBuffer
            int ret = m_TcpSocket.Recv(tempBuffer, sizeof(tempBuffer));
            if (ret == 0)
            {   // 远端Socket断开
                m_IsConnected = false;
            }
            else if (ret > 0)
            {
                m_RecvBuffer.Write(reinterpret_cast<char*>(tempBuffer), static_cast<size_t>(ret));
            }
            else
            {
                // TODO 错误处理
            }
        }
    }

}   // namespace Joy
