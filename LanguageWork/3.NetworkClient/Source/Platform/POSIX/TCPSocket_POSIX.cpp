
// 兼容POSIX平台的TCPSocket操作封装实现

#include "SocketAddress.h"
#include "TCPSocket.h"
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>

// 错误码
extern int errno;
// 获取错误码
inline static int GetErrorCode()
{
    return errno;
}

namespace Joy
{
    void TCPSocket::NativeStartup()
    {
        // do nothing for linux.
    }
    
    void TCPSocket::NativeCleanup()
    {
        // do nothing for linux.
    }

    TCPSocket::TCPSocket()
        : m_Socket(-1)
    {
        // 创建Socket资源
        int ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ret == -1)
        {
            // 创建socket资源异常打印
            printf("Error: TCPSocket::TCPSocket, %d, \n", GetErrorCode());
            return;
        }
        m_Socket = ret;
    }

    TCPSocket::~TCPSocket()
    {
        if (m_Socket != -1)
        {
            close(m_Socket);
            m_Socket = -1;
        }
    }

    int TCPSocket::Connect(const SocketAddress& sockaddr) const
    {
        int ret = connect(m_Socket, &sockaddr.m_Address, static_cast<socklen_t>(sockaddr.GetSize()));
        if (ret != 0)
        {
            // if (GetErrorCode() == EINPROGRESS)
            // {   // 非阻塞模式，ret == -1, errorno == EINPROGRESS 表示正在连接中
            //     return 0;
            // }
            printf("Error: TCPSocket::Connect, %d \n", GetErrorCode());
        }
        return ret;
    }

    int TCPSocket::Send(const void* inSendBuffer, size_t inLen) const
    {
        ssize_t ret = send(m_Socket, inSendBuffer, inLen, 0);
        if (ret == -1)
        {
            printf("TCPSocket::Send, %d, \n", GetErrorCode());
        }
        return static_cast<int>(ret);
    }

    int TCPSocket::Recv(void* inRecvBuffer, size_t inLen) const
    {
        ssize_t ret = recv(m_Socket, inRecvBuffer, inLen, 0);
        if (ret == -1)
        {
            printf("TCPSocket::Recv, %d, \n", GetErrorCode());
        }
        return static_cast<int>(ret);
    }

    // 停止Socket
    void TCPSocket::Shutdown() const
    {
        int ret = shutdown(m_Socket, SHUT_RD);
        if (ret != 0)
        {
            printf("TCPSocket::Shutdown, %d \n", GetErrorCode());
        }
    }

    // 关闭Socket
    void TCPSocket::Close()
    {
        close(m_Socket);
        m_Socket = -1;
    }

    // 设置接收缓冲区大小
    void TCPSocket::SetRecvBufferSize(int inBufferSize) const
    {
        int ret = setsockopt(m_Socket, SOL_SOCKET, SO_RCVBUF, static_cast<const int*>(&inBufferSize), sizeof(inBufferSize));
        if (ret != 0)
        {
            printf("Error! TCPSocket::SetRecvBufferSize, %d \n", GetErrorCode());
        }
    }

    // 获取内核接收缓冲山大小
    int TCPSocket::GetRecvBufferSize() const
    {
        int       optValue    = 0;
        socklen_t optValueLen = sizeof(optValue);
        int       ret         = getsockopt(m_Socket, SOL_SOCKET, SO_RCVBUF, (int*)&optValue, &optValueLen);
        if (ret != 0)
        {
            printf("Error! TCPSocket::GetRecvBufferSize, %d \n", GetErrorCode());
            return 0;
        }
        return optValue;
    }

    // 设置发送缓冲区大小
    void TCPSocket::SetSendBufferSize(int inBufferSize) const
    {
        int ret = setsockopt(m_Socket, SOL_SOCKET, SO_SNDBUF, static_cast<const int*>(&inBufferSize), sizeof(inBufferSize));
        if (ret != 0)
        {
            printf("Error! TCPSocket::SetSendBufferSize, %d \n", GetErrorCode());
        }
    }

    // 获取内核发送缓冲山大小
    int TCPSocket::GetSendBufferSize() const
    {
        int       optValue    = 0;
        socklen_t optValueLen = sizeof(optValue);
        int       ret         = getsockopt(m_Socket, SOL_SOCKET, SO_SNDBUF, &optValue, &optValueLen);
        if (ret != 0)
        {
            printf("Error! TCPSocket::GetSendBufferSize, %d \n", GetErrorCode());
            return 0;
        }
        return optValue;
    }

    // 是否开启Nagle算法
    void TCPSocket::SetNoDelay(bool isNodelay) const
    {
        int noDelayTag = isNodelay ? 1 : 0;
        int ret        = setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, static_cast<const int*>(&noDelayTag), sizeof(noDelayTag));
        if (ret != 0)
        {
            printf("Error! TCPSocket::SetNoDelay, %d \n", GetErrorCode());
        }
    }

    // Nagle算法开启状态
    bool TCPSocket::IsNoDelay() const
    {
        int       isNodelay   = 0;
        socklen_t optValueLen = sizeof(int);
        int       ret         = getsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, static_cast<int*>(&isNodelay), &optValueLen);
        if (ret != 0)
        {
            printf("Error! TCPSocket::IsNoDelay, %d \n", GetErrorCode());
        }
        return isNodelay == 1;
    }

    // 设置socket为非阻塞模式
    void TCPSocket::SetNoBlockingMode(bool isShouldBeNonBlocking) const
    {
        int flags = fcntl(m_Socket, F_GETFL, 0);
        flags     = isShouldBeNonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
        int ret   = fcntl(m_Socket, F_SETFL, flags);
        if (ret != 0)
        {
            printf("Error! TCPSocket::SetNoBlockingMode, %d, \n", GetErrorCode());
        }
    }

}   // namespace Joy

#pragma region 单元测试 - TCPSocket

#include <chrono>
#include <iostream>
#include <thread>

using Joy::SocketAddress;
using Joy::TCPSocket;

void UnitTest_RunOnSubThread()
{
    SocketAddress address("192.168.17.65", 8089);
    TCPSocket     socket{};
    // socket.SetRecvBufferSize(10240);
    // socket.SetSendBufferSize(10240);
    // socket.SetNoDelay(true);
    // socket.SetNoBlockingMode(true);
    std::cout << "接收缓冲区大小:" << socket.GetRecvBufferSize() << std::endl;
    std::cout << "发送缓冲区大小:" << socket.GetSendBufferSize() << std::endl;
    std::cout << "Nagle算法状态:" << socket.IsNoDelay() << std::endl;
    socket.Connect(address);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    socket.Shutdown();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    socket.Close();
}

void UnitTest_TCPSocket()
{
    std::thread netThread(UnitTest_RunOnSubThread);
    std::cout << "启动网络线程" << std::endl;
    netThread.join();
    std::cout << "网络线程结束" << std::endl;
}

#pragma endregion