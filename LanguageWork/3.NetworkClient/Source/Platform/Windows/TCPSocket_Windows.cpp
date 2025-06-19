
#include "SocketAddress.h"
#include "TCPSocket.h"
#include <iostream>

namespace Joy
{
    void TCPSocket::NativeStartup()
    {
        WSAData wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "Failture to init WSA" << std::endl;
            return;
        }
    }

    void TCPSocket::NativeCleanup()
    {
        WSACleanup();
    }

    TCPSocket::TCPSocket()
        : m_Socket(INVALID_SOCKET)
    {
        int ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ret == INVALID_SOCKET)
        {
            std::cerr << "" << WSAGetLastError() << std::endl;
            return;
        }
        m_Socket = ret;
    }

    TCPSocket::~TCPSocket()
    {
        if (m_Socket != INVALID_SOCKET)
        {
            closesocket(m_Socket);
            m_Socket = INVALID_SOCKET;
        }
    }

    // 连接远端
    int TCPSocket::Connect(const SocketAddress& address) const
    {
        int ret = connect(m_Socket, &address.m_Address, address.GetSize());
        if (ret == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAEWOULDBLOCK)
            {
                return 0;
            }
            std::cerr << "Error: TCPSocket::Connect, " << WSAGetLastError() << std::endl;
        }
        return ret;
    }

    // 发送数据(可能阻塞)
    int TCPSocket::Send(const void* inSendBuffer, size_t inLen) const
    {
        int ret = send(m_Socket, static_cast<const char*>(inSendBuffer), inLen, 0);
        if (ret == SOCKET_ERROR)
        {
            std::cerr << "Error: TCPSocket::Send, " << WSAGetLastError() << std::endl;
        }
        return ret;
    }

    // 接收数据(阻塞)
    int TCPSocket::Recv(void* inRecvBuffer, size_t inLen) const
    {
        int ret = recv(m_Socket, static_cast<char*>(inRecvBuffer), inLen, 0);
        if (ret == SOCKET_ERROR)
        {
            std::cerr << "Error: TCPSocket::Recv, " << WSAGetLastError() << std::endl;
        }
        return ret;
    }

    // 停止Socket
    void TCPSocket::Shutdown() const
    {
        int ret = shutdown(m_Socket, SD_SEND);
        if (ret == SOCKET_ERROR)
        {
            std::cerr << "Error: TCPSocket::Shutdown, " << WSAGetLastError() << std::endl;
        }
    }

    // 关闭Socket
    void TCPSocket::Close()
    {
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }

    // 设置内核接收缓冲区大小
    void TCPSocket::SetRecvBufferSize(int inBufferSize) const
    {
        int ret = setsockopt(m_Socket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char*>(&inBufferSize), sizeof(inBufferSize));
        if (ret == SOCKET_ERROR)
        {
            std::cerr << "Error! TCPSocket::SetRecvBufferSize, " << WSAGetLastError() << std::endl;
        }
    }

    // 获取内核接收缓冲区大小
    int TCPSocket::GetRecvBufferSize() const
    {
        int optValue    = 0;
        int optValueLen = sizeof(optValue);
        int ret         = getsockopt(m_Socket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char*>(&optValue), &optValueLen);
        if (ret == SOCKET_ERROR)
        {
            std::cerr << "Error! TCPSocket::GetRecvBufferSize, " << WSAGetLastError() << std::endl;
            return 0;
        }
        return optValue;
    }

    // 设置内核发送缓冲区大小
    void TCPSocket::SetSendBufferSize(int inBufferSize) const
    {
        int ret = setsockopt(m_Socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char*>(&inBufferSize), sizeof(inBufferSize));
        if (ret == SOCKET_ERROR)
        {
            std::cerr << "Error! TCPSocket::SetSendBufferSize, " << WSAGetLastError() << std::endl;
        }
    }

    // 获取内核发送缓冲区大小
    int TCPSocket::GetSendBufferSize() const
    {
        int optValue    = 0;
        int optValueLen = sizeof(optValue);
        int ret         = getsockopt(m_Socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char*>(&optValue), &optValueLen);
        if (ret == SOCKET_ERROR)
        {
            std::cerr << "Error! TCPSocket::GetSendBufferSize, " << WSAGetLastError() << std::endl;
            return 0;
        }
        return optValue;
    }

    // 设置是否开启Nagle算法
    void TCPSocket::SetNoDelay(bool isNodelay) const
    {
        int noDelayTag = isNodelay ? 1 : 0;
        int ret        = setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&noDelayTag), sizeof(noDelayTag));
        if (ret != 0)
        {
            std::cerr << "Error! TCPSocket::SetNoDelay, " << WSAGetLastError() << std::endl;
        }
    }

    // Nagle算法开启状态
    bool TCPSocket::IsNoDelay() const
    {
        int isNodelay   = 0;
        int optValueLen = sizeof(int);
        int ret         = getsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&isNodelay), &optValueLen);
        if (ret != 0)
        {
            std::cerr << "Error! TCPSocket::IsNoDelay, " << WSAGetLastError() << std::endl;
        }
        return isNodelay == 1;
    }

    // 设置socket为非阻塞模式
    void TCPSocket::SetNoBlockingMode(bool isShouldBeNonBlocking) const
    {
        u_long arg = isShouldBeNonBlocking ? 1 : 0;
        int    ret = ioctlsocket(m_Socket, FIONBIO, &arg);
        if (ret == SOCKET_ERROR)
        {
            std::cerr << "Error! TCPSocket::SetNoBlockingMode, " << WSAGetLastError() << std::endl;
        }
    }
}   // namespace Joy

#pragma region 单元测试 - TCPSocket

#include <chrono>
#include <thread>

using Joy::SocketAddress;
using Joy::TCPSocket;

static void UnitTest_RunOnSubThread()
{
    SocketAddress address("192.168.17.65", 8089);
    TCPSocket     socket{};
    socket.SetNoBlockingMode(true);
    socket.Connect(address);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    socket.Shutdown();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    socket.Close();
}

void UnitTest_TCPSocket()
{
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Failture to init WSA" << std::endl;
        return;
    }

    std::thread netThread(UnitTest_RunOnSubThread);
    std::cout << "NetThread Start: " << netThread.get_id() << std::endl;
    netThread.join();
    std::cout << "NetThread End" << std::endl;

    WSACleanup();
}

#pragma endregion
