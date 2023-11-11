
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <winnt.h>
#if !defined(WIN32)
	#include <fcntl.h>
#endif
#include <cerrno>
#include <iostream>
#include <vector>

// 数据缓冲区大小
#define GOOD_SEGMENT_SIZE 512

// 链接windows系统的socket2支持库，当前系统版本为2.2
// #pragma comment(lib, "ws2_32.lib") // CMake中配置链接库

static bool g_IsGameRunning;

class UDPSocket;
typedef std::shared_ptr<UDPSocket> UDPSocketPtr;
class TCPSocket;
typedef std::shared_ptr<TCPSocket> TCPSocketPtr;
class SocketUtil;

class SocketAddress
{
	friend class UDPSocket;
	friend class TCPSocket;
public:
	SocketAddress() = default;

	SocketAddress(uint32_t inAddress, uint16_t inPort)
	{
		// 使用0将sin_zero全部置零，避免一些未初始化字段的系统错误
		memset(GetSockAddrIn()->sin_zero, 0, sizeof(GetSockAddrIn()->sin_zero));
		// 设置协议族，网络端口和地址
		GetSockAddrIn()->sin_family = AF_INET;
		GetSockAddrIn()->sin_port = htonl(inPort); // 主机字节序转为网络字节序
		GetSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
	}

	explicit SocketAddress(const sockaddr& inSockAddr)
	{
		memcpy(&m_SockAddr, &inSockAddr, sizeof(inSockAddr));
	}

	size_t GetSize() const { return sizeof(m_SockAddr); }

private:
	sockaddr_in* GetSockAddrIn()
	{
		// sockaddr结构体重新解释为sockaddr_in，sockaddr_in结构体长度与sockaddr结构体长度一致
		return reinterpret_cast<sockaddr_in*>(&m_SockAddr);
	}

private:
	sockaddr m_SockAddr;
};

typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

class SocketAddressFactory
{
public:
	static SocketAddressPtr CreateIPv4FromString(const std::string& inIPString)
	{
		// 是否包含端口号
		auto pos = inIPString.find_last_of(':');
		std::string host, service;
		if (pos != std::string::npos)
		{
			// 分割IP地址和端口
			host = inIPString.substr(0, pos);
			service = inIPString.substr(pos + 1);
		}
		else
		{
			// 使用默认端口
			host = inIPString;
			service = "0";
		}
		addrinfo hint;
		// hint的内存全部置零，避免跨平台初始化区别导致的一些问题。
		memset(&hint, 0, sizeof(hint));
		hint.ai_family = AF_INET;
		// 通过getaddrinfo函数，请求DNS服务器将主机名解析为IP地址，hint作为一个提示，用于DNS服务器返回我们需要格式的信息
		addrinfo* result;
		int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
		if (error != 0 && result != nullptr)
		{
			// 请求主机名对应的IP发生错误，清理addrinfo
			freeaddrinfo(result);
			return nullptr;
		}
		// addrinfo是一个链表结果，因为一台主机可能会有多个IP地址，可以遍历链表找到需要的sockaddr
		while (!result->ai_addr && result->ai_next)
		{
			result = result->ai_next;
		}
		if (!result->ai_addr)
		{
			// sockaddr为空，异常清理
			freeaddrinfo(result);
			return nullptr;
		}
		SocketAddressPtr toRet = std::make_shared<SocketAddress>(*result->ai_addr);
		freeaddrinfo(result);
		return toRet;
	}
};

/// <summary>
/// 封装UDPSocket，使用UDP发送和接收数据包
/// </summary>
class UDPSocket
{
	friend class SocketUtil;
public:
	~UDPSocket()
	{
		closesocket(m_Socket);
	}

	/// <summary>
	/// Socket绑定到主机的网络IP和端口上
	/// </summary>
	/// <param name="inBindAddress"></param>
	/// <returns></returns>
	int Bind(const SocketAddress& inBindAddress)
	{
		int err = bind(m_Socket, &inBindAddress.m_SockAddr, inBindAddress.GetSize());
		if (err != NULL)
		{
			// 异常处理
		}
		return NO_ERROR;
	}

	/// <summary>
	/// 发送数据
	/// </summary>
	/// <param name="inData">数据缓冲区</param>
	/// <param name="inLen">数据缓冲区长度</param>
	/// <param name="inTo">目的地址信息，socket将数据发送到目的地址</param>
	/// <returns></returns>
	int SendTo(const void* inData, int inLen, const SocketAddress& inTo)
	{
		int byteSentCount = sendto(m_Socket, static_cast<const char*>(inData), inLen, 0, &inTo.m_SockAddr, inTo.GetSize());
		if (byteSentCount >= 0)
		{
			return byteSentCount;
		}
		else
		{
			// TODO 返回异常错误，错误代码负数
			return -1;
		}
	}

	/// <summary>
	/// 接收数据
	/// </summary>
	/// <param name="inBuffer">数据缓冲区</param>
	/// <param name="inLen">数据缓存区长度</param>
	/// <param name="outFrom">数据源地址信息，recvfrom函数会填充该信息</param>
	/// <returns></returns>
	int ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom)
	{
		int fromLength = outFrom.GetSize();
		int readByteCount = recvfrom(m_Socket, static_cast<char*>(inBuffer), inLen, 0, &outFrom.m_SockAddr, &fromLength);
		if (readByteCount >= 0)
		{
			return readByteCount;
		}
		else
		{
			// TODO 返回异常错误，错误代码负数
			return -1;
		}
	}

	/// <summary>
	/// 设置Socket是否为非阻塞模式
	/// </summary>
	/// <param name="isShouldBeNonBlocking"></param>
	/// <returns></returns>
	int SetNonBlockingMode(bool isShouldBeNonBlocking)
	{
#if WIN32
		u_long arg = isShouldBeNonBlocking ? 1 : 0;
		int result = ioctlsocket(m_Socket, FIONBIO, &arg);
#else
		int flags = fcntl(m_Socket, F_GETFL, 0);
		flags = isShouldBeNonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
		int result = fcntl(m_Socket, F_SETFL, flags);
#endif
		if (result == SOCKET_ERROR)
		{
			// TODO 处理非阻塞模式设置异常错误。
			return -1;
		}
		return 0;
	}

private:
	UDPSocket(SOCKET inSocket)
		: m_Socket(inSocket)
	{
	}

private:
	SOCKET m_Socket;
};

enum class EnumSocketAddressFamily
{
	INET = AF_INET,
	INET6 = AF_INET6,
};

class TCPSocket
{
	friend class SocketUtil;
public:
	~TCPSocket()
	{
		closesocket(m_Socket);
	}

	/// <summary>
	/// 客户端发起服务器连接，第一次握手
	/// </summary>
	/// <param name="inAddress"></param>
	/// <returns></returns>
	int Connect(const SocketAddress& inAddress)
	{
		int error = connect(m_Socket, &inAddress.m_SockAddr, inAddress.GetSize());
		if (error < 0)
		{
			// TODO 收集错误信息
			return -1;
		}
		return NO_ERROR;
	}

	/// <summary>
	/// 服务器创建和绑定Socket后，开启监听模式，接收来自客户端的请求
	/// </summary>
	/// <param name="inBackLog">允许的最大链接数，超过最大链接数的请求都会被丢弃</param>
	/// <returns></returns>
	int Listen(int inBackLog = 32)
	{
		int error = listen(m_Socket, inBackLog);
		if (error < 0)
		{
			// TODO 收集错误信息
			return -1;
		}
		return NO_ERROR;
	}

	/// <summary>
	/// 监听的Socket接收客户端第一次请求，生成一个新的Socket，用于接收处理后续该客户端的消息
	/// </summary>
	/// <param name="inFromAddress"></param>
	/// <returns></returns>
	std::shared_ptr<TCPSocket> Accept(SocketAddress& inFromAddress)
	{
		int iAddrLen = inFromAddress.GetSize();
		SOCKET newSocket = accept(m_Socket, &inFromAddress.m_SockAddr, &iAddrLen);
		if (newSocket != INVALID_SOCKET)
		{
			return TCPSocketPtr(new TCPSocket(newSocket));
		}
		else
		{
			// TODO 客户端链接请求接收错误处理
			return nullptr;
		}
	}

	/// <summary>
	/// 将Socket绑定到主机地址和端口上，用于接收和发送数据
	/// </summary>
	/// <param name="inToAddress"></param>
	/// <returns></returns>
	int Bind(const SocketAddress& inToAddress)
	{
		int error = bind(m_Socket, &inToAddress.m_SockAddr, inToAddress.GetSize());
		if (error != 0)
		{
			// TODO 收集Socket绑定到主机地址和端口错误
			return -1;
		}
		return NO_ERROR;
	}
	
	/// <summary>
	/// 发送数据
	/// </summary>
	/// <param name="inData">数据缓冲区</param>
	/// <param name="inLen">缓冲区长度</param>
	/// <returns>发送的数据字节数</returns>
	int Send(const void* inData, int inLen)
	{
		int byteSentCount = send(m_Socket, static_cast<const char*>(inData), inLen, 0);
		if (byteSentCount < 0)
		{
			// TODO 收集发送数据错误信息
			return -1;
		}
		return byteSentCount;
	}

	/// <summary>
	/// 接收数据
	/// </summary>
	/// <param name="inBuffer"></param>
	/// <param name="inLen"></param>
	/// <returns></returns>
	int Receive(void* inBuffer, int inLen)
	{
		int byteRecvCount = recv(m_Socket, static_cast<char*>(inBuffer), inLen, 0);
		if (byteRecvCount < 0)
		{
			// TODO 收集接收数据错误信息
			return -1;
		}
		return byteRecvCount;
	}

private:
	TCPSocket(SOCKET inSocket) : m_Socket(inSocket) {}

private:
	SOCKET m_Socket;
};

class SocketUtil
{
public:
	/// <summary>
	/// 向操作系统初始化一个UDPSocket
	/// </summary>
	/// <param name="inFamily"></param>
	/// <returns></returns>
	static UDPSocketPtr CreateUDPSocket(EnumSocketAddressFamily inFamily)
	{
		SOCKET udpSocket = socket(static_cast<int>(inFamily), SOCK_DGRAM, IPPROTO::IPPROTO_UDP);
		if (udpSocket != INVALID_SOCKET)
		{
			return UDPSocketPtr(new UDPSocket(udpSocket));
		}
		else
		{
			// 创建Socket错误
			return nullptr;
		}
	}


	/// <summary>
	/// 向操作系统申请一个TCPSocket
	/// </summary>
	/// <param name="inFamily"></param>
	/// <returns></returns>
	static TCPSocketPtr CreateTCPSocket(EnumSocketAddressFamily inFamily)
	{
		SOCKET tcpSocket = socket(static_cast<int>(inFamily), SOCK_STREAM, IPPROTO::IPPROTO_TCP);
		if (tcpSocket != INVALID_SOCKET)
		{
			return TCPSocketPtr(new TCPSocket(tcpSocket));
		}
		else
		{
			return nullptr;
		}
	}

	/// <summary>
	/// 输出错误信息
	/// </summary>
	/// <param name="wstrHint"></param>
	static void ReportError(const std::wstring& wstrHint)
	{
		// TODO 报告错误信息，打印和写入日志
	}

	/// <summary>
	/// 获取错误代码
	/// </summary>
	/// <returns></returns>
	static int GetLastError()
	{
		return WSAGetLastError();
	}

	/// <summary>
	/// 将Socket填充到fd_set容器中
	/// </summary>
	/// <param name="outSet">fd_set容器</param>
	/// <param name="inSockets"></param>
	/// <returns></returns>
	static fd_set* FillSetFromVector(fd_set& outSet, const std::vector<TCPSocketPtr>* inSockets)
	{
		if (inSockets)
		{
			FD_ZERO(&outSet);
			for (const TCPSocketPtr& socket : *inSockets)
			{
				FD_SET(socket->m_Socket, &outSet);
			}
			return &outSet;
		}
		else
		{
			return nullptr;
		}
	}

	/// <summary>
	/// 将输入sockets中存在于fd_set中的socket过滤出来
	/// </summary>
	/// <param name="outSockets"></param>
	/// <param name="inSockets"></param>
	/// <param name="inSet"></param>
	static void FillVectorFromSet(std::vector<TCPSocketPtr>* outSockets, const std::vector<TCPSocketPtr>* inSockets, const fd_set* inSet)
	{
		if (inSockets && outSockets)
		{
			outSockets->clear();
			for (const TCPSocketPtr& socket : *inSockets)
			{
				if (FD_ISSET(socket->m_Socket, inSet))
				{
					outSockets->emplace_back(socket);
				}
			}
		}
	}

	/// <summary>
	/// 封装select函数，
	/// </summary>
	/// <param name="inReadSet"></param>
	/// <param name="outReadSet"></param>
	/// <param name="inWriteSet"></param>
	/// <param name="outWriteSet"></param>
	/// <param name="inExceptSet"></param>
	/// <param name="outExceptSet"></param>
	/// <returns></returns>
	static int Select(const std::vector<TCPSocketPtr>* inReadSet, std::vector<TCPSocketPtr>* outReadSet,
		const std::vector<TCPSocketPtr>* inWriteSet, std::vector<TCPSocketPtr>* outWriteSet,
		const std::vector<TCPSocketPtr>* inExceptSet, std::vector<TCPSocketPtr>* outExceptSet)
	{
		fd_set read, write, except;

		fd_set* readPtr = FillSetFromVector(read, inReadSet);
		fd_set* writePtr = FillSetFromVector(write, inWriteSet);
		fd_set* exceptPtr = FillSetFromVector(except, inExceptSet);

		int toRet = select(0, readPtr, writePtr, exceptPtr, nullptr);
		if (toRet > 0)
		{
			FillVectorFromSet(outReadSet, inReadSet, readPtr);
			FillVectorFromSet(outWriteSet, inWriteSet, writePtr);
			FillVectorFromSet(outExceptSet, inExceptSet, exceptPtr);
		}

		return toRet;
	}
};

void DoTCPLoop()
{
	TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(EnumSocketAddressFamily::INET);
	SocketAddress receivingAddress(INADDR_ANY, 48000);
	if (listenSocket->Bind(receivingAddress) != NO_ERROR)
	{
		return;
	}
	std::vector<TCPSocketPtr> readBlockSockets;
	readBlockSockets.push_back(listenSocket);

	std::vector<TCPSocketPtr> readableSockets;

	while (g_IsGameRunning)
	{
		int ret = SocketUtil::Select(&readBlockSockets, &readableSockets, nullptr, nullptr, nullptr, nullptr);
		if (ret > 0)
		{
			for (const TCPSocketPtr& socketPtr : readableSockets)
			{
				if (socketPtr == listenSocket)
				{
					// 监听Socket，处理远程主机的连接请求
					SocketAddress newClientAddress;
					TCPSocketPtr newTcpSocket = listenSocket->Accept(newClientAddress);
					readBlockSockets.push_back(newTcpSocket);
					// TODO 新链接处理
				}
				else
				{
					// 连接Socket，接收数据
					char segment[GOOD_SEGMENT_SIZE];
					int dataReceived = socketPtr->Receive(segment, GOOD_SEGMENT_SIZE);
					if (dataReceived > 0)
					{
						// TODO 接收远程数据处理
					}
				}
			}
		}
	}
}

int TestSocketFunc()
{
	// 首先初始化WinSock库
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int errorCode = WSAStartup(wVersionRequested, &wsaData);
	if (errorCode != 0)
	{
		std::cerr << "Failture to init WSA" << std::endl;
		return -1;
	}
	// 创建一个Socket
	SOCKET tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (tcpSocket == INVALID_SOCKET)
	{
		std::cerr << "Invalid socket create!" << std::endl;
		// 获取错误代码，确认错误原因
		switch (WSAGetLastError())
		{
		case WSANOTINITIALISED:
			std::cerr << "Need inititalize WSA first!" << std::endl;
			break;
		default:
			break;
		}
		return -1;
	}
	std::cout << "Create WinSocket Success." << std::endl;

	// TODO .... 学习使用Windows系统下的Socket

	// 停止Socket，SD_SEND:停止发送，SD_RECEIVE:停止接收，SD_BOTH:停止发送和接收
	shutdown(tcpSocket, SD_SEND);
	// 关闭Socket
	closesocket(tcpSocket);
	// 程序结束，清理WSA
	WSACleanup();

	return 0;
}