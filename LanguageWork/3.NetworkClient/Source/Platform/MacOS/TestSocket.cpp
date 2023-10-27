
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

int TestSocket()
{
    int tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (tcpSocket == -1) {
        std::cout<< "Invalid socket create!" <<std::endl;
    }
    
    shutdown(tcpSocket, 1);
    
    
    
}
