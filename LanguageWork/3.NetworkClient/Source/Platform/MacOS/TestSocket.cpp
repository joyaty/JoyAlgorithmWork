
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/in_var.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>

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
