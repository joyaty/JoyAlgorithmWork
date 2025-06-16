
#include "SocketAddress.h"
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>

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

    SocketAddress::SocketAddress(const std::string& inAddress, uint16_t inPort)
    {
        sockaddr_in* pInetFormat     = GetSockAddrIn();
        pInetFormat->sin_family      = AF_INET;            // 规定IPv4地址族AF_INET
        pInetFormat->sin_port        = htons(inPort);      // 主机字节序转为网络字节序
        inet_pton(pInetFormat->sin_family, inAddress.c_str(), &pInetFormat->sin_addr);
        // 使用0初始化sockaddr_in.sin_zero字段(8byte)，避免未初始化字段导致一些系统错误。此字段对于Inet格式的IPv4地址无用。
        std::memset(pInetFormat->sin_zero, 0, sizeof(pInetFormat->sin_zero));
    }

    SocketAddress::SocketAddress(const sockaddr& inAddr)
    {
        memcpy(&m_Address, &inAddr, sizeof(inAddr));
    }
}   // namespace Joy