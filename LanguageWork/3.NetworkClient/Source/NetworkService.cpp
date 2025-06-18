
#include "NetworkService.h"
#include "CircleBuffer.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include <chrono>
#include <cstdint>
#include <memory>
#include <thread>

namespace Joy
{
    NetworkService::NetworkService()
        : m_TcpSocket()
        , m_SendBuffer()
        , m_SendThread(nullptr)
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
            m_SendThread = std::make_unique<std::thread>(&NetworkService::RunOnSendThread, this);
        }
    }

    void NetworkService::Stop()
    {

    }

    void NetworkService::SendMessage(const std::string& inMessage)
    {
        int size = static_cast<int>(inMessage.length());

        // m_SendBuffer.Write(char *pData, size_t dataSize)
    }

    // 网络消息发送线程执行
    void NetworkService::RunOnSendThread()
    {
        // while (m_IsConnected)
        // {
        //     if (m_SendBuffer.GetBufferSize() != 0)
        //     {
        //         char* pData = nullptr;
        //         size_t nDataSize = 0;
        //         m_SendBuffer.Read(pData, nDataSize);
        //     }
        //     // 休眠10毫秒
        //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
        // }
    }


}   // namespace Joy

