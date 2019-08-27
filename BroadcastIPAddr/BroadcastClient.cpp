#include "BroadcastClient.h"
#include <regex> 

#define RecBuffLen  1024
#define RecTimeOut  100 //ms

BroadcastClient::BroadcastClient()
{
}


BroadcastClient::~BroadcastClient()
{
}

long BroadcastClient::SendData(const char* data, unsigned int& dataLen, const char* addrIP)
{
	std::regex regex = std::regex("^(25[0-5]|2[0-4]\\d|[0-1]\\d{2}|[1-9]?\\d)\\.(25[0-5]|2[0-4]\\d|[0-1]\\d{2}|[1-9]?\\d)\\.(25[0-5]|2[0-4]\\d|[0-1]\\d{2}|[1-9]?\\d)\\.(25[0-5]|2[0-4]\\d|[0-1]\\d{2}|[1-9]?\\d)$");
	if (!std::regex_match(addrIP, regex))
	{
		return -1;
	}

	unsigned long addr = inet_addr(addrIP);
	regex = std::regex("^([0])\\.([0])\\.([0])\\.([0])$");
	if (std::regex_match(addrIP, regex))
	{
		addr = INADDR_BROADCAST;
	}
	return m_udpSocket.SendData(data, dataLen, addr);
}
long BroadcastClient::RegRecFun(BroadcastRecDataFun recDataFun)
{
	m_recDataFun = recDataFun;
	return 0;
}

long BroadcastClient::BindReceivePort(unsigned int port)
{
	m_udpSocket.SetReceivePort(port);
	return 0;
}

long BroadcastClient::bindSendPort(unsigned int port)
{
	m_udpSocket.SetSendPort(port);
	return 0;
}

long BroadcastClient::Process()
{
	if (nullptr != m_recDataThread)
	{
		return 0;
	}
	m_recDataThread = new std::thread(&BroadcastClient::RecDataThread, this);
}

void BroadcastClient::RecDataThread()
{
	char buff[RecBuffLen] = { 0 };
	unsigned int dataLen = RecBuffLen;
	char ip[MAX_PATH] = { 0 };
	unsigned int port = 0;
	unsigned int ipBuffLen = MAX_PATH;
	while (true)
	{
		if (-1 == m_udpSocket.RecvData(buff, dataLen, RecTimeOut))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}
		if (nullptr == m_recDataFun)
		{
			continue;
		}
		m_udpSocket.GetSocketIP(ip, ipBuffLen);
		m_udpSocket.GetSocketPort(port);
		m_recDataFun(ip, port, buff, dataLen);
		memset(buff, 0, RecBuffLen);
		memset(ip, 0, MAX_PATH);
		dataLen = RecBuffLen;
	}
}

