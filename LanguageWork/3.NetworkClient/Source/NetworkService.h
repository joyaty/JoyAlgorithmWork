
#pragma once

#include <memory>
#include <string>
#include <thread>
#include "TCPSocket.h"
#include "CircleBuffer.h"

namespace Joy
{
    class TCPSocket;
    class CircleBuffer;
    class NetworkService
    {
    public:
        NetworkService();
        // 启动Service
        void Start(const std::string& inAddress, uint16_t inPort);
        // 停止Service
        void Stop();
        // 发送消息
        void SendMessage(const std::string& inMessage);

    private:
        void RunOnSendThread();

    private:
        // Socket连接
        TCPSocket m_TcpSocket{};
        // socket是否连接
        bool m_IsConnected{};

        // 发送缓冲区
        CircleBuffer m_SendBuffer;
        // 发送线程
        std::unique_ptr<std::thread> m_SendThread;
    };

}   // namespace Joy