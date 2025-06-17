
#include "NetworkService.h"
#include "CircleBuffer.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
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

    // 网络消息发送线程执行
    void NetworkService::RunOnSendThread()
    {
        while (m_IsConnected)
        {
        }
    }


}   // namespace Joy