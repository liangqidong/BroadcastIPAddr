#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include <WinSock2.h>  
#include <stdio.h>  
#pragma comment(lib, "ws2_32.lib")  

class UDPSocket
{
public:
	UDPSocket();
	~UDPSocket();
	typedef struct tagIPInfo
	{
		char ip[30];
		tagIPInfo()
		{
			memset(ip, 0, 30);
		}
	}IPInfo;

	long SetReceivePort(unsigned int port);
	long SetBindPort(unsigned int port);
	long bindSocket();
	long SendData(char* data, unsigned int& dataLen);
	long RecvData(char* data, unsigned int& dataLen, unsigned int timeout = 50);
	long GetLocalIP(char* ip);
	long GetLocalIPs(IPInfo* ips, int& ipNum);
	long GetSocketIP(char* ip);

private:
	SOCKADDR_IN m_clientAddr;
	SOCKADDR_IN m_bindAddr;
	SOCKET		m_connectSocket;
};

#endif//UDP_SOCKET_H

