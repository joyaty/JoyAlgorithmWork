
#include "TCPSocket.h"
#include "SocketAddress.h"
#include <iostream>

namespace Joy
{
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
}

#pragma region 单元测试 - TCPSocket

#include <chrono>
#include <thread>

using Joy::SocketAddress;
using Joy::TCPSocket;

static void UnitTest_RunOnSubThread()
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
