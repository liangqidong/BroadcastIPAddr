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
}

UDPSocket::~UDPSocket()
{
	closesocket(m_connectSocket); 
	WSACleanup();
}

long UDPSocket::SetReceivePort(unsigned int port)
{
	m_clientAddr.sin_family = AF_INET;
	m_clientAddr.sin_port = htons(port);
	m_clientAddr.sin_addr.s_addr = INADDR_BROADCAST;
	return 0;
}

long UDPSocket::SetBindPort(unsigned int port)
{
	m_bindAddr.sin_family = AF_INET;
	m_bindAddr.sin_port = htons(port);
	m_bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	return 0;
}

long UDPSocket::bindSocket()
{
	int err = 0;
	m_connectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == m_connectSocket)
	{
		err = WSAGetLastError();
		printf("\"socket\" error! error code is %d\n", err);
		return -1;
	}
	bool bOpt = true;
	int a = setsockopt(m_connectSocket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));
	if (SOCKET_ERROR == a)
	{
		err = WSAGetLastError();
		printf("\"bind\" error! error code is %d\n", err);
		return -1;
	}
	// 绑定套接字   
	err = bind(m_connectSocket, (sockaddr*)&m_bindAddr, sizeof(sockaddr_in));
	if (SOCKET_ERROR == err)
	{
		err = WSAGetLastError();
		printf("\"bind\" error! error code is %d\n", err);
		return -1;
	}

	return 0;
}

long UDPSocket::SendData(char* data, unsigned int& dataLen)
{
	dataLen = sendto(m_connectSocket, data, dataLen, 0, (sockaddr*)&m_clientAddr, sizeof(SOCKADDR));
	if (SOCKET_ERROR == dataLen)
	{
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
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	FD_ZERO(&readfds);
	FD_SET(m_connectSocket, &readfds);
	if (select(m_connectSocket + 1, &readfds, NULL, NULL, &tv) > 0)
	{
		dataLen = recvfrom(m_connectSocket, data, dataLen, 0, (SOCKADDR*)&m_clientAddr, &nAddrLen);
		if (SOCKET_ERROR == dataLen)
		{
			printf("received data error!\n");
			return -1;
		}
	}
	else
	{
		printf("timeout!there is no data arrived!\n");
		return -1;
	}
	return 0;
}

long UDPSocket::GetLocalIP(char* ip)
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
	strcpy(ip, inet_ntoa(*(in_addr*)*host->h_addr_list));
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
					strcpy(ips[i].ip, pIpAddrString->IpAddress.String);
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

long UDPSocket::GetSocketIP(char* ip)
{
	strcpy(ip, inet_ntoa(m_clientAddr.sin_addr));
	return 0;
}

