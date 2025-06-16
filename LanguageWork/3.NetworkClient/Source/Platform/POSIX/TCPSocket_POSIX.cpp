
// 兼容POSIX平台的TCPSocket操作封装实现

#include "SocketAddress.h"
#include "TCPSocket.h"
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <netdb.h>
#include <netinet/in.h>
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
    TCPSocket::TCPSocket()
        : m_Socket(-1)
    {
        // 创建Socket资源
        int ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ret == -1)
        {
            // 创建socket资源异常打印
            printf("Error: TCPSocket::TCPSocket, %d, %s \n", GetErrorCode(), gai_strerror(GetErrorCode()));
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

    int TCPSocket::Connect(const SocketAddress& sockaddr)
    {
        int ret = connect(m_Socket, &sockaddr.m_Address, static_cast<socklen_t>(sockaddr.GetSize()));
        if (ret != 0)
        {
            printf("Error: TCPSocket::Connect, %d, %s \n", GetErrorCode(), gai_strerror(GetErrorCode()));
        }
        return ret;
    }

    int TCPSocket::Send(const void* inSendBuffer, size_t inLen)
    {
        ssize_t ret = send(m_Socket, inSendBuffer, inLen, 0);
        if (ret == -1)
        {
            printf("TCPSocket::Send, %d, %s \n", GetErrorCode(), gai_strerror(GetErrorCode()));
        }
        return static_cast<int>(ret);
    }

    int TCPSocket::Recv(void* inRecvBuffer, size_t inLen)
    {
        ssize_t ret = recv(m_Socket, inRecvBuffer, inLen, 0);
        if (ret == -1)
        {
            printf("TCPSocket::Recv, %d, %s \n", GetErrorCode(), gai_strerror(GetErrorCode()));
        }
        return static_cast<int>(ret);
    }

    // 停止Socket
    void TCPSocket::Shutdown()
    {
        int ret = shutdown(m_Socket, SHUT_RD);
        if (ret == -1)
        {
            printf("TCPSocket::Shutdown, %d, %s \n", GetErrorCode(), gai_strerror(GetErrorCode()));
        }
    }

    // 关闭Socket
    void TCPSocket::Close()
    {
        close(m_Socket);
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