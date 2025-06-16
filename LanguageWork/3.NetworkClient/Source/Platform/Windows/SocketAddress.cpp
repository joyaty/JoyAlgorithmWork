
#include "SocketAddress.h"
#include <WS2tcpip.h>

namespace Joy
{
    SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort)
    {
        sockaddr_in* pInetFormat     = GetSockAddrIn();
        pInetFormat->sin_family      = AF_INET;            // 规定IPv4地址族AF_INET
        pInetFormat->sin_port        = htons(inPort);      // 主机字节序转为网络字节序
        pInetFormat->sin_addr.s_addr = htonl(inAddress);   // 主机字节序转为网络字节序
        // 使用0初始化sockaddr_in.sin_zero字段(8byte)，避免未初始化字段导致一些系统错误。此字段对于Inet格式的IPv4地址无用。
        std::memset(pInetFormat->sin_zero, 0, sizeof(pInetFormat->sin_zero));
    }

    // 从IP地址(string字符串描述)和端口构造
    SocketAddress::SocketAddress(const std::string& inAddress, uint16_t inPort)
    {
        sockaddr_in* pInetFormat = GetSockAddrIn();
        pInetFormat->sin_family  = AF_INET;         // 规定IPv4地址族AF_INET
        pInetFormat->sin_port    = htons(inPort);   // 主机字节序转为网络字节序
#if UNICODE
        std::wstring wAddress(inAddress.cbegin(), inAddress.cend());   // 转为宽字符
        InetPton(pInetFormat->sin_family, wAddress.c_str(), &pInetFormat->sin_addr);
#else
        InetPton(pInetFormat->sin_family, inAddress.c_str(), &pInetFormat->sin_addr);
#endif
        // 使用0初始化sockaddr_in.sin_zero字段(8byte)，避免未初始化字段导致一些系统错误。此字段对于Inet格式的IPv4地址无用。
        std::memset(pInetFormat->sin_zero, 0, sizeof(pInetFormat->sin_zero));
    }

    // 从sockaddr结构体构造
    SocketAddress::SocketAddress(const sockaddr& inSockAddr)
    {
        memcpy(&m_Address, &inSockAddr, sizeof(inSockAddr));
    }
}   // namespace Joy
