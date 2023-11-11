
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/in_var.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>

#define SOCKET_ERROR (-1)

// 全局变量errno，用于捕获错误代码
extern int errno;
int GetErrorNo() { return errno; }

class TCPSocket;
class UDPSocket;
class SocketUtil;



// 封装网络协议地址结构体
class SocketAddress
{
    friend class TCPSocket;
    friend class UDPSocket;
public:
    SocketAddress(uint32_t inAddress, uint16_t inPort)
    {
        // 将sin_zero字段的内存置0，避免一些跨平台初始化差异造成的系统错误
        memset(GetSockAddrIn()->sin_zero, 0, sizeof(GetSockAddrIn()->sin_zero));
        // 设置协议族、网络端口和地址
        GetSockAddrIn()->sin_family = AF_INET;
        GetSockAddrIn()->sin_addr.s_addr = htonl(inAddress); // 主机字节序转化为网络字节序
        GetSockAddrIn()->sin_port = htonl(inPort);
    }
    
    explicit SocketAddress(const sockaddr& inSockAddr)
    {
        memcpy(&m_SockAddr, &inSockAddr, sizeof(inSockAddr));
    }
    
public:
    // 获取协议地址信息结构体的内存大小
    uint32_t GetSize() const { return sizeof(m_SockAddr); }
    
private:
    // sockaddr结构体重新解释为sockaddr_in结构体
    sockaddr_in* GetSockAddrIn()
    {
        return reinterpret_cast<sockaddr_in*>(&m_SockAddr);
    }
    
private:
    sockaddr m_SockAddr;
};

// 封装处理UDP协议的Socket
class UDPSocket
{
    friend class SocketUtil;
public:
    ~UDPSocket()
    {
        close(m_Socket);
    }
    
    int Bind(const SocketAddress& inBindAddr)
    {
        int error = bind(m_Socket, &inBindAddr.m_SockAddr, inBindAddr.GetSize());
        if(error != 0)
        {
            // TODO 收集Socket绑定到主机地址和端口异常信息
            return -1;
        }
        return 0;
    }
    
    int SendTo(const void* inBufferData, size_t inLen, const SocketAddress& inToAddr)
    {
        int byteSentCount = sendto(m_Socket, inBufferData, inLen, 0, &inToAddr.m_SockAddr, inToAddr.GetSize());
        
        if (byteSentCount >= 0)
        {
            return byteSentCount;
        }
        else
        {
            // TODO  收集发送数据异常错误
            return -1;
        }
    }
    
    int RecvFrom(void* inBufferData, int inLen, SocketAddress& outFromAddr)
    {
        socklen_t fromLength = outFromAddr.GetSize();
        int byteReadCount = recvfrom(m_Socket, inBufferData, inLen, 0, &outFromAddr.m_SockAddr, &fromLength);
        if (byteReadCount >= 0)
        {
            return byteReadCount;
        }
        else
        {
            // TODO 收集接收数据异常错误
            return -1;
        }
    }
    
    // 设置Socket是否非阻塞模式
    int SetNonBlockingMode(bool isShouldBeNonBlocking)
    {
        int flags = fcntl(m_Socket, F_GETFL, 0);
        flags = isShouldBeNonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
        int result = fcntl(m_Socket, F_SETFL, flags);
        if (result == SOCKET_ERROR)
        {
            // 处理错误编码
            return -1;
        }
        return 0;
    }
    
private:
    UDPSocket(int inSocket) : m_Socket(inSocket) {}
    
private:
    int m_Socket;
};
typedef std::shared_ptr<UDPSocket> UDPSocketPtr;

class TCPSocket
{
    
};


class SocketUtil
{
public:
    static UDPSocketPtr CreateUDPSocket()
    {
        int udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (udpSocket < 0)
        {
            // TODO
            return nullptr;
        }
        
        return UDPSocketPtr(new UDPSocket(udpSocket));
    }
};


int TestSocketFunc()
{
    int tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (tcpSocket < 0)
    {
        std::cout << "Invalid socket create!" << std::endl;
    }
    
    std::cout << "Create MacOS Socket Success." << std::endl;
    
    close(tcpSocket);
}
