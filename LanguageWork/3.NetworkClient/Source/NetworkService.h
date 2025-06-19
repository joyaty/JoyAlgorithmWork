
#pragma once

#include "CircleBuffer.h"
#include "Serialization/SerializationUtils.h"
#include "PacketParser.h"
#include "TCPSocket.h"
#include <string>
#include <thread>

namespace Joy
{
    class TCPSocket;
    class CircleBuffer;
    class NetworkService
    {
    public:
        NetworkService();

        /**
         * @brief 启动网络模块
         *
         * @param inAddress
         * @param inPort
         */
        void Start(const std::string& inAddress, uint16_t inPort);

        /**
         * @brief 停止网络模块
         *
         */
        void Stop();

        /**
         * @brief 帧循环
         *
         */
        void Update();

        /**
         * @brief 发送消息
         *
         * @param inMessage 消息字符串
         */
        void SendMessage(const std::string& inMessage);

        /**
         * @brief 是否连接
         *
         * @return true
         * @return false
         */
        bool IsConnected() const { return m_IsConnected; }

    private:
        /**
         * @brief 启动发送子线程
         *
         */
        void StartSend();

        /**
         * @brief 发送子线程执行入口
         *
         */
        void RunOnSendThread();

        /**
         * @brief 启动接收子线程
         *
         */
        void StartRecv();

        /**
         * @brief 接收子线程逻辑入口
         *
         */
        void RunOnRecvThread();

    private:
        // Socket连接
        TCPSocket m_TcpSocket{};
        // socket是否连接
        std::atomic<bool> m_IsConnected{};

        // 应用层发送缓冲区
        CircleBuffer m_SendBuffer;
        // 发送子线程
        std::thread m_SendThread;
        // 输出流
        OutputMemoryStream m_Stream;

        // 应用层接收缓冲区
        CircleBuffer m_RecvBuffer;
        // 接收子线程
        std::thread m_RecvThread;
        // 消息包解析器
        PacketParser m_Parser;
    };

}   // namespace Joy