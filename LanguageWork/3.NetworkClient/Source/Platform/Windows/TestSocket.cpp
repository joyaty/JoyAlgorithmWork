
#include <WinSock2.h>
#include <iostream>

// 链接windows系统的socket2支持库，当前系统版本为2.2
// #pragma comment(lib, "ws2_32.lib") // CMake中配置链接库

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