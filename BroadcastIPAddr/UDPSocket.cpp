#include "UDPSocket.h"
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")

UDPSocket::UDPSocket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	// 启动socket api   
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return ;
	}
	m_serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	if (INVALID_SOCKET == m_serverSocket)
	{
		err = WSAGetLastError();
		printf("\"socket\" error! error code is %d\n", err);
		return ;
	}
	m_receivePort = 0;
	m_sendPort = 0;
	m_bindSocket = false;
}

UDPSocket::~UDPSocket()
{
	closesocket(m_serverSocket); 
	WSACleanup();
}

long UDPSocket::SetSendPort(unsigned int port)
{
	m_sendPort = port;
	return 0;
}

long UDPSocket::SetReceivePort(unsigned int port)
{
	m_receivePort = port;
	return 0;
}

long UDPSocket::bindSocket()
{
	// 绑定套接字 
	int err = 0;
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(m_receivePort);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	err = bind(m_serverSocket, (sockaddr*)&serverAddr, sizeof(sockaddr_in));
	if (SOCKET_ERROR == err)
	{
		err = WSAGetLastError();
		printf("\"bind\" error! error code is %d\n", err);
		return -1;
	}
	m_bindSocket = true;
	return 0;
}

long UDPSocket::SendData(const char* data, unsigned int& dataLen, unsigned long addr)
{
	SOCKADDR_IN clientAddr;
	memset(clientAddr.sin_zero, 0, 8);
	int port = m_sendPort;
	if ( 0 == m_sendPort)
	{
		port = ntohs(m_peeraddr.sin_port);
	}
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(port);
	clientAddr.sin_addr.s_addr = addr;
	SOCKET clientSocket = m_serverSocket;
	if (m_bindSocket)
	{
		clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (INVALID_SOCKET == clientSocket)
		{
			int err = WSAGetLastError();
			printf("error! error code is %d/n", err);
			return -1;
		}
	}
	if (INADDR_BROADCAST == addr)
	{
		bool bOpt = true;
		int a = setsockopt(clientSocket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));
		if (SOCKET_ERROR == a)
		{
			int err = WSAGetLastError();
			printf("\"SendData setsockopt\" error! error code is %d\n", err);
			return -1;
		}
	}
	dataLen = sendto(clientSocket, data, dataLen, 0, (SOCKADDR*)&clientAddr, sizeof(SOCKADDR));
	if (SOCKET_ERROR == dataLen)
	{
		int err = WSAGetLastError();
		printf("\"sendto\" error! error code is %d\n", err);
		return -1;
	}
	return 0;
}

long UDPSocket::RecvData(char* data, unsigned int& dataLen, unsigned int timeout)
{
	int err = 0;
	int nAddrLen = sizeof(SOCKADDR);
	fd_set readfds;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = timeout*1000;
	FD_ZERO(&readfds);
	FD_SET(m_serverSocket, &readfds);
	if (select(m_serverSocket + 1, &readfds, NULL, NULL, &tv) > 0)
	{
		printf("recvfrom in!\n");
		dataLen = recvfrom(m_serverSocket, data, dataLen, 0, (sockaddr*)&m_peeraddr, &nAddrLen);
		if (SOCKET_ERROR == dataLen)
		{
			printf("received data error!\n");
			return -1;
		}
		if (SOCKET_ERROR == dataLen)
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
	return 0;
}

long UDPSocket::GetLocalIP(char* ip, unsigned int& dataLen)
{
	
	char hostname[256] = { 0 };
	int ret = gethostname(hostname, sizeof(hostname));
	if (ret == SOCKET_ERROR)
	{
		return -1;
	}
	
	HOSTENT* host = gethostbyname(hostname);
	if (host == NULL)
	{
		return -1;
	}
	strcpy_s(ip, dataLen,inet_ntoa(*(in_addr*)*host->h_addr_list));
	return 0;
}

long UDPSocket::GetLocalIPs(IPInfo* ips, int& ipNum)
{
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);

	int netCardNum = 0;
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	int i = 0;
	if (ERROR_SUCCESS == nRel)
	{
		while (pIpAdapterInfo)
		{
			//可能网卡有多IP,因此通过循环去判断
			IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			do
			{
				if (ipNum > i)
				{
					strcpy_s(ips[i].ip, MAX_PATH,pIpAddrString->IpAddress.String);
				}
				pIpAddrString = pIpAddrString->Next;
				i++;
			} while (pIpAddrString);
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
	ipNum = i;
	return 0;
}

long UDPSocket::GetSocketIP(char* ip, unsigned int& dataLen)
{
	strcpy_s(ip, dataLen,inet_ntoa(m_peeraddr.sin_addr));
	return 0;
}

long UDPSocket::GetSocketPort(unsigned int& port)
{
	port = ntohs(m_peeraddr.sin_port);
	return 0;
}

