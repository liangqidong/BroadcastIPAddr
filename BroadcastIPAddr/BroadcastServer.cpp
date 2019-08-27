#include "BroadcastServer.h"
#include <regex> 
#include <iostream>

#define RecBuffLen  1024
#define RecTimeOut  100 //ms
BroadcastServer::BroadcastServer()
	: m_recDataFun(nullptr)
	, m_recDataThread(nullptr)
{
}


BroadcastServer::~BroadcastServer()
{
	if (nullptr != m_recDataThread && m_recDataThread->joinable())
	{
		m_recDataThread->join();
		delete m_recDataThread;
		m_recDataThread = nullptr;
	}
}

long BroadcastServer::SendData(const char* data, unsigned int& dataLen, const char* addrIP)
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
	return m_udpSocket.SendData(data,dataLen, addr);
}

long BroadcastServer::RegRecFun(BroadcastRecDataFun recDataFun)
{
	m_recDataFun = recDataFun;
	return 0;
}

long BroadcastServer::BindReceivePort(unsigned int port)
{
	m_udpSocket.SetReceivePort(port);
	return 0;
}

long BroadcastServer::bindSendPort(unsigned int port)
{
	m_udpSocket.SetSendPort(port);
	return 0;
}

long BroadcastServer::Process()
{
	m_udpSocket.bindSocket();
	if (nullptr != m_recDataThread)
	{
		return 0;
	}
	m_recDataThread = new std::thread(&BroadcastServer::RecDataThread,this);
	return 0;
}

void BroadcastServer::RecDataThread()
{
	char buff[RecBuffLen] = { 0 };
	unsigned int dataLen = RecBuffLen;
	char ip[MAX_PATH] = { 0 };
	unsigned int port = 0;
	unsigned int ipBuffLen = MAX_PATH;
	while (true)
	{
		if (-1 == m_udpSocket.RecvData(buff, dataLen,RecTimeOut))
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
