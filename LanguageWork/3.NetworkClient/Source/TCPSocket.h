
#pragma once

#include <cstddef>
#if _WIN32
#    include <WinSock2.h>
#endif

namespace Joy
{
    // 依赖前置声明 - SocketAddress
    class SocketAddress;

    // 封装TCPSocket操作的接口类
    class TCPSocket
    {
        // 使用类型别名兼容不同平台的Socket类型
#if __linux__
        using SocketType = int;
#elif _WIN32
        using SocketType = SOCKET;
#endif

    public:
        TCPSocket();
        ~TCPSocket();

    public:
        // 连接远端
        int Connect(const SocketAddress& address) const;
        // 发送数据(可能阻塞)
        int Send(const void* inSendBuffer, size_t inLen) const;
        // 接收数据(阻塞)
        int Recv(void* inRecvBuffer, size_t inLen) const;
        // 停止Socket
        void Shutdown() const;
        // 关闭Socket
        void Close();

    public:
        // 设置内核接收缓冲区大小
        void SetRecvBufferSize(int inBufferSize) const;
        // 获取内核接收缓冲山大小
        int GetRecvBufferSize() const;
        // 设置内核发送缓冲区大小
        void SetSendBufferSize(int inBufferSize) const;
        // 获取内核发送缓冲山大小
        int GetSendBufferSize() const;
        // 设置是否开启Nagle算法
        void SetNoDelay(bool isNodelay) const;
        // Nagle算法开启状态
        bool IsNoDelay() const;
        // 设置socket为非阻塞模式
        void SetNoBlockingMode(bool isShouldBeNonBlocking) const;

    private:
        SocketType m_Socket;
    };
}   // namespace Joy