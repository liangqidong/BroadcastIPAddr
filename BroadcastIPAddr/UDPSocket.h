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
		char ip[MAX_PATH];
		tagIPInfo()
		{
			memset(ip, 0, MAX_PATH);
		}
	}IPInfo;

	long SetSendPort(unsigned int port);
	long SetReceivePort(unsigned int port);
	long bindSocket();
	long SendData(const char* data, unsigned int& dataLen, unsigned long addr = INADDR_BROADCAST);//addr = inet_addr("127.0.0.1")
	long RecvData(char* data, unsigned int& dataLen, unsigned int timeout = 50);
	long GetLocalIP(char* ip, unsigned int& dataLen);
	long GetLocalIPs(IPInfo* ips, int& ipNum);
	long GetSocketIP(char* ip, unsigned int& dataLen);
	long GetSocketPort(unsigned int& port);

private:
	SOCKADDR_IN m_peeraddr;
	SOCKET		m_serverSocket;
	unsigned int m_sendPort;
	unsigned int m_receivePort;
	bool		m_bindSocket;
};

#endif//UDP_SOCKET_H

