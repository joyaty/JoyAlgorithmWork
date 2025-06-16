
#include <cstdint>
#include <memory>
#include <string>
// socket相关的头文件，不同操作系统平台有不同的库
#if __linux__
#    include <netinet/in.h>
#    include <sys/socket.h>
#elif _WIN32
#    include <WinSock2.h>
#endif

namespace Joy
{
    // Socket地址的封装
    class SocketAddress
    {
    public:
        friend class TCPSocket;

    public:
        // 默认构造函数
        SocketAddress() = default;
        // 从IP地址(32位int类型描述)和端口构造
        SocketAddress(uint32_t inAddress, uint16_t inPort);
        // 从IP地址(string字符串描述)和端口构造
        SocketAddress(const std::string& inAddress, uint16_t inPort);
        // 从sockaddr结构体构造
        explicit SocketAddress(const sockaddr& inSockAddr);

    public:
        int GetSize() const { return sizeof(m_Address); }

    private:
        // 将通用的socket地址结构重新解释为IPv4的socket地址结构，2byte的address family，2byte->port，4byte->ipv4 address.
        sockaddr_in* GetSockAddrIn()
        {
            return reinterpret_cast<sockaddr_in*>(&m_Address);
        }

    private:
        // 通用的socket地址信息结构，2字节的地址类型，14字节数组的地址信息
        sockaddr m_Address;
    };

    // 定义类型别名
    using SocketAddressPtr = std::shared_ptr<SocketAddress>;

}   // namespace Joy